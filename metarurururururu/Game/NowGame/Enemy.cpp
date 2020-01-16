#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GameTime.h"
#include "Astar.h"
#include "Bullet.h"

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
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/heisi_idle.tka");
	m_animClips[enAnimationClip_run].Load(L"Assets/animData/heisi_run.tka");
	m_animClips[enAnimationClip_shot].Load(L"Assets/animData/heisi_shot.tka");

	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);

	Init();
	m_charaCon.Init(
		10.0f,
		10.0f,
		m_position
	);
	m_player = FindGO<Player>("player");
	ChangeState(&EnemyState::m_hesitate);
	
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo", m_animClips, enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle);
	m_currentPath = 0;
	m_position = PathList[0].position;

	return true;
}

void Enemy::Update()
{
	CVector3 EnemyCenter = m_position;
	EnemyCenter.y += 80;
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		if ((bullet->GetPosition() - EnemyCenter).Length() <= 50.0f)
		{
			//�v���C���[�̒e�ۂȂ�B
			if (bullet->GetWhosebullet())
			{
				m_hp--;
			}
		}
		return true;
	});
	if (m_hp <= 0) {
		m_death = true;
	}
	m_moveSpeed = CVector3::Zero();
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

		//����p�̌v�Z�B
		CVector3 toPlayer = m_player->GetPosition() - m_position;
		float toPlayerLen = toPlayer.Length();
		toPlayer.Normalize();
		if (m_moveSpeed.Length() > 0.01) {
			m_oldMoveSpeed = m_moveSpeed;
		}
		m_oldMoveSpeed.Normalize();
		float angle = toPlayer.Dot(m_oldMoveSpeed);

		angle = acos(angle);
		float a = fabsf(angle);

		CMath::DegToRad(15.0f);
		float ab = CMath::RadToDeg(a);

		//�p�j���������͌x���Ԑ����������Ȃ�B
		if (m_currentstate == &EnemyState::m_hesitate || m_currentstate == &EnemyState::m_vigilanceCancel) {

			//����p�ɓ������Ƃ��̏����B
			if (ab < 15.0f && toPlayerLen < 1500.0f) {
				if (m_currentstate != &EnemyState::m_vigilance) {
					//�x���̐��Ɉڍs�B
					ChangeState(&EnemyState::m_vigilance);
				}

			}
		}
		//�x���Ԑ����Ȃ�B
		if (m_currentstate == &EnemyState::m_vigilance) {
			//����p�ɓ������Ƃ��̏����B
			if (ab < 45.0f && toPlayerLen < 500.0f) {
				if (m_currentstate != &EnemyState::m_battlePosture) {
					//�퓬�̐��Ɉڍs�B
					ChangeState(&EnemyState::m_battlePosture);
					AstarEXEcount = 0;
				}
			}
			//else {
			//	if (m_currentstate != &EnemyState::m_vigilance) {
			//		//�x���̐��Ɉڍs�B
			//		ChangeState(&EnemyState::m_vigilance);
			//	}
			//}
		}

		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(m_position.x, m_position.y + 80.0f, m_position.z));
		end.setOrigin(btVector3(m_player->GetPosition().x, m_player->GetPosition().y + 80.0f, m_player->GetPosition().z));
		ShotCallBack callBack;
		g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);

		if (m_currentstate == &EnemyState::m_battlePosture) {
			if (callBack.hit == false) {
				if (ab < 45.0f && toPlayerLen < 500.0f) {
					Firing();
					m_onFiring = true;
				}
				else {
					m_onFiring = false;
				}
			}
		}

	}
	Rotation();
	m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
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
			//AddPath(path);
			kari.push_back(path);
			//OutputDebugString("aaa");
			return 1;
		}
	);
	PathList.resize(kari.size());
	for (int i = 0; i < kari.size(); i++) {
		PathList[kari[i].myPath] = kari[i];
	}
	PathList[PathList.size() - 1].next = 0;
}



