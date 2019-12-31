#pragma once
#include "Sprite.h"

class Clear : public IGameObject
{
public:
	Clear();
	~Clear();
	bool Start() override;
	void Update() override;
	void Render2D() override;
private:
	Sprite		m_sprite;			//スプライト。
	float		m_alph = 0.0f;		//アルファ値。
};

