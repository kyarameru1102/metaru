#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "FPSCamera.h"
#include "GameOver.h"
#include "C4.h"

struct FootStepCallBack : public btCollisionWorld::ClosestConvexResultCallback
{
	bool hit = false;
	FootStepCallBack() :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f))
	{}
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		hit = true;
		return 0.0;
	}
};

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
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/heisi_idle_L.tka");
	m_animClips[enAnimationClip_run].Load(L"Assets/animData/heisi_run.tka");
	m_animClips[enAnimationClip_shot].Load(L"Assets/animData/heisi_shot.tka");
	m_animClips[enAnimationClip_walk].Load(L"Assets/animData/heisi_walk.tka");
	m_animClips[enAnimationClip_hold].Load(L"Assets/animData/heisi_hold.tka");
	m_animClips[enAnimationClip_shotend].Load(L"Assets/animData/heisi_shotend.tka");
	m_animClips[enAnimationClip_reload].Load(L"Assets/animData/heisi_reload.tka");
	m_animClips[enAnimationClip_death].Load(L"Assets/animData/heisi_death.tka");
	m_animClips[enAnimationClip_creep_idle].Load(L"Assets/animData/heisi_huse_idle.tka");
	m_animClips[enAnimationClip_creep_forward].Load(L"Assets/animData/heisi_creepingForward.tka");
	
	m_animClips[enAnimationClip_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);
	m_animClips[enAnimationClip_shot].SetLoopFlag(true);
	m_animClips[enAnimationClip_walk].SetLoopFlag(true);
	m_animClips[enAnimationClip_hold].SetLoopFlag(true);
	m_animClips[enAnimationClip_creep_idle].SetLoopFlag(true);
	m_animClips[enAnimationClip_creep_forward].SetLoopFlag(true);
	


	m_charaCon.Init(
		15.0f,
		70.0f,
		m_position
	);
	ChangeState(&m_idleState);

	//cmo�t�@�C���̓ǂݍ��݁B
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo",m_animClips,enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->SetShadowReciever(true);
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_fpsCamera = FindGO<FPSCamera>("fpsCamera");

	m_maxHP = m_hp;
	m_maxAmmo = m_ammo;

	//�A�j���[�V�����C�x���g��ݒ肷��B
	Animation& anim = m_skinModelRender->GetAnimation();
	anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) 
		{
			OnAnimationEvent();
		}
	);

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
	//@todo for debug
