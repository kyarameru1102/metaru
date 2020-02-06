#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GameTime.h"
#include "Astar.h"
#include "Bullet.h"

//�X�e�[�g�ꗗ�B
EnemyStateBattlePosture		EnemyState::m_battlePosture;
EnemyStateHesitate			EnemyState::m_hesitate;
EnemyStateVigilance			EnemyState::m_vigilance;
EnemyStateVigilanceCancel	EnemyState::m_vigilanceCancel;

struct AStarSmoothingCallBack : public btCollisionWorld::ClosestConvexResultCallback
{
	bool hit = false;
	AStarSmoothingCallBack() :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f))
	{}
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		hit = true;
		return 0.0;
	}
};

struct ShotCallBack : public btCollisionWorld::ClosestConvexResultCallback
{
	bool hit = false;
	ShotCallBack() :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f))
	{}
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		hit = true;
		return 0.0;
	}
};

Enemy::Enemy()
{
	m_collider.Create(1.0f, 30.0f);
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/heisi_idle_L.tka");
	m_animClips[enAnimationClip_run].Load(L"Assets/animData/heisi_run.tka");
	m_animClips[enAnimationClip_shot].Load(L"Assets/animData/heisi_shot.tka");
	m_animClips[enAnimationClip_walk].Load(L"Assets/animData/heisi_walk.tka");
	m_animClips[enAnimationClip_reload].Load(L"Assets/animData/heisi_reload.tka");
	m_animClips[enAnimationClip_death].Load(L"Assets/animData/heisi_death.tka");

	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);
	m_animClips[enAnimationClip_shot].SetLoopFlag(true);
	m_animClips[enAnimationClip_walk].SetLoopFlag(true);

	Init();
	m_charaCon.Init(
		10.0f,
		10.0f,
		m_position
	);

	Game* game = GetGame();
	m_player = game->GetPlayer();
	ChangeState(&EnemyState::m_hesitate);
	
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo", m_animClips, enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle,true);
	m_currentPath = 0;
	m_position = PathList[0].position;

	m_moveSpeed = CVector3::Zero();
	m_moveSpeed.x = -1.0f;
	m_moveSpeed.Normalize();
	return true;
}

void Enemy::Update()
{
	if (m_position.y <= -10.0f) {
		m_hp = 0;
	}
	Damage();
	
	if (!m_death) {
		//�ړ��n�����B
		{
			//�p�j���B
			if (m_currentstate == &EnemyState::m_hesitate) {
				PathMove();
			}
			//�x���Ԑ��ɓ�������B
			if (m_currentstate == &EnemyState::m_vigilance) {
				VigilanceMove();
			}
			//�x���Ԑ����������ꂽ��B
			if (m_currentstate == &EnemyState::m_vigilanceCancel) {
				VigilanceCancelMove();
			}
			if (m_currentstate == &EnemyState::m_battlePosture) {
				BattleMove();
			}
		}
		//�����[�h�B
		if (m_relodeOn) {
			m_skinModelRender->PlayAnimation(enAnimationClip_reload, true, 0.3);
			m_reloadTimer--;
			if (m_reloadTimer <= 0) {
				m_ammo = 30;
				m_relodeOn = false;
				m_reloadTimer = 130;
			}
		}

		//����p�̌v�Z�B
		ViewingAngle();
		//��Q�������邩����B
		ShotPossible();


		//�p�j���������͌x���Ԑ����������Ȃ�B
		if (m_currentstate == &EnemyState::m_hesitate || m_currentstate == &EnemyState::m_vigilanceCancel) {
			//����p�ɓ������Ƃ��̏����B
			if (m_angle < 65.0f && m_toPlayerLen < 2500.0f && !m_hit) {
				if (m_currentstate != &EnemyState::m_vigilance) {
					//�x���̐��Ɉڍs�B
					ChangeState(&EnemyState::m_vigilance);
				}

			}
		}
		//�x���Ԑ����Ȃ�B
		if (m_currentstate == &EnemyState::m_vigilance) {
			//����p�ɓ������Ƃ��̏����B
			if (m_angle < 90.0f && m_toPlayerLen < 1000.0f && !m_hit) {
				if (m_currentstate != &EnemyState::m_battlePosture) {
					//�퓬�̐��Ɉڍs�B
					ChangeState(&EnemyState::m_battlePosture);
					AstarEXEcount = 0;
				}
			}
		}

		//��Q�������邩�ǂ����B
		if (!m_hit) {
			//�퓬��Ԃ��ǂ����B
			if (m_currentstate == &EnemyState::m_battlePosture) {
				if (m_angle < 65.0f && m_toPlayerLen < 1000.0f) {
					//��Q�����Ȃ��Ď���p�̒��ɂ���Ȃ猂�B
					//�c�e������Ό��B
					if (m_ammo >= 0) {
						if (m_shotTimer == 0) {
							Firing();
							m_shotTimer = 5;
							m_ammo--;
						}
						else {
							m_moveSpeed = m_player->GetPosition() - m_position;
							m_moveSpeed.Normalize();
							m_moveSpeed *= 0.1f;
						}
						m_skinModelRender->PlayAnimation(enAnimationClip_shot, true, 0.3);
						m_onFiring = true;
						m_shotTimerOn = true;
					}
					//�����[�h�����B
					else {
						if (m_ammo != 30) {
							m_relodeOn = true;
						}
						m_shotTimer = 0;
						m_shotTimerOn = false;
					}
				}
				else {
					m_skinModelRender->PlayAnimation(enAnimationClip_run, true, 0.3);
					m_onFiring = false;
					m_shotTimer = 0;
					m_shotTimerOn = false;
				}
				if (m_shotTimerOn) {
					m_shotTimer--;
				}
			}
		}


		MoveAnimation();
		Rotation();
		m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
		m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
		m_skinModelRender->SetRotation(m_rotation);
		m_skinModelRender->SetPosition(m_position);
	}
	else {
		m_itai = false;
	}
}

