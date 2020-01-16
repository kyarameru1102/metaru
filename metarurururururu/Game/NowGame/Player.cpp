#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "FPSCamera.h"
#include "GameOver.h"

Player::Player()
{
	m_ui = NewGO<UI>(0, "ui");
}


Player::~Player()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_ui);
}

bool Player::Start()
{
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/heisi_idle.tka");
	m_animClips[enAnimationClip_run].Load(L"Assets/animData/heisi_run.tka");
	m_animClips[enAnimationClip_shot].Load(L"Assets/animData/heisi_shot.tka");

	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);

	m_charaCon.Init(
		15.0f,
		70.0f,
		m_position
	);
	ChangeState(&m_idleState);

	//cmoファイルの読み込み。
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo",m_animClips,enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle);
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_fpsCamera = FindGO<FPSCamera>("fpsCamera");
	return true;
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
	CVector3 PlayerCenter = m_position;
	PlayerCenter.y += 80;
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		if ((bullet->GetPosition() - PlayerCenter).Length() <= 50.0f)
		{
			int aaaa = 0;
			//敵兵の弾なら。
			if (!bullet->GetWhosebullet())
			{
				m_hp--;
			}
		}
		return true;
	});
	if (m_hp <= 0 && !m_death) {
		NewGO<GameOver>(0, "gameOver");
		m_death = true;
	}

	if(!m_death) {
		m_currentstate->Update();

		if (m_currentstate == &m_holdGunState)
		{
			HoldMove();
			HoldRotation();
		}
		else if (m_currentstate->IsPossibleMove())
		{
			if (m_currentstate != &m_holdGunState) {
				Move();
				MoveAnimation();
			}
		}
		if (m_currentstate->IsPossibleGunShoot()) {
			if (g_pad[0].IsPress(enButtonRB2))
			{
				ChangeState(&m_holdGunState);
				Firing();
			}
			if (g_pad[0].IsPress(enButtonLB1))
			{
				ChangeState(&m_holdGunState);
				m_skinModelRender->PlayAnimation(enAnimationClip_shot);
			}
		}
		m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();

		m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);

		if (g_pad[0].IsPress(enButtonLB2)) {
			m_skinModelRender->SetRenderOn(false);
			CameraSwitchFPS();
		}
		else {
			m_skinModelRender->SetRenderOn(true);
			CameraSwitchTPS();
		}
		if (!g_pad[0].IsPressAnyKey())
		{
			ChangeState(&m_idleState);
		}
		if (m_currentstate->IsRotateByMove()) {
			Rotation();
		}
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

	m_moveSpeed += cameraRight * Lstick.x * 300;
	m_moveSpeed += cameraFront * Lstick.y * 300;
}

void Player::MoveAnimation()
{
	CVector3 toNextLength;
	CVector3 nextPos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	toNextLength = nextPos - m_position;
	if(toNextLength.Length() >= 0.1f)
	{
		m_skinModelRender->PlayAnimation(enAnimationClip_run,0.5);
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
	/*m_Plxz = moveSpeedXZ;
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 180.0f);
	qRot.Multiply(m_Plxz);*/

	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}

void Player::HoldRotation()
{
	
	CVector3 moveSpeedXZ = CVector3::One();
	if (m_fps == true) {
		m_fpsCamera = FindGO<FPSCamera>("fpsCamera");
		moveSpeedXZ = m_fpsCamera->Getdirection();
	}
	else {
		m_gameCamera = FindGO<GameCamera>("gameCamera");
		moveSpeedXZ = m_gameCamera->Getdirection();
		moveSpeedXZ *= -1.0f;
	}
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();
	if (moveSpeedXZ.LengthSq() < 0.5f) {
		return;
	}
	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(moveSpeedXZ.x, moveSpeedXZ.z));
}
void Player::Firing()
{
	Bullet* bullet = nullptr;
	bullet = NewGO<Bullet>(0, "bullet");
	CVector3 BulletDrc;
	if (m_fps) {
		BulletDrc = m_fpsCamera->Getdirection();
	}
	else if (!m_fps) {
		BulletDrc = m_gameCamera->Getdirection() * -1;
	}
	BulletDrc.Normalize();
	BulletDrc *= 100;
	bullet->SetPosition(m_position);
	bullet->SetmoveSpeed(BulletDrc);
	bullet->SetPlayer();
	m_skinModelRender->PlayAnimation(enAnimationClip_shot);
}
//FPSカメラに切り替える関数。
void Player::CameraSwitchFPS()
{
	if (m_fps == false)
	{
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
		//m_skinModelRender->SetRenderOn(true);
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


