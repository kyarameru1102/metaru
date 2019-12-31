#pragma once
#include "Sprite.h"

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update()override;
	void Render2D()override;
private:
	Sprite		m_sprite;		//スプライト。
};