#if 1
	if (!m_clear) {
		//�}�b�v���痎���Ă��܂����Ƃ��̏����B
		if (m_position.y <= -10.0f) {
			m_hp = 0;
		}
		//�_���[�W�����B
		Damage();

		if (!m_death) {
			m_currentstate->Update();

			//C4�ݒu�����B
			InstallationC4();

			//�����Ă��邩�ǂ����̃t���O�����B
			if (m_currentstate != &m_creepState && m_currentstate != &m_creepMoveState) {
				m_creep = false;
			}

			//���R�񕜁B
			NaturalRecovery();
			
			//�e���\���Ă���Ȃ�B
			if (m_currentstate == &m_holdGunState || m_currentstate == &m_reloadState)
			{
				HoldMove();
				if (m_currentstate == &m_holdGunState) {
					HoldRotation();
				}
			}
			//�e���\���Ă��Ȃ��ē�����Ȃ�B
			else if (m_currentstate->IsPossibleMove())
			{
				if (m_currentstate == &m_creepState || m_currentstate == &m_creepMoveState) {
					m_creep = true;
					Creep();
					CreepMove();
				}
				else if (m_currentstate != &m_holdGunState) {
					Move();
				}
			}
			//�����ĂȂ��ĉ���������Ă��Ȃ��Ȃ�B
			else if (!g_pad[0].IsPressAnyKey() && m_currentstate != &m_creepState && m_currentstate != &m_creepMoveState)
			{
				ChangeState(&m_idleState);
			}
			//�����؂�ւ��B
			if (m_currentstate == &m_idleState || m_currentstate == &m_moveState)
			{
				//A�{�^���������ꂽ��B
				if (g_pad[0].IsTrigger(enButtonA))
				{
					//�����X�e�[�g�ɐ؂�ւ���B
					ChangeState(&m_creepState);
				}
			}
			//�����X�e�[�g�Ȃ�B
			else if (m_currentstate == &m_creepState) {
				
				if (g_pad[0].IsTrigger(enButtonA)) {
					ChangeState(&m_idleState);
				}
			}
			//�����[�h�����B
			if (m_currentstate == &m_reloadState) {
				Reload();
			}
			//�e�����Ă�X�e�[�g�Ȃ�B
			if (m_currentstate->IsPossibleGunShoot()) {
				//�p�b�h��L1��������Ă�����B
				if (g_pad[0].IsPress(enButtonLB1))
				{
					//�X�e�[�g��؂�ւ���B
					ChangeState(&m_holdGunState);
					if (!m_Firing) {
						m_skinModelRender->PlayAnimation(enAnimationClip_hold, true, 0.3);
					}
				}
				else {
					//�\���Ă��Ȃ��ďe�������Ă��Ȃ��đ����Ă��Ȃ���΁B
					if (!m_dash && !m_Firing) {
						if (!m_fps) {
							m_skinModelRender->PlayAnimation(enAnimationClip_idle, true, 0.3);
						}
					}
				}
				//�p�b�h��R2��������Ă�����B
				if (g_pad[0].IsPress(enButtonRB2))
				{
					//���B
					Firing();
				}
				else {
					//�����Ă��Ȃ��B
					m_Firing = false;
					m_shotTimer = 0;
					m_shotTimerOn = false;
					m_dangan = false;
					if (!g_pad[0].IsPress(enButtonLB2) && !g_pad[0].IsPress(enButtonLB1)) {
						ChangeState(&m_idleState);
					}
				}
				if (m_shotTimerOn) {
					m_shotTimer--;
				}
				//�����[�h�X�e�[�g�؂�ւ��B
				if (g_pad[0].IsPress(enButtonX) && !m_dangan && m_ammo < m_maxAmmo) {
					ChangeState(&m_reloadState);
				}

				//L2�{�^���������ꂽ�Ƃ��̏����B
				if (g_pad[0].IsPress(enButtonLB2)) {
					//�����[�h�X�e�[�g�ȊO�Ȃ�΁B
					if (m_currentstate != &m_reloadState) {
						//�e���\����B
						if (!m_Firing) {
							m_skinModelRender->PlayAnimation(enAnimationClip_hold, true, 0.3);
						}
						m_skinModelRender->SetRenderOn(false);
						//��l�̎��_�ɐ؂�ւ���B
						CameraSwitchFPS();
					}
					else {
						m_skinModelRender->SetRenderOn(true);
						//�O�l�̎��_�ɐ؂�ւ���B
						CameraSwitchTPS();
					}
				}
				else {
					m_skinModelRender->SetRenderOn(true);
					CameraSwitchTPS();
				}
			}
			if (m_currentstate->IsRotateByMove()) {
				Rotation();
			}
			if (m_currentstate != &m_creepState && m_currentstate != &m_creepMoveState) {
				//�ړ��A�j���[�V�����B
				MoveAnimation();
			}
			//�d�́B
			m_moveSpeed.y -= 980.0f * GameTime().GetFrameDeltaTime();
			//�L�����N�^�[�R���g���[���[�����Ƃɍ��W��ݒ�B
			m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
		}
	}
#endif
	//�X�L�����f���ɍ��W�Ɖ�]��ݒ�B
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void Player::Move()
{
	//�����[�h�X�e�[�g�ȊO�ŁB
	if (m_currentstate != &m_reloadState) 
	{
		//�ړ��X�e�[�g�łȂ���΁B
		if (m_currentstate != &m_moveState)
		{
			//�ړ��X�e�[�g�ɐ؂�ւ���B
			ChangeState(&m_moveState);
		}
	}
	//���X�e�B�b�N�̓��͗ʂ��擾�B
	CVector3 Lstick;
	Lstick.x = g_pad[0].GetLStickXF();
	Lstick.y = g_pad[0].GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	CVector3 cameraFront = g_camera3D.GetFront();
	CVector3 cameraRight = g_camera3D.GetRight();

	cameraFront.y = 0.0f;
	cameraRight.y = 0.0f;
	cameraFront.Normalize();
	cameraRight.Normalize();

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�ړ����x��ݒ�B
	m_moveSpeed += cameraRight * Lstick.x * 200;
	m_moveSpeed += cameraFront * Lstick.y * 200;
}

