#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "FPSCamera.h"

Player::Player()
{
}


Player::~Player()
{
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/idle.tka");
	m_animClips[enAnimationClip_walk].Load(L"Assets/animData/walk.tka");
	m_animClips[enAnimationClip_run].Load(L"Assets/animData/run.tka");

	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_walk].SetLoopFlag(true);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);

	m_charaCon.Init(
		20.0f,
		70.0f,
		m_position
	);
	ChangeState(&m_idleState);
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	//cmoファイルの読み込み。
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/unityChan.cmo",m_animClips,enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisY);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle);

	m_fpsCamera = FindGO<FPSCamera>("fpsCamera");
	return true;
}

void Player::OnDestroy()
{
}

void Player::ChangeState(IPlayerState* nextState)
{
	if (m_currentstate != nullptr)
	{
		m_currentstate->Leave();
	}
	m_currentstate = nextState;
	m_currentstate->Enter();
}

void Player::Update()
{
	m_currentstate->Update();
	
	if (m_currentstate == &m_holdGunState)
	{
		HoldMove();
		HoldRotation();
	}
	else if (m_currentstate->IsPossibleMove()) 
	{
		Move();
		MoveAnimation();
	}
	if (m_currentstate->IsPossibleGunShoot()) {
		if (g_pad[0].IsPress(enButtonRB1))
		{
			/*auto mRot = CMatrix::Identity;
			mRot.MakeRotationFromQuaternion(m_rotation);
			m_forward.x = mRot.m[2][0];
			m_forward.y = mRot.m[2][1];
			m_forward.z = mRot.m[2][2];*/
			Firing();
		}
	}
	m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();

	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	
	if (g_pad[0].IsPress(enButtonLB1)) {
		CameraSwitchFPS();
	}
	else {
		CameraSwitchTPS();
	}
	if (!g_pad[0].IsPressAnyKey())
	{
		ChangeState(&m_idleState);
	}
	if (m_currentstate->IsRotateByMove()) {
		Rotation();
	}
	
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}


void Player::Move()
{
	if (m_currentstate != &m_moveState)
	{
		ChangeState(&m_moveState);
	}
	CVector3 Lstick;
	Lstick.x = g_pad[0].GetLStickXF();
	Lstick.y = g_pad[0].GetLStickYF();
	
	CVector3 cameraFront = g_camera3D.GetFront();
	CVector3 cameraRight = g_camera3D.GetRight();

	cameraFront.y = 0.0f;
	cameraRight.y = 0.0f;
	cameraFront.Normalize();
	cameraRight.Normalize();

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	m_moveSpeed += cameraRight * Lstick.x * 500;
	m_moveSpeed += cameraFront * Lstick.y * 500;
}

void Player::MoveAnimation()
{
	CVector3 toNextLength;
	CVector3 nextPos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	toNextLength = nextPos - m_position;
	float aaa = toNextLength.Length();
	if (toNextLength.Length() >= 10.0f)
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_run,0.5);
	}
	else if(toNextLength.Length() >= 0.1f)
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_walk,0.5);
	}
	else {
		m_skinModelRender->PlayAnimation(enAnimationClip_idle,0.3);
	}
}

void Player::HoldMove()
{
	CVector3 Lstick;
	Lstick.x = g_pad[0].GetLStickXF();
	Lstick.y = g_pad[0].GetLStickYF();

	CVector3 cameraFront = g_camera3D.GetFront();
	CVector3 cameraRight = g_camera3D.GetRight();

	cameraFront.y = 0.0f;
	cameraRight.y = 0.0f;
	cameraFront.Normalize();
	cameraRight.Normalize();

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	m_moveSpeed += cameraRight * Lstick.x * 200;
	m_moveSpeed += cameraFront * Lstick.y * 200;
}

void Player::Rotation()
{
	CVector3 moveSpeedXZ = m_moveSpeed;
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();
	if (moveSpeedXZ.LengthSq() < 0.5f) {
		return;
	}
	m_Plxz = moveSpeedXZ;
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 180.0f);
	qRot.Multiply(m_Plxz);

	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}

void Player::HoldRotation()
{
	
	CVector3 moveSpeedXZ = CVector3::One();
	if (m_fpsCamera != NULL) {
		moveSpeedXZ = m_fpsCamera->Getdirection();
	}
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();
	if (moveSpeedXZ.LengthSq() < 0.5f) {
		return;
	}
	m_Plxz = moveSpeedXZ;
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 180.0f);
	qRot.Multiply(m_Plxz);

	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}
void Player::Firing()
{
	m_bullet = NewGO<Bullet>(0, "bullet");
	CVector3 Bulletpos = m_position;
	Bulletpos.y += 50.0f;
	m_bullet->SetPosition(Bulletpos);
	CVector3 BulletDrc;
	if (m_fps) {
		BulletDrc = m_fpsCamera->Getdirection();
	}
	else if (!m_fps) {
		BulletDrc = m_gameCamera->Getdirection() * -1;
	}
	BulletDrc.Normalize();
	BulletDrc *= 500;
	m_bullet->SetmoveSpeed(BulletDrc);
}
//FPSカメラに切り替える関数。
void Player::CameraSwitchFPS()
{
	if (m_fps == false)
	{
		DeleteGO(m_skinModelRender);
		CVector3 direction;
		direction = m_gameCamera->Getdirection();
		DeleteGO(m_gameCamera);
		m_fpsCamera = NewGO<FPSCamera>(0, "fpsCamera");
		m_fpsCamera->Setdirection(direction * -1);
		m_fps = true;
		if (m_currentstate != &m_holdGunState) {
			ChangeState(&m_holdGunState);
		}
		
	}
	
}
//TPSカメラに切り替える。
void Player::CameraSwitchTPS()
{
	if (m_fps == true)
	{
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init(L"Assets/modelData/unityChan.cmo", m_animClips, enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisY);
		CVector3 direction;
		direction = m_fpsCamera->Getdirection();
		DeleteGO(m_fpsCamera);
		m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
		m_gameCamera->Setdirection(direction * -1);
		m_fps = false;
		if (m_currentstate != &m_idleState) {
			ChangeState(&m_idleState);
		}
	}
}


