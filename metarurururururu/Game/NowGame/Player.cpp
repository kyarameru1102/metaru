#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "FPSCamera.h"
#include "GameOver.h"

Player::Player()
{
	m_ui = NewGO<UI>(0, "ui");
	m_shotSE.Init(L"Assets/sound/gunfire.wav");
	m_shotSE2.Init(L"Assets/sound/gunfire.wav");
	m_shotSE3.Init(L"Assets/sound/gunfire.wav");
	m_shotSE4.Init(L"Assets/sound/gunfire.wav");
	m_shotSE5.Init(L"Assets/sound/gunfire.wav");
	m_shotSE6.Init(L"Assets/sound/gunfire.wav");
	m_shotSE7.Init(L"Assets/sound/gunfire.wav");
	m_shotSE8.Init(L"Assets/sound/gunfire.wav");
}


Player::~Player()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_ui);
}

bool Player::Start()
{
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/heisi_idle_L.tka");
	m_animClips[enAnimationClip_run].Load(L"Assets/animData/heisi_run.tka");
	m_animClips[enAnimationClip_shot].Load(L"Assets/animData/heisi_shot.tka");
	m_animClips[enAnimationClip_walk].Load(L"Assets/animData/heisi_walk.tka");
	m_animClips[enAnimationClip_hold].Load(L"Assets/animData/heisi_hold.tka");
	m_animClips[enAnimationClip_shotend].Load(L"Assets/animData/heisi_shotend.tka");
	m_animClips[enAnimationClip_reload].Load(L"Assets/animData/heisi_reload.tka");
	m_animClips[enAnimationClip_death].Load(L"Assets/animData/heisi_death.tka");
	
	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);
	m_animClips[enAnimationClip_shot].SetLoopFlag(true);
	m_animClips[enAnimationClip_walk].SetLoopFlag(true);
	m_animClips[enAnimationClip_hold].SetLoopFlag(true);

	m_charaCon.Init(
		15.0f,
		70.0f,
		m_position
	);
	ChangeState(&m_idleState);

	//cmoファイルの読み込み。
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo",m_animClips,enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->SetShadowReciever(true);
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
	if (!m_clear) {

		if (m_position.y <= -10.0f) {
			m_hp = 0;
		}

		Damage();

		if (!m_death) {
			m_currentstate->Update();

			//銃を構えているなら。
			if (m_currentstate == &m_holdGunState || m_currentstate == &m_reloadState)
			{
				HoldMove();
				if (m_currentstate == &m_holdGunState) {
					HoldRotation();
				}
			}
			//銃を構えていなくて動けるなら。
			else if (m_currentstate->IsPossibleMove())
			{
				if (m_currentstate != &m_holdGunState) {
					Move();
				}
			}
			//リロード処理。
			if (m_currentstate == &m_reloadState) {
				Reload();
			}
			else if (!g_pad[0].IsPressAnyKey())
			{
				ChangeState(&m_idleState);
			}
			//銃が撃てるステートなら。
			if (m_currentstate->IsPossibleGunShoot()) {
				//パッドのL1が押されていたら。
				if (g_pad[0].IsPress(enButtonLB1))
				{
					//ステートを切り替える。
					ChangeState(&m_holdGunState);
					if (!m_Firing) {
						m_skinModelRender->PlayAnimation(enAnimationClip_hold, true, 0.3);
					}
				}
				else {
					//構えていなくて銃を撃っていなくて走っていなければ。
					if (!m_dash && !m_Firing) {
						if (!m_fps) {
							m_skinModelRender->PlayAnimation(enAnimationClip_idle, true, 0.3);
						}
					}
				}
				//パッドのR2が押されていたら。
				if (g_pad[0].IsPress(enButtonRB2))
				{
					//撃つ。
					Firing();
				}
				else {
					//撃っていない。
					m_Firing = false;
					m_shotTimer = 0;
					m_shotTimerOn = false;
					m_dangan = false;
				}
				if (m_shotTimerOn) {
					m_shotTimer--;
				}
				if (g_pad[0].IsPress(enButtonX) && !m_dangan) {
					ChangeState(&m_reloadState);
				}
			}

			if (g_pad[0].IsPress(enButtonLB2)) {
				if (m_currentstate != &m_reloadState) {
					if (!m_Firing) {
						m_skinModelRender->PlayAnimation(enAnimationClip_hold, true, 0.3);
					}
					m_skinModelRender->SetRenderOn(false);
					CameraSwitchFPS();
				}
				else {
					m_skinModelRender->SetRenderOn(true);
					CameraSwitchTPS();
				}
			}
			else {
				m_skinModelRender->SetRenderOn(true);
				CameraSwitchTPS();
			}

			if (m_currentstate->IsRotateByMove()) {
				Rotation();
			}
			MoveAnimation();



			//重力。
			m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
			m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
		}
	}
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void Player::Move()
{
	if (m_currentstate != &m_reloadState) {
		if (m_currentstate != &m_moveState)
		{
			ChangeState(&m_moveState);
		}
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
	if(toNextLength.Length() >= 6.0f)
	{
		if (m_currentstate != &m_reloadState) {
			m_skinModelRender->PlayAnimation(enAnimationClip_run, true, 0.5);
		}
		m_dash = true;
		m_skinModelRender->PlayAnimation(enAnimationClip_run,false,0.5);
	}
	else if (toNextLength.Length() >= 0.1f) {
		m_dash = false;
		m_skinModelRender->PlayAnimation(enAnimationClip_walk,false, 0.5);
	}
	else {
		if (m_currentstate == &m_holdGunState)
		{
			m_skinModelRender->PlayAnimation(enAnimationClip_hold, false, 0.3);
		}
		else {
			m_skinModelRender->PlayAnimation(enAnimationClip_idle, false, 0.3);
		}
		m_dash = false;
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

	m_moveSpeed += cameraRight * Lstick.x * 100;
	m_moveSpeed += cameraFront * Lstick.y * 100;
}

void Player::Rotation()
{
	CVector3 moveSpeedXZ = m_moveSpeed;
	moveSpeedXZ.y = 0.0f;
	moveSpeedXZ.Normalize();
	if (moveSpeedXZ.LengthSq() < 0.5f) {
		return;
	}

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
//銃を撃つ。
void Player::Firing()
{
	m_dangan = false;
	//残弾があれば。
	if (m_ammo >= 1) {
		//ステートを切り替える。
		ChangeState(&m_holdGunState);
		//撃つ。
		if (m_shotTimer == 0) {
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
			if (m_shotSE.IsPlaying() == false) {
				m_shotSE.Play(false);
			}
			else if(m_shotSE2.IsPlaying() == false){
				m_shotSE2.Play(false);
			}
			else if (m_shotSE3.IsPlaying() == false) {
				m_shotSE3.Play(false);
			}
			else if (m_shotSE4.IsPlaying() == false) {
				m_shotSE4.Play(false);
			}
			else if (m_shotSE5.IsPlaying() == false) {
				m_shotSE5.Play(false);
			}
			else if (m_shotSE6.IsPlaying() == false) {
				m_shotSE6.Play(false);
			}
			else if (m_shotSE7.IsPlaying() == false) {
				m_shotSE7.Play(false);
			}
			else if (m_shotSE8.IsPlaying() == false) {
				m_shotSE8.Play(false);
			}
			m_shotTimer = 5;
			m_ammo--;
			m_dangan = true;
		}
		m_Firing = true;
		m_skinModelRender->PlayAnimation(enAnimationClip_shot, true, 0.1);
		m_shotTimerOn = true;
	}
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
		CVector3 direction;
		direction = m_fpsCamera->Getdirection();
		DeleteGO(m_fpsCamera);
		m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
		m_gameCamera->Setdirection(direction * -1);
		m_fps = false;
		if (m_currentstate != &m_reloadState) {
			if (m_currentstate != &m_idleState) {
				ChangeState(&m_idleState);
			}
		}
	}
}

void Player::Damage()
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
		m_skinModelRender->PlayAnimation(enAnimationClip_death, true, 0.3f);
		m_skinModelRender->PlayAnimation(enAnimationClip_death, false, 0.3f);
		m_death = true;
	}
}

void Player::Reload()
{
	m_skinModelRender->PlayAnimation(enAnimationClip_reload, true, 0.3);
	m_reloadTimer--;
	if (m_reloadTimer <= 0) {
		m_ammo = 30;
		ChangeState(&m_idleState);
		m_reloadTimer = 130;
	}
}