void Enemy::Init()
{
	std::wstring p = L"assets/level/Enemypath_";
	p += m_initPath;
	p += L".tkl";

	std::vector<Path> kari;
	
	m_level.Init(p.c_str(),
		[&](LevelObjectData& obiData)->bool {
			wchar_t last[3];
			size_t l = wcslen(obiData.name);
			wchar_t la = obiData.name[l - 2];
			last[0] = la;
			la = obiData.name[l - 1];
			last[1] = la;
			last[2] = L'\0';
			Path path;
			path.position = obiData.position;
			char las[3];
			wcstombs(las, last, sizeof(last));
			path.myPath = atoi(las);
			path.next = path.myPath + 1;
			kari.push_back(path);
			return 1;
		}
	);
	PathList.resize(kari.size());
	for (int i = 0; i < kari.size(); i++) {
		PathList[kari[i].myPath] = kari[i];
	}
	PathList[PathList.size() - 1].next = 0;
}
//�v���C���[��������܂ł̃p�X�ړ��B
void Enemy::PathMove()
{
	if (PathList.size() != 1) {
		m_moveSpeed = PathList[PathList[m_currentPath].next].position - m_position;
		float len = m_moveSpeed.Length();
		m_moveSpeed.Normalize();
		m_moveSpeed += m_moveSpeed * 90.0f;
		if (len < 10.0f) {
			m_currentPath = PathList[m_currentPath].next;
		}
	}
}
//�x�����̈ړ��B
void Enemy::VigilanceMove()
{
	if (AstarEXEcount == 0) {
		m_astar.Execute(m_position, m_player->GetPosition());
		AstarEXEcount++;
	}

	AstarSmooth();
	
	//A*�o�H�T���ŏo�����ʂŃp�X�ړ��B
	m_moveSpeed = m_smoothPos - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 200.0f;
	
	if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 40.0f) {
		//�p�X�̍Ō�܂ōs������B
		
		ChangeState(&EnemyState::m_vigilanceCancel);
		m_astar.Execute(m_position, PathList[PathList[m_currentPath].next].position);
		
		AstarEXEcount = 0;
	}
}
//�x���Ԑ��������̈ړ��B
void Enemy::VigilanceCancelMove()
{
	
	AstarSmooth();

	m_moveSpeed = m_smoothPos - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 200.0f;
	
	if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 30.0f) {
		ChangeState(&EnemyState::m_hesitate);
		m_moveSpeed = CVector3::Zero();
		m_moveSpeed.x = -1.0f;
		m_moveSpeed.Normalize();
	}
	
}
//�퓬�Ԑ����̈ړ��B
void Enemy::BattleMove()
{
	if (!m_hit && (m_player->GetPosition() - m_position).Length() <= 2000.0f 
		&& (m_player->GetPosition().y - m_position.y) < 5.0f 
		&& (m_player->GetPosition().y - m_position.y) > -5.0f) {
		if (!m_onFiring) {
			if (!m_relodeOn) {
				m_moveSpeed = m_player->GetPosition() - m_position;
				m_moveSpeed.Normalize();
				m_moveSpeed *= 200.0f;
			}
		}
	}
	else {
		//����p�ɂ��Ȃ���΁B
		if (!m_onFiring) {
			if (AstarEXEcount == 50 || AstarEXEcount == 0) {
				m_astar.Execute(m_position, m_player->GetPosition());
				AstarEXEcount = 0;
			}
			AstarEXEcount++;

			//A*�X���[�W���O�B
			AstarSmooth();

			if (!m_onFiring || m_hit) {
				//A*�o�H�T���ŏo�����ʂŃp�X�ړ��B
				if (!m_relodeOn) {
					m_moveSpeed = m_smoothPos - m_position;
					m_moveSpeed.Normalize();
					m_moveSpeed += m_moveSpeed * 200.0f;
				}
			}
			if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 30.0f) {
				//�p�X�̍Ō�܂ōs������B
				m_astar.Execute(m_position, m_player->GetPosition());
				AstarEXEcount = 0;
			}
		}
	}
}
//���C�B
void Enemy::Firing()
{
	CVector3 EnemyBulletDrc;
	EnemyBulletDrc = m_player->GetPosition() - m_position;
	EnemyBulletDrc.Normalize();
	EnemyBulletDrc *= 100.0f;
	Bullet* bullet = nullptr;
	bullet = NewGO<Bullet>(0, "bullet");
	bullet->SetPosition(m_position);
	bullet->SetmoveSpeed(EnemyBulletDrc);
	bullet->SetEnemy();
	EnemyBulletDrc.Normalize();
	m_moveSpeed = EnemyBulletDrc * 0.1;
	
}
//�ړ����̃A�j���[�V�����B
void Enemy::MoveAnimation()
{
	
	if (!m_onFiring)
	{

		if (m_moveSpeed.Length() >= 100.f) {
			m_skinModelRender->PlayAnimation(enAnimationClip_run, true, 0.5);
			m_skinModelRender->PlayAnimation(enAnimationClip_run, false, 0.5);
		}
		else if (m_moveSpeed.Length() >= 1.1f) {
			m_skinModelRender->PlayAnimation(enAnimationClip_walk, true, 0.5);
			m_skinModelRender->PlayAnimation(enAnimationClip_walk, false, 0.5);
		}
		else {
			if (!m_relodeOn) {
				m_skinModelRender->PlayAnimation(enAnimationClip_idle, true, 0.5);
			}
			m_skinModelRender->PlayAnimation(enAnimationClip_idle, false, 0.5);
		}
	}
	else {
		m_skinModelRender->PlayAnimation(enAnimationClip_shot, false, 0.3);
	}
}
//��]�B
void Enemy::Rotation()
{
	CVector3 moveSpeedXZ = m_moveSpeed;
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();
	if (moveSpeedXZ.LengthSq() < 0.5f) {
		return;
	}
	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}