//��{�I�ɉ����g�ɓK�p����A�j���[�V������ݒ肷��֐��B
void Player::MoveAnimation()
{
	CVector3 toNextLength;
	//���̍��W�B
	CVector3 nextPos = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//���̍��W�܂ł̋����B
	toNextLength = nextPos - m_position;
	if(toNextLength.Length() >= 6.0f)
	{
		if (m_currentstate != &m_reloadState) {
			//�㔼�g�ɑ���A�j���[�V�����K�p�B
			m_skinModelRender->PlayAnimation(enAnimationClip_run, true, 0.5f);
		}
		m_dash = true;
		//�����g�ɑ���A�j���[�V�����K�p�B
		m_skinModelRender->PlayAnimation(enAnimationClip_run,false,0.5f);
	}
	else if (toNextLength.Length() >= 1.1f) {
		m_dash = false;
		//�����g�ɕ����A�j���[�V�����K�p�B
		m_skinModelRender->PlayAnimation(enAnimationClip_walk,false, 0.5f);
	}
	else {
		if (m_currentstate == &m_holdGunState)
		{
			//�����g�ɏe���\����A�j���[�V�����K�p�B
			m_skinModelRender->PlayAnimation(enAnimationClip_hold, false, 0.3f);
		}
		else {
			//�����g�ɑҋ@�A�j���[�V�����K�p�B
			m_skinModelRender->PlayAnimation(enAnimationClip_idle, false, 0.3f);
		}
		m_dash = false;
	}
}

void Player::HoldMove()
{
	//���X�e�B�b�N�̓��͗ʂ��擾�B
	CVector3 Lstick;
	Lstick.x = g_pad[0].GetLStickXF();
	Lstick.y = g_pad[0].GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	CVector3 cameraFront = g_camera3D.GetFront();
	CVector3 cameraRight = g_camera3D.GetRight();

	cameraFront.y = 0.0f;
	cameraRight.y = 0.0f;
	cameraFront.Normalize();
	cameraRight.Normalize();

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�ړ����x��ݒ�B
	m_moveSpeed += cameraRight * Lstick.x * 100;
	m_moveSpeed += cameraFront * Lstick.y * 100;
}

void Player::Rotation()
{
	//�ړ�������������߂�B
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
	CVector3 moveSpeedXZ;
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
//�e�����B
void Player::Firing()
{
	m_dangan = false;
	//�c�e������΁B
	if (m_ammo >= 1) {
		//�X�e�[�g��؂�ւ���B
		ChangeState(&m_holdGunState);
		//���B
		if (m_shotTimer == 0) {
			//�e�ې����B
			Bullet* bullet = nullptr;
			bullet = NewGO<Bullet>(0, "bullet");
			//�e�ۂ̌����B
			CVector3 BulletDrc;
			if (m_fps) {
				BulletDrc = m_fpsCamera->Getdirection();
			}
			else if (!m_fps) {
				BulletDrc = m_gameCamera->Getdirection() * -1;
			}
			BulletDrc.Normalize();
			//���x���ݒ�B
			BulletDrc *= 100;
			//�����ʒu�ݒ�B
			bullet->SetPosition(m_position);
			//�ړ����x�y�шړ�������ݒ�B
			bullet->SetmoveSpeed(BulletDrc);
			//�v���C���[�̒e�ۂł��邱�Ƃ�ݒ�B
			bullet->SetPlayer();
			//�e��������SE�B
			CSoundSource* m_shotSE;
			m_shotSE = NewGO<CSoundSource>(0);
			m_shotSE->Init(L"Assets/sound/gunfire.wav");
			m_shotSE->Play(false);

			m_shotTimer = 5;
			m_ammo--;
			m_dangan = true;
		}
		m_Firing = true;
		//�㔼�g�ɏe�����A�j���[�V�����K�p�B
		m_skinModelRender->PlayAnimation(enAnimationClip_shot, true, 0.1f);
		m_shotTimerOn = true;
	}
	else {
		if (!m_dash) {
			m_skinModelRender->PlayAnimation(enAnimationClip_idle, true, 0.1f);
		}
	}
}
//FPS�J�����ɐ؂�ւ���֐��B
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
//TPS�J�����ɐ؂�ւ���B
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
			//�G���̒e�Ȃ�B
			if (!bullet->GetWhosebullet())
			{
				m_hp--;
				//DeleteGO(bullet);
			}
		}
		return true;
		});
	if (m_hp <= 0 && !m_death) {
		NewGO<GameOver>(0, "gameOver");
		//�㔼�g�Ɖ����g�Ɏ��S�A�j���[�V�����K�p�B
		m_skinModelRender->PlayAnimation(enAnimationClip_death, true, 0.3f);
		m_skinModelRender->PlayAnimation(enAnimationClip_death, false, 0.3f);
		m_death = true;
	}
}