void Enemy::PathMove()
{
	m_moveSpeed = PathList[PathList[m_currentPath].next].position - m_position;
	float len = m_moveSpeed.Length();
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 100.0f;
	if (len < 10.0f) {
		m_currentPath = PathList[m_currentPath].next;
	}
	MoveAnimation();
}

void Enemy::VigilanceMove()
{
	if (AstarEXEcount == 0) {
		m_astar.Execute(m_position, m_player->GetPosition());
		//m_astar2.Execute(m_position, m_player->GetPosition());
		//m_beforeAstar = m_position;
		AstarEXEcount++;
	}
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
			else {
			}
		}
	}
	time = 0;
	
	//A*�o�H�T���ŏo�����ʂŃp�X�ړ��B
	m_moveSpeed = m_smoothPos - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 100.0f;
	MoveAnimation();
	
	if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 40.0f) {
		//�p�X�̍Ō�܂ōs������B
		
		ChangeState(&EnemyState::m_vigilanceCancel);
		m_astar.Execute(m_position, PathList[PathList[m_currentPath].next].position);
		
		AstarEXEcount = 0;
	}
}

void Enemy::VigilanceCancelMove()
{
	
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
			else {
			}
		}
	}
	time = 0;

	m_moveSpeed = m_smoothPos - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 100.0f;
	MoveAnimation();
	
	if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 30.0f) {
		ChangeState(&EnemyState::m_hesitate);
	}
	
}

void Enemy::BattleMove()
{
	//������p�x�̌v�Z
	CVector3 toPlayer = m_player->GetPosition() - m_position;
	float toPlayerLen = toPlayer.Length();
	toPlayer.Normalize();
	if (m_moveSpeed.Length() > 0.01) {
		m_oldMoveSpeed = m_moveSpeed;
	}
	m_oldMoveSpeed.Normalize();
	float angle = toPlayer.Dot(m_oldMoveSpeed);

	angle = acos(angle);
	float a = fabsf(angle);

	CMath::DegToRad(15.0f);
	float ab = CMath::RadToDeg(a);
	//����p�ɓ�������B
	if (ab < 45.0f && toPlayerLen < 500.0f) {
		m_discovery = true;		//�������B
		AstarEXEcount = 0;
	}
	else {
		m_discovery = false;	//����p���ɂ��Ȃ��B
		//AstarEXEcount = 0;
	}
	//����p�ɂ��Ȃ���΁B
	if (!m_onFiring || !m_discovery) {
		if (AstarEXEcount == 50 || AstarEXEcount == 0) {
			m_astar.Execute(m_position, m_player->GetPosition());
			AstarEXEcount = 0;
		}
		AstarEXEcount++;

		
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
				else {
				}
			}
		}
		if ((m_astar.GetAStarAnswerPos() - m_position).Length() >= 40.0f) {
			
		}
		time = 0;
		if (!m_onFiring) {
			//A*�o�H�T���ŏo�����ʂŃp�X�ړ��B
			m_moveSpeed = m_smoothPos - m_position;
			m_moveSpeed.Normalize();
			m_moveSpeed += m_moveSpeed * 100.0f;
			MoveAnimation();
		}
		if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 30.0f) {
			//�p�X�̍Ō�܂ōs������B
			m_astar.Execute(m_position, m_player->GetPosition());
			//ChangeState(&EnemyState::m_vigilanceCancel);
			AstarEXEcount = 0;
		}
	}
}

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
	m_skinModelRender->PlayAnimation(enAnimationClip_shot, 0.3);
}

void Enemy::MoveAnimation()
{
	CVector3 toNextLength;
	CVector3 nextPos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	toNextLength = nextPos - m_position;
	if (toNextLength.Length() >= 1.0f)
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_run, 0.5);
	}
	else {
		m_skinModelRender->PlayAnimation(enAnimationClip_idle, 0.3);
	}
}

void Enemy::Rotation()
{
	CVector3 moveSpeedXZ = m_moveSpeed;
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();
	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}

void Enemy::ChangeState(IEnemyState* nextState)
{
	if (m_currentstate != nullptr)
	{
		m_currentstate->Leave();
	}
	m_currentstate = nextState;
	m_currentstate->Enter();
}
