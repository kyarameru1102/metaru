#pragma once

class Player;
class C4 : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	C4();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~C4();
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
	CVector3 GetPosition() const
	{
		return m_position;
	}
private:
	SkinModelRender*		m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	CVector3				m_position = CVector3::Zero();				//���W�B
	CQuaternion				m_rotation = CQuaternion::Identity();		//��]�B
	sensya*					m_sensya = nullptr;							//��ԁB
	EnemyGeneralCommander*	m_EGC = nullptr;							//�G���̑��i�߁B
	Player*					m_player = nullptr;							//�v���C���[�B
};
