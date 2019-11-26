#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GameTime.h"
#include "Astar.h"

EnemyStateBattlePosture		EnemyState::m_battlePosture;
EnemyStateHesitate			EnemyState::m_hesitate;
EnemyStateVigilance			EnemyState::m_vigilance;
EnemyStateVigilanceCancel	EnemyState::m_vigilanceCancel;

Enemy::Enemy()
{
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
		20.0f,
		70.0f,
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
	m_moveSpeed = CVector3::Zero();
	//ˆÚ“®Œnˆ—B
	{
		//œpœj’†B
		if (m_currentstate == &EnemyState::m_hesitate) {
			PathMove();
		}
		//Œx‰ú‘Ô¨‚É“ü‚Á‚½‚çB
		if (m_currentstate == &EnemyState::m_vigilance) {
			VigilanceMove();
		}
		//Œx‰ú‘Ô¨‚ª‰ğœ‚³‚ê‚½‚çB
		if (m_currentstate == &EnemyState::m_vigilanceCancel) {
			VigilanceCancelMove();
		}
	}

	if (g_pad[0].IsTrigger(enButtonA)) {
		ChangeState(&EnemyState::m_vigilance);
	}

	//‹–ìŠp‚ÌŒvZB
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

	//œpœj’†‚à‚µ‚­‚ÍŒx‰ú‘Ô¨‚ª‰ğœ’†‚È‚çB
	if (m_currentstate == &EnemyState::m_hesitate || m_currentstate == &EnemyState::m_vigilanceCancel) {
		
		//‹–ìŠp‚É“ü‚Á‚½‚Æ‚«‚Ìˆ—B
		if (ab < 15.0f && toPlayerLen < 1500.0f) {
			if (m_currentstate != &EnemyState::m_vigilance) {
				//Œx‰ú‘Ì§‚ÉˆÚsB
				ChangeState(&EnemyState::m_vigilance);
			}
		}
	}
	//Œx‰ú‘Ô¨’†‚È‚çB
	if (m_currentstate == &EnemyState::m_vigilance) {
		//‹–ìŠp‚É“ü‚Á‚½‚Æ‚«‚Ìˆ—B
		if (ab < 45.0f && toPlayerLen < 500.0f) {
			if (m_currentstate != &EnemyState::m_battlePosture) {
				//Œx‰ú‘Ì§‚ÉˆÚsB
				ChangeState(&EnemyState::m_battlePosture);
			}
		}
	}
	if (m_currentstate == &EnemyState::m_battlePosture) {
		CVector3 EnemyBulletDrc;
		EnemyBulletDrc = m_player->GetPosition() - m_position;
		EnemyBulletDrc.Normalize();
		EnemyBulletDrc *= 100.0f;
		Bullet* bullet = nullptr;
		bullet = NewGO<Bullet>(0, "bullet");
		bullet->SetPosition(m_position);
		bullet->SetmoveSpeed(EnemyBulletDrc);
		bullet->SetEnemy();
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
		m_beforeAstar = m_position;
	}
	AstarEXEcount++;
	//A*Œo˜H’T¸‚Åo‚½Œ‹‰Ê‚ÅƒpƒXˆÚ“®B
	m_moveSpeed = m_astar.GetAStarAnswerPos() - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 100.0f;
	MoveAnimation();
	if ((m_astar.GetAStarAnswerPos() - m_position).Length() < 50.0f)
	{
		m_astar.AdvanceIt();
		if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd()) {
			//ƒpƒX‚ÌÅŒã‚Ü‚Ås‚Á‚½‚çB
			m_astar.Execute(m_position, m_beforeAstar);
			ChangeState(&EnemyState::m_vigilanceCancel);
			AstarEXEcount = 0;
		}
	}
}

void Enemy::VigilanceCancelMove()
{
	m_moveSpeed = m_astar.GetAStarAnswerPos() - m_position;
	m_moveSpeed.Normalize();
	m_moveSpeed += m_moveSpeed * 100.0f;
	MoveAnimation();
	if ((m_astar.GetAStarAnswerPos() - m_position).Length() < 50.0f)
	{
		m_astar.AdvanceIt();
		if (m_astar.GetAStarAnswerIt() == m_astar.GetAStarAnswerEnd()) {
			ChangeState(&EnemyState::m_hesitate);
		}
	}
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

	m_Enxz = moveSpeedXZ;
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 180.0f);
	qRot.Multiply(m_Enxz);

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
