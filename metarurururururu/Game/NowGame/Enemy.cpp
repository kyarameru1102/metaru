#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GameTime.h"
#include "Astar.h"
#include "Bullet.h"

//ステート一覧。
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
		//移動系処理。
		{
			//徘徊中。
			if (m_currentstate == &EnemyState::m_hesitate) {
				PathMove();
			}
			//警戒態勢に入ったら。
			if (m_currentstate == &EnemyState::m_vigilance) {
				VigilanceMove();
			}
			//警戒態勢が解除されたら。
			if (m_currentstate == &EnemyState::m_vigilanceCancel) {
				VigilanceCancelMove();
			}
			if (m_currentstate == &EnemyState::m_battlePosture) {
				BattleMove();
			}
		}
		//リロード。
		if (m_relodeOn) {
			m_skinModelRender->PlayAnimation(enAnimationClip_reload, true, 0.3);
			m_reloadTimer--;
			if (m_reloadTimer <= 0) {
				m_ammo = 30;
				m_relodeOn = false;
				m_reloadTimer = 130;
			}
		}

		//視野角の計算。
		ViewingAngle();
		//障害物があるか判定。
		ShotPossible();


		//徘徊中もしくは警戒態勢が解除中なら。
		if (m_currentstate == &EnemyState::m_hesitate || m_currentstate == &EnemyState::m_vigilanceCancel) {
			//視野角に入ったときの処理。
			if (m_angle < 65.0f && m_toPlayerLen < 2500.0f && !m_hit) {
				if (m_currentstate != &EnemyState::m_vigilance) {
					//警戒体制に移行。
					ChangeState(&EnemyState::m_vigilance);
				}

			}
		}
		//警戒態勢中なら。
		if (m_currentstate == &EnemyState::m_vigilance) {
			//視野角に入ったときの処理。
			if (m_angle < 90.0f && m_toPlayerLen < 1000.0f && !m_hit) {
				if (m_currentstate != &EnemyState::m_battlePosture) {
					//戦闘体制に移行。
					ChangeState(&EnemyState::m_battlePosture);
					AstarEXEcount = 0;
				}
			}
		}

		//障害物があるかどうか。
		if (!m_hit) {
			//戦闘状態かどうか。
			if (m_currentstate == &EnemyState::m_battlePosture) {
				if (m_angle < 65.0f && m_toPlayerLen < 1000.0f) {
					//障害物がなくて視野角の中にいるなら撃つ。
					//残弾があれば撃つ。
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
					//リロード処理。
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
//プレイヤーを見つけるまでのパス移動。
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
//警戒中の移動。
void Enemy::VigilanceMove()
{
	if (AstarEXEcount == 0) {
		m_astar.Execute(m_position, m_player->GetPosition());
		AstarEXEcount++;
	}

	AstarSmooth();
	
	//A*経路探査で出た結果でパス移動。
	m_moveSpeed = m_smoothPos - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 200.0f;
	
	if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 40.0f) {
		//パスの最後まで行ったら。
		
		ChangeState(&EnemyState::m_vigilanceCancel);
		m_astar.Execute(m_position, PathList[PathList[m_currentPath].next].position);
		
		AstarEXEcount = 0;
	}
}
//警戒態勢解除中の移動。
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
//戦闘態勢中の移動。
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
		//視野角にいなければ。
		if (!m_onFiring) {
			if (AstarEXEcount == 50 || AstarEXEcount == 0) {
				m_astar.Execute(m_position, m_player->GetPosition());
				AstarEXEcount = 0;
			}
			AstarEXEcount++;

			//A*スムージング。
			AstarSmooth();

			if (!m_onFiring || m_hit) {
				//A*経路探査で出た結果でパス移動。
				if (!m_relodeOn) {
					m_moveSpeed = m_smoothPos - m_position;
					m_moveSpeed.Normalize();
					m_moveSpeed += m_moveSpeed * 200.0f;
				}
			}
			if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd() && (m_smoothPos - m_position).Length() <= 30.0f) {
				//パスの最後まで行ったら。
				m_astar.Execute(m_position, m_player->GetPosition());
				AstarEXEcount = 0;
			}
		}
	}
}
//発砲。
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
//移動時のアニメーション。
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
//回転。
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
//ステートを切り替える。
void Enemy::ChangeState(IEnemyState* nextState)
{
	if (m_currentstate != nullptr)
	{
		m_currentstate->Leave();
	}
	m_currentstate = nextState;
	m_currentstate->Enter();
}
//ダメージを受ける。
void Enemy::Damage()
{
	CVector3 EnemyCenter = m_position;
	EnemyCenter.y += 80;
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		if ((bullet->GetPosition() - EnemyCenter).Length() <= 150.0f)
		{
			//プレイヤーの弾丸なら。
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
//視野角。
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
//プレイヤーとの間に障害物があるかどうか。
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
		//スムージング処理。
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
