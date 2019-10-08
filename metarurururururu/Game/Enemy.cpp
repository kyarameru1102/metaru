#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GameTime.h"

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
	m_skinModelRender->Init(L"Assets/modelData/takatozin.cmo");
	m_currentPath = 0;
	m_position = PathList[0].position;
	return true;
}

void Enemy::Update()
{
	m_moveSpeed = CVector3::Zero();
	//�p�j���B
	if (m_currentstate == &m_hesitate) {
		PathMove();
	}
	//�퓬�Ԑ����̍s���B
	if (m_currentstate == &m_battlePosture) {
		BattleMove();
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
	
	//�p�j���������͌x���Ԑ��Ȃ�B
	if (m_currentstate == &m_hesitate || m_currentstate == &m_vigilance) {
		float a = fabsf(angle);
		
			CMath::DegToRad(15.0f);
			float ab = CMath::RadToDeg(a);
		//����p�ɓ������Ƃ��̏����B
		if (ab < 15.0f && toPlayerLen < 500.0f) {
			if (m_currentstate != &m_vigilance) {
				//�x���̐��Ɉڍs�B
				ChangeState(&m_vigilance);
			}
			//�������������B(�܂��m��ł͂Ȃ��B
			m_discovery = true;
		}
		//����p����o���B
		else if (ab > 15.0f)
		{
			m_discovery = false;
			if (m_currentstate != &m_hesitate) {
				ChangeState(&m_hesitate);
			}
		}
		if (m_discovery) {
			m_timer += GameTime().GetFrameDeltaTime();
		}

		//����p���ɂ����Ԃ�3�b��������G�����m��B�퓬�Ԑ����Ƃ�B
		if (m_timer >= 3.0f) {
			if (m_currentstate != &m_battlePosture) {
				//�퓬�Ԑ��Ɉڍs�B
				ChangeState(&m_battlePosture);
			}
			m_timer = 0.0f;
		}
	}
	if (toPlayerLen > 500.0f) {
		if (m_currentstate != &m_hesitate) {
			ChangeState(&m_hesitate);
			m_discovery = false;
			m_timer = 0.0f;
		}
	}
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
	CVector3 moveSpeedXZ = m_oldMoveSpeed;
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
