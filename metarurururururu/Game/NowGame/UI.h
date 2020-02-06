#pragma once
#include "Sprite.h"
#include "FontRender.h"

class Player;
class UI : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	UI();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~UI();
	/// <summary>
	/// 開始関数。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
	void Render2D()override;
private:
	Sprite		m_sprite;				//スプライト。
	FontRender* m_fontRender = nullptr; //フォント。
	Player*     m_player = nullptr;		//プレイヤー。
	bool		m_hit = false;			//敵に弾丸があたった
};

