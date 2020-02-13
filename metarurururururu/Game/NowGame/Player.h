#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerReloadState.h"
#include "PlayerCreepState.h"
#include "PlayerCreepMoveState.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "UI.h"
#include "Human.h"
#include "physics/CapsuleCollider.h"

extern Pad g_pad[Pad::CONNECT_PAD_MAX];

class Bullet;
class GameCamera;
class FPSCamera;
class Player : public Human
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Player();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Player();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update();
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	/// <summary>
	/// �ړ����̃A�j���[�V���������B
	/// </summary>
	void MoveAnimation();
	/// <summary>
	/// �e���\���Ă��鎞�̈ړ������B
	/// </summary>
	void HoldMove();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �e���\���Ă���Ƃ��̉�]�����B
	/// </summary>
	void HoldRotation();
	/// <summary>
	/// �e���������B�e�ۂ𐶐����鏈���B
	/// </summary>
	void Firing();
	/// <summary>
	/// �J�����̐؂�ւ������BFPS�J�����ɐ؂�ւ���B
	/// </summary>
	void CameraSwitchFPS();
	/// <summary>
	/// �J�����̐؂�ւ������BTPS�J�����ɐ؂�ւ���B
	/// </summary>
	void CameraSwitchTPS();
	/// <summary>
	///�X�e�[�g��؂�ւ���֐��B 
	/// </summary>
	/// <param name="nextState">�؂�ւ������X�e�[�g�B</param>
	void ChangeState(IPlayerState* nextState);
	/// <summary>
	/// �_���[�W���󂯂鏈���B
	/// </summary>
	void Damage();
	/// <summary>
	/// �����[�h�����B
	/// </summary>
	void Reload();
	/// <summary>
	/// �����̏����B
	/// </summary>
	void FootStep();
	/// <summary>
	/// �A�j���[�V�����C�x���g�ŌĂԊ֐��B
	/// </summary>
	void OnAnimationEvent();
	/// <summary>
	/// ���R�񕜂̏����B
	/// </summary>
	void NaturalRecovery();
	/// <summary>
	/// �����Ă���Ƃ��̈ړ������B
	/// </summary>
	void CreepMove();
	/// <summary>
	/// �����Ă���Ƃ��̏����B
	/// </summary>
	void Creep();
	/// <summary>
	/// C4��ݒu����֐��B
	/// </summary>
	void InstallationC4();
	/// <summary>
	/// �v���C���[�̉�]��Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>rotation	��]�B</returns>
	CQuaternion GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// �N���A�������ǂ����̃t���O��ݒ肷��֐��B
	/// </summary>
	/// <param name="flag">
	/// true	�N���A���Ă���
	/// false	�N���A���Ă��Ȃ��B
	/// </param>
	void SetClearFlag(bool flag)
	{
		m_clear = flag;
	}
	/// <summary>
	/// ����ł��邩�ǂ����Ԃ��֐��B
	/// </summary>
	/// <returns>
	/// true	����ł���B
	/// false	����ł��Ȃ��B
	/// </returns>
	bool GetDeath() const
	{
		return m_death;
	}
	/// <summary>
	/// �N���A�������ǂ����̃t���O��Ԃ��֐��B
	/// </summary>
	/// <returns>
	/// true	�N���A�����B
	/// false	�N���A���Ă��Ȃ��B
	/// </returns>
	bool GetClear() const
	{
		return m_clear;
	}
	/// <summary>
	/// �e�ۂ��o�Ă��邩�Ԃ��֐��B
	/// </summary>
	/// <returns>
	/// true	�o�Ă���B
	/// false	�o�Ă��Ȃ��B
	/// </returns>
	bool GetDangan() const
	{
		return m_dangan;
	}
	/// <summary>
	/// ���ݕ����Ă����Ԃ��ǂ����Ԃ��֐��B
	/// </summary>
	/// <returns>
	/// true	�����Ă���B
	/// false	�����Ă��Ȃ��B
	/// </returns>
	bool GetCreep() const
	{
		return m_creep;
	}
	/// <summary>
	/// ���݂�HP��Ԃ��B
	/// </summary>
	/// <returns>
	/// m_hp�@�̗́B
	/// </returns>
	int GetHP() const
	{
		return m_hp;
	}
	
private:
	bool					m_fps = false;								//FPS�J������Ԃ��H
	GameCamera*				m_gameCamera = nullptr;						//TPS�J�����B
	FPSCamera*				m_fpsCamera = nullptr;						//FPS�J�����B
	IPlayerState*			m_currentstate = nullptr;					//�v���C���[�̌��݂̏�ԁB
	PlayerStateHoldGun		m_holdGunState;								//�e���\���Ă���X�e�[�g�B
	PlayerStateIdle			m_idleState;								//�ҋ@�X�e�[�g�B
	PlayerStateMove			m_moveState;								//�ړ��X�e�[�g�B
	PlayerReloadState		m_reloadState;								//�����[�h���̃X�e�[�g�B
	PlayerCreepState		m_creepState;								//�����ҋ@�X�e�[�g�B
	PlayerCreepMoveState	m_creepMoveState;							//�����ړ��X�e�[�g�B
	CharacterController		m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	enum EnAnimationClip {											
		enAnimationClip_idle,											//�ҋ@�B
		enAnimationClip_run,											//����B
		enAnimationClip_shot,											//�e�����B
		enAnimationClip_walk,											//�����B
		enAnimationClip_hold,											//�\����B
		enAnimationClip_shotend,										//�����I���B
		enAnimationClip_reload,											//�����[�h�B
		enAnimationClip_death,											//���S�B
		enAnimationClip_creep_idle,										//�����ҋ@�B
		enAnimationClip_creep_forward,									//�����O�i�B
		enAnimationClip_Num,
	};
	AnimationClip			m_animClips[enAnimationClip_Num];			//�A�j���[�V�����N���b�v�B
	UI*						m_ui = nullptr;								//UI�B
	bool					m_clear = false;							//�N���A�������ǂ����̃t���O�B
	int						m_hp = 5;									//���݂̃v���C���[��HP�B
	int						m_maxHP;									//HP�̍ő�l�B
	bool					m_Firing = false;							//�e�������Ă��邩�ǂ����B
	bool					m_dash = false;								//�����Ă��邩�ǂ����B
	bool					m_dangan = false;							//�e�ۂ��o�Ă��邩�ǂ����B
	int						m_maxAmmo;									//�e�ɑ��U�ł���ő�e���B
	bool					m_naturalRecoveryTrigger = false;			//���R�񕜂���t���O�B
	int						m_naturalRecoveryCount = 0;					//���R�񕜂܂ł̎��ԁB
	bool					m_creep = false;							//�����Ă��邩�ǂ����B
};