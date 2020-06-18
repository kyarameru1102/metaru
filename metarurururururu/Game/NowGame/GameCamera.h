#pragma once
#include "CameraCollisionSolver.h"
#include "SpringCamera.h"
/// <summary>
/// �O�l�̎��_�̃J�����B
/// </summary>
class Player;
class Enemy;
class ClearPoint;
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
	ClearPoint*				m_clearPoint = nullptr;
	CVector3				m_enemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };				//�G���̍��W�B
	CVector3				m_direction;												//�����B
	float					m_rightLength;
	bool					m_LookInTo = false;											//���z���̃t���O�B
	CVector3				m_okEnemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };
	bool					m_migi = false;
	bool					m_isLookOn = false;											//���b�N�I�����Ă��邩�ǂ����B
	bool					m_heri = false;												//�w���R�v�^�[�̂ق��ɃJ�����������邩�ǂ����B
	CVector3				m_heriPos = CVector3::Zero();								//�w���R�v�^�[�̍��W�B
	int						m_heriTimer = 120;											//�w���R�v�^�[�����Ă��鎞�ԁB
	SpringCamera			m_springCamera;
};

