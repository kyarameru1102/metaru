#pragma once
#include "Sprite.h"

class Result : public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override;
	void Update() override;
	void Render2D()override;
	void SetKillCount(int killCount)
	{
		m_killCount = killCount;
	}
private:
	int			m_killCount = 0;
	Sprite		m_sprite;
};

