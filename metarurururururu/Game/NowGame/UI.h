#pragma once
#include "Sprite.h"
#include "FontRender.h"

class Player;
class UI : public IGameObject
{
public:
	UI();
	~UI();
	bool Start() override;
	void Update() override;
	void Render2D()override;
	void OnDestroy() override;
private:
	Sprite		m_sprite;				//�X�v���C�g�B
	FontRender* m_fontRender = nullptr; //�t�H���g�B
	Player*     m_player = nullptr;		//�v���C���[�B
};

