#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"

class Player;
class sensya : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	sensya();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~sensya();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	bool Start()override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update()override;
	/// <summary>
	/// ���W��ݒ肷��֐��B
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �p�x��ݒ肷��֐��B
	/// </summary>
	/// <param name="rot">���[�e�[�V����</param>
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ���W��Ԃ��֐��B
	/// </summary>
	/// <returns>�|�W�V����</returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
private:
	MeshCollider m_meshCollider;									//���b�V���R���C�_�[�B
	RigidBody m_rigidBody;											//���́B
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	SkinModelRender*	m_sensyaCollider = nullptr;					//��Ԃ̃R���C�_�[�p�̃X�L�����f�������_�[�B
	CVector3			m_position = CVector3::Zero();				//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();		//��]�B
	CVector3			m_scl = CVector3::One();					//�g�嗦�B
	Player*				m_player = nullptr;							//�v���C���[�B
	bool				m_c4On = false;								//c4�����邩�ǂ����̃t���O�B
};

