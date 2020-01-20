#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "UI.h"

extern Pad g_pad[Pad::CONNECT_PAD_MAX];

class Bullet;
class GameCamera;
class FPSCamera;
class Player : public IGameObject
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
	/// �v���C���[�̃|�W�V������ݒ肷��֐��B
	/// </summary>
	/// <param name="pos">
	/// �|�W�V�����B
	/// </param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �v���C���[�̃|�W�V������Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>position	���W�B</returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
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
private:
	SkinModel			m_model;									//�X�L�����f���B
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	CVector3			m_moveSpeed = CVector3::Zero();
	CVector3			m_position = CVector3::Zero();				//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();
	bool				m_fps = false;								//FPS�J������Ԃ��H
	GameCamera*			m_gameCamera = nullptr;
	FPSCamera*			m_fpsCamera = nullptr;
	IPlayerState*		m_currentstate = nullptr;					//�v���C���[�̌��݂̏�ԁB
	PlayerStateHoldGun	m_holdGunState;								//�e���\���Ă���X�e�[�g�B
	PlayerStateIdle		m_idleState;								//�ҋ@�X�e�[�g�B
	PlayerStateMove		m_moveState;								//�ړ��X�e�[�g�B
	CharacterController m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	enum EnAnimationClip {											
		enAnimationClip_idle,										//�ҋ@�B
		enAnimationClip_run,										//����B
		enAnimationClip_shot,										//�e�����B
		enAnimationClip_walk,										//�����B
		enAnimationClip_hold,										//�\����B
		enAnimationClip_shotend,									//�����I���B
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];			//�A�j���[�V�����N���b�v�B
	UI*					m_ui = nullptr;								//UI�B
	bool				m_clear = false;							//�N���A�������ǂ����̃t���O�B
	int					m_hp = 9999910;								//�v���C���[��HP�B
	bool				m_death = false;							//����ł��邩�ǂ����B
	bool				m_Firing = false;							//�e�������Ă��邩�ǂ����B
	bool				m_dash = false;								//�����Ă��邩�ǂ����B
};

