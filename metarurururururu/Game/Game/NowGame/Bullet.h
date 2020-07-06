#pragma once
#include "Player.h"
#include "Enemy.h"

class Player;
class Enemy;
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
	void SetPlayer()
	{
		m_havePlayer = true;
	}
	void SetEnemy()
	{
		m_haveEnemy = true;
	}
	/// <summary>
	/// ���W��Ԃ��֐��B
	/// </summary>
	/// <returns>position	���W�B</returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �N�̒e�ۂ��Ԃ��֐��B
	/// </summary>
	/// <returns>
	/// true	�v���C���[�̒e�ہB
	/// false	�G���̒e�ہB
	/// </returns>
	bool GetWhosebullet() const
	{
		return m_Players;
	}
private:
	CVector3			m_position = CVector3::Zero();
	CVector3			m_moveSpeed = CVector3::Zero();
	SkinModelRender*	m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	Player*				m_player = nullptr;
	Enemy*				m_enemy = nullptr;
	float				m_toPlayerLen = 0.0f;						//�e�ۂƃv���C���[�̋����B
	bool				m_havePlayer = false;						//�v���C���[�̒e�ۂ��ǂ����B
	bool				m_haveEnemy = false;						//�G�l�~�[�̒e�ۂ��ǂ����B
	CVector3			m_toPos;
	bool				m_Players = false;
};

