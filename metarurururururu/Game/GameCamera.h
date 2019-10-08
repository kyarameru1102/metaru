#pragma once
#include "CameraCollisionSolver.h"

class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void Render() override;
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
	CVector3 Getdirection()
	{
		return m_direction;
	}
private:
	CVector3				m_target = CVector3::Zero();
	Player*					m_player = nullptr;
	CVector3				m_direction;					//�����B
	CameraCollisionSolver	m_cameraCollider;				//�J�����̃R���C�_�[�B
	float					m_rightLength;
	CameraCollisionSolver	m_targetCollider;				//�����_�̃R���C�_�[
};

