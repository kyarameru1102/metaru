#pragma once
#include "Player.h"
class Player;
class Bullet : public IGameObject
{
public:
	Bullet();
	~Bullet();
	bool Start() override;
	void Update() override;
	void SetmoveSpeed(CVector3 s) 
	{
		m_moveSpeed = s;
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
private:
	CVector3			m_position = CVector3::Zero();
	CVector3			m_moveSpeed = CVector3::Zero();
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	Player*				m_player = nullptr;
	float				m_toPlayerLen = 0.0f;						//�e�ۂƃv���C���[�̋����B
};

