#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GameTime.h"
#include "AStar/Astar.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	Init();
	m_charaCon.Init(
		20.0f,
		70.0f,
		m_position
	);
	m_player = FindGO<Player>("player");
	ChangeState(&m_hesitate);
	
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo");
	m_currentPath = 0;
	m_position = PathList[0].position;
	//m_astar.Execute(m_position, m_player->GetPosition());

	return true;
}

void Enemy::Update()
{
	m_moveSpeed = CVector3::Zero();
	//徘徊中。
	if (m_currentstate == &m_hesitate) {
		PathMove();
	}
	////戦闘態勢中の行動。
	//if (m_currentstate == &m_battlePosture) {
	//	BattleMove();
	//}
	//警戒態勢に入ったら。
	if (m_currentstate == &m_vigilance) {
		if (AstarEXEcount == 0) {
			m_astar.Execute(m_position, m_player->GetPosition());
			m_beforeAstar = m_position;
		}
		AstarEXEcount++;
		//A*経路探査で出た結果でパス移動。
		m_moveSpeed = m_astar.GetAStarAnswerPos() - m_position;
		if ((m_astar.GetAStarAnswerPos() - m_position).Length() < 50.0f)
		{
			m_astar.AdvanceIt();
			if (m_astar.GetAStarAnswerIt() != m_astar.GetAStarAnswerEnd()) {
				//////////////////////////////////////////////
				//ナビゲーションメッシュのリンク確認用処理。
				/*m_astarDebug = NewGO<AstarDebug>(0);
				m_astarDebug->SetPosition(m_position);
				auto len = m_astar.GetAStarAnswerPos();
				len = len - m_position;
				len.Normalize();
				CQuaternion Rot = CQuaternion::Identity();
				float kakuo = acos(len.Dot(CVector3::AxisY()));
				if (kakuo > 0.0f || kakuo < -FLT_MIN)
				{
					kakuo = CMath::RadToDeg(kakuo);
					CVector3 jiku;
					jiku.Cross(CVector3::AxisY(), len);
					if (kakuo > 0.0f || kakuo < -FLT_MIN)
					{
						jiku.Normalize();
						Rot.SetRotationDeg(jiku, kakuo);
					}

				}
				m_astarDebug->SetQuaternion(Rot);*/
				
				//////////////////////////////////////////////
			}
			else {
				//パスの最後まで行ったら。
				m_astar.Execute(m_position, m_beforeAstar);
				ChangeState(&m_vigilanceCancel);
				AstarEXEcount = 0;
			}
		}
	}
	if (m_currentstate == &m_vigilanceCancel) {
		m_moveSpeed = m_astar.GetAStarAnswerPos() - m_position;
		if ((m_astar.GetAStarAnswerPos() - m_position).Length() < 50.0f)
		{
			m_astar.AdvanceIt();
			if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd()) {
				ChangeState(&m_hesitate);
			}
		}
	}

	if (g_pad[0].IsTrigger(enButtonA)) {
		ChangeState(&m_vigilance);
	}

	////視野角の計算。
	//CVector3 toPlayer = m_player->GetPosition() - m_position;
	//float toPlayerLen = toPlayer.Length();
	//toPlayer.Normalize();
	//if (m_moveSpeed.Length() > 0.01) {
	//	m_oldMoveSpeed = m_moveSpeed;
	//}
	//m_oldMoveSpeed.Normalize();
	//float angle = toPlayer.Dot(m_oldMoveSpeed);
	//angle = acos(angle);
	//
	////徘徊中もしくは警戒態勢なら。
	//if (m_currentstate == &m_hesitate || m_currentstate == &m_vigilance) {
	//	float a = fabsf(angle);
	//	
	//		CMath::DegToRad(15.0f);
	//		float ab = CMath::RadToDeg(a);
	//	//視野角に入ったときの処理。
	//	if (ab < 15.0f && toPlayerLen < 500.0f) {
	//		if (m_currentstate != &m_vigilance) {
	//			//警戒体制に移行。
	//			ChangeState(&m_vigilance);
	//			m_astar.Execute(m_position, m_player->GetPosition());
	//		}
	//		//何かを見つけた。(まだ確定ではない。
	//		m_discovery = true;
	//	}
		////視野角から出た。
		//else if (ab > 15.0f)
		//{
		//	m_discovery = false;
		//	if (m_currentstate != &m_hesitate) {
		//		ChangeState(&m_hesitate);
		//	}
		//}
		//if (m_discovery) {
		//	m_timer += GameTime().GetFrameDeltaTime();
		//}

		////視野角内にいる状態で3秒たったら敵発見確定。戦闘態勢をとる。
		//if (m_timer >= 3.0f) {
		//	if (m_currentstate != &m_battlePosture) {
		//		//戦闘態勢に移行。
		//		ChangeState(&m_battlePosture);
		//	}
		//	m_timer = 0.0f;
		//}
	//}
	/*if (toPlayerLen > 500.0f) {
		if (m_currentstate != &m_hesitate) {
			ChangeState(&m_hesitate);
			m_discovery = false;
			m_timer = 0.0f;
		}
	}
	*/

	Rotation();
	m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void Enemy::OnDestroy()
{
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
	m_moveSpeed += m_moveSpeed * 250.0f;
	if (len < 10.0f) {
		m_currentPath = PathList[m_currentPath].next;
	}
}

void Enemy::BattleMove()
{
	m_moveSpeed = m_player->GetPosition() - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed +=  m_moveSpeed * 900.0f;
}

void Enemy::Rotation()
{
	CVector3 moveSpeedXZ = m_moveSpeed;
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();

	m_Enxz = moveSpeedXZ;
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 180.0f);
	qRot.Multiply(m_Enxz);

	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}

void Enemy::ChangeState(IEnemyState * nextState)
{
	if (m_currentstate != nullptr)
	{
		m_currentstate->Leave();
	}
	m_currentstate = nextState;
	m_currentstate->Enter();
}
