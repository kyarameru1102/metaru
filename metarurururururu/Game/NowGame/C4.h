#pragma once
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
private:
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	CVector3			m_position = CVector3::Zero();				//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();		//��]�B
};

