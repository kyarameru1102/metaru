#pragma once
#include "CameraCollisionSolver.h"
/// <summary>
/// �O�l�̎��_�̃J�����B
/// </summary>
class Player;
class Enemy;
class GameCamera : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	GameCamera();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~GameCamera();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	bool Start() override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �f�X�g���N�^����ɌĂ΂��֐��B
	/// DeleteGO�֐����Ă΂ꂽ�^�C�~���O�ŌĂ΂��֐��B
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// ���b�N�I�����Ă���Ƃ��̊֐��B
	/// </summary>
	/// <param name="pos">�J�����̃|�W�V�����B</param>
	void isLookOn(CVector3 pos);
	/// <summary>
	/// �J�����̌�����ݒ肷��B
	/// </summary>
	/// <param name="direction">
	/// �����B
	/// </param>
	void Setdirection(CVector3 direction)
	{
		m_direction = direction;
	}
	/// <summary>
	/// �J�����̌����Ă��������Ԃ��B
	/// </summary>
	/// <returns>
	/// �����B
	/// </returns>
	CVector3 Getdirection() const
	{
		return m_direction;
	}
private:
	CVector3				m_target = CVector3::Zero();
	Player*					m_player = nullptr;
	Enemy*					m_enemy = nullptr;
	CVector3				m_direction;					//�����B
	CameraCollisionSolver	m_cameraCollider;				//�J�����̃R���C�_�[�B
	float					m_rightLength;
	CameraCollisionSolver	m_targetCollider;				//�����_�̃R���C�_�[
	bool					m_lockOn = false;					//���b�N�I���̃t���O�B
};

