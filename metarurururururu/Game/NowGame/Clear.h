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
	Sprite		m_sprite;			//�X�v���C�g�B
	float		m_alph = 0.0f;		//�A���t�@�l�B
};