void Player::Reload()
{
	//�㔼�g�Ƀ����[�h�A�j���[�V������K�p�B
	m_skinModelRender->PlayAnimation(enAnimationClip_reload, true, 0.3);
	m_reloadTimer--;
	if (m_reloadTimer <= 0) {
		//�c�e�����B
		m_ammo = m_maxAmmo;
		//�ҋ@�X�e�[�g�ɕύX�B
		ChangeState(&m_idleState);
		m_reloadTimer = 130;
	}
}

void Player::FootStep()
{
	//������SE�B
	CSoundSource* m_walkSE;
	m_walkSE = NewGO<CSoundSource>(0);
	m_walkSE->Init(L"Assets/sound/footstep.wav");
	m_walkSE->Play(false);
	
}

void Player::OnAnimationEvent()
{
	//���������B
	FootStep();
}

void Player::NaturalRecovery()
{
	//���R�񕜂ł���HP�B
	int recoveryHP = m_maxHP;
	if (m_hp < m_maxHP) {
		m_naturalRecoveryTrigger = true;
	}
	else {
		m_naturalRecoveryTrigger = false;
	}

	if (m_naturalRecoveryTrigger) {
		m_naturalRecoveryCount++;
	}
	else {
		m_naturalRecoveryCount = 0;
	}

	if (m_naturalRecoveryCount >= 300) {
		if (recoveryHP > m_hp) {
			m_hp++;
		}
	}
}

void Player::CreepMove()
{
	//���X�e�B�b�N�̓��͗ʂ��擾�B
	CVector3 Lstick;
	Lstick.x = g_pad[0].GetLStickXF();
	Lstick.y = g_pad[0].GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	CVector3 cameraFront = g_camera3D.GetFront();
	CVector3 cameraRight = g_camera3D.GetRight();

	cameraFront.y = 0.0f;
	cameraRight.y = 0.0f;
	cameraFront.Normalize();
	cameraRight.Normalize();

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�ړ����x��ݒ�B
	m_moveSpeed += cameraRight * Lstick.x * 80;
	m_moveSpeed += cameraFront * Lstick.y * 80;
}

void Player::Creep()
{
	if (!g_pad[0].GetLStickXF() && !g_pad[0].GetLStickYF()) {
		m_skinModelRender->PlayAnimation(enAnimationClip_creep_idle, true, 0.5f);
		m_skinModelRender->PlayAnimation(enAnimationClip_creep_idle, false, 0.5f);
	}
	else {
		m_skinModelRender->PlayAnimation(enAnimationClip_creep_forward, true, 0.3f);
		m_skinModelRender->PlayAnimation(enAnimationClip_creep_forward, false, 0.3f);
	}
}

void Player::InstallationC4()
{
	if (!m_creep) {
		if (g_pad[0].IsTrigger(enButtonB) && !g_pad[0].IsPress(enButtonLB1) && !g_pad[0].IsPress(enButtonLB2)) {
			C4* c4 = NewGO<C4>(0, "c4");
			c4->SetPosition(m_position);
		}
	}
}
