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
private:
	CVector3			m_position = CVector3::Zero();
	CVector3			m_moveSpeed = CVector3::Zero();
	SkinModelRender*	m_skinModelRender = nullptr;				//スキンモデルレンダー。
	Player*				m_player = nullptr;
	Enemy*				m_enemy = nullptr;
	float				m_toPlayerLen = 0.0f;						//弾丸とプレイヤーの距離。
	bool				m_havePlayer = false;						//プレイヤーの弾丸かどうか。
	bool				m_haveEnemy = false;						//エネミーの弾丸かどうか。
	CVector3			m_toPos;
};

