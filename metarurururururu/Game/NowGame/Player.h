#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "SkinModelRender.h"
#include "Bullet.h"

extern Pad g_pad[Pad::CONNECT_PAD_MAX];

class Bullet;
class GameCamera;
class FPSCamera;
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void OnDestroy();
	//�ړ������B
	void Move();
	//�ړ����̃A�j���[�V���������B
	void MoveAnimation();
	//�e���\���Ă��鎞�̈ړ������B
	void HoldMove();
	//��]�����B
	void Rotation();
	//�e���\���Ă���Ƃ��̉�]�����B
	void HoldRotation();
	//�e���������B�e�ۂ𐶐����鏈���B
	void Firing();
	//�J�����̐؂�ւ������BFPS�J�����ɐ؂�ւ���B
	void CameraSwitchFPS();
	//�J�����̐؂�ւ������BTPS�J�����ɐ؂�ւ���B
	void CameraSwitchTPS();
	//�X�e�[�g��؂�ւ���֐��B
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
	CVector3 GetPosition()
	{
		return m_position;
	}
private:
	SkinModel			m_model;									//�X�L�����f���B
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	CVector3			m_moveSpeed = CVector3::Zero();
	CVector3			m_position = CVector3::Zero();				//���W�B
	CVector3			m_Plxz;										//��]�p�B
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
		enAnimationClip_idle,
		enAnimationClip_run,
		enAnimationClip_shot,
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];			//�A�j���[�V�����N���b�v�B
	//Bullet*				m_bullet = nullptr;							//�e�ہB
};

