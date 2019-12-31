#pragma once
#include "Sprite.h"

class UI : public IGameObject
{
public:
	UI();
	~UI();
	bool Start() override;
	void Update() override;
	void Render2D()override;
private:
	Sprite		m_sprite;		//�X�v���C�g�B
};

