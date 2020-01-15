#pragma once
#include "Sprite.h"

class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start() override;
	void Update() override;
	void Render2D() override;
private:
	Sprite		m_sprite;			//スプライト。
	float		m_alph = 0.0f;		//アルファ値。
	int			m_timer = 0;		//タイマー。
};

