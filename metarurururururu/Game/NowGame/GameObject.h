#pragma once
class CGameObjectManager;
typedef unsigned char GOPrio;

/// <summary>
/// �Q�[���I�u�W�F�N�g�B
/// </summary>
class IGameObject {
public:
	IGameObject();
	virtual ~IGameObject() {};
	/// <summary>
	/// Update�̒��O�ɌĂ΂��J�n�����B
	/// </summary>
	/// <returns>
	/// Start�֐���true��Ԃ���Update���Ă΂ꂾ���B
	/// </returns>
	virtual bool Start() { return true; }
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// �폜�����Ƃ��ɌĂ΂��֐��B
	/// </summary>
	virtual void OnDestroy() {}
	/// <summary>
	/// �`��֐��B
	/// </summary>
	virtual void Render() {}
	/// <summary>
	/// 2D�p�̕`��֐��B
	/// </summary>
	virtual void Render2D() {}
	/// /// <summary>
	/// �D��x���擾�B
	/// </summary>
	GOPrio GetPriority() const
	{
		return m_priority;
	}
	/// <summary>
	/// m_isStart��true�������֐��B
	/// </summary>	
	void StartCallEnd()
	{
		m_isStart = true;
	}
	/// <summary>
	/// m_isStart��true��false����Ԃ��֐��B
	/// </summary>
	bool GetStart()
	{
		return m_isStart;
	}
	/// <summary>
	/// �D��x��ݒ肷��֐��B
	/// </summary>
	void SetPriority(GOPrio prio)
	{
		m_priority = prio;
	}
	/// <summary>
	/// ���O�L�[��ݒ肷��֐��B
	/// </summary>
	void SetNameKey(unsigned int nK)
	{
		m_nameKey = nK;
	}
	/// <summary>
	/// ���S�t���O�𗧂Ă�֐��B
	/// </summary>
	void SetIsDead()
	{
		m_isDead = true;
	}
	/// <summary>
	/// ����ł܂����H
	/// </summary>
	bool GetIsDead()
	{
		return m_isDead;
	}
	friend class GameObjectManager;
protected:
	GOPrio			m_priority;					//�D��x�B
	bool			m_isStart = false;			//Start�֐����Ă΂ꂽ���H
	unsigned int	m_nameKey = 0;				//���O�L�[�B
	bool			m_isDead = false;			//���S�t���O�B
};