//�X�e�[�g��؂�ւ���B
void Enemy::ChangeState(IEnemyState* nextState)
{
	if (m_currentstate != nullptr)
	{
		m_currentstate->Leave();
	}
	m_currentstate = nextState;
	m_currentstate->Enter();
}
//�_���[�W���󂯂�B
void Enemy::Damage()
{
	CVector3 EnemyCenter = m_position;
	EnemyCenter.y += 80;
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		if ((bullet->GetPosition() - EnemyCenter).Length() <= 150.0f)
		{
			//�v���C���[�̒e�ۂȂ�B
			if (bullet->GetWhosebullet())
			{
				m_hp--;
				m_itai = true;
			}
			else {
				m_itai = false;
			}
		}
		else {
			m_itai = false;
		}
		return true;
		});
	if (m_hp <= 0) {
		m_death = true;
		m_skinModelRender->PlayAnimation(enAnimationClip_death, true, 0.3f);
		m_skinModelRender->PlayAnimation(enAnimationClip_death, false, 0.3f);
		m_charaCon.RemoveRigidBoby();
		m_notLookOn = true;
		//DeleteGO(this);
	}
}
//����p�B
void Enemy::ViewingAngle()
{
	CVector3 toPlayer = m_player->GetPosition() - m_position;
	m_toPlayerLen = toPlayer.Length();
	toPlayer.Normalize();
	if (m_moveSpeed.Length() > 0.01) {
		m_oldMoveSpeed = m_moveSpeed;
	}
	m_oldMoveSpeed.Normalize();
	float angle = toPlayer.Dot(m_oldMoveSpeed);

	angle = acos(angle);
	float a = fabsf(angle);

	//CMath::DegToRad(15.0f);
	m_angle = CMath::RadToDeg(a);
}
//�v���C���[�Ƃ̊Ԃɏ�Q�������邩�ǂ����B
void Enemy::ShotPossible()
{
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(btVector3(m_position.x, m_position.y + 80.0f, m_position.z));
	end.setOrigin(btVector3(m_player->GetPosition().x, m_player->GetPosition().y + 80.0f, m_player->GetPosition().z));
	ShotCallBack callBack;
	g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);
	if (callBack.hit) {
		m_hit = true;
	}
	else {
		m_hit = false;
	}
}

void Enemy::AstarSmooth()
{
	float time = 0;
	while (1)
	{
		time++;
		//�X���[�W���O�����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		CVector3 nextPos = m_astar.GetAStarAnswerPos();
		start.setOrigin(btVector3(m_position.x, m_position.y + 80.0f, m_position.z));
		end.setOrigin(btVector3(nextPos.x, nextPos.y + 80.0f, nextPos.z));
		AStarSmoothingCallBack callBack;
		if (callBack.hit == true || time >= 10) {
			break;
		}
		g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);
		if (callBack.hit == false) {
			if (m_astar.GetAStarAnswerIt() != m_astar.GetAStarAnswerEnd()) {
				m_smoothPos = m_astar.GetAStarAnswerPos();

				if ((nextPos.y - m_position.y) <= 5.0f && (nextPos.y - m_position.y) >= -5.0f) {
					m_astar.AdvanceIt();
				}
			}
		}
	}
}
