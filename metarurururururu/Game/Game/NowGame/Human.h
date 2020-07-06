#pragma once
class Human : public IGameObject
{
public:
	/// <summary>
	/// �|�W�V������ݒ肷��֐��B
	/// </summary>
	/// <param name="pos">
	/// ���W�B
	/// </param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot) {
		m_rotation = rot;
	}
	/// <summary>
	/// �|�W�V������Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>
	/// ���W�B
	/// </returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �c�e��\������B
	/// </summary>
	/// <returns>
	/// ammo  �c�e�B
	/// </returns>
	int GetAmmo() const
	{
		return m_ammo;
	}
protected:
	SkinModel			m_model;									//�X�L�����f���B
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	CVector3			m_moveSpeed = CVector3::Zero();				//�ړ����x�y�шړ������B
	CVector3			m_position = CVector3::Zero();				//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();		//��]�B
	bool				m_death = false;							//����ł��邩�ǂ����B
	int					m_ammo = 30;								//�c�e�B
	int					m_shotTimer = 0;							//�e�������̃^�C�}�[�B
	bool				m_shotTimerOn = false;						//�^�C�}�[���n�߂Ă������ǂ����B
	int					m_reloadTimer = 130;						//�����[�h������Ƃ��̃^�C�}�[�B
};