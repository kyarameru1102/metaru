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
	Sprite		m_sprite;				//スプライト。
	FontRender* m_fontRender = nullptr; //フォント。
	Player*     m_player = nullptr;		//プレイヤー。
};

