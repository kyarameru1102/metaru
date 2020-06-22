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
	/// <summary>
	/// プレイヤーがダメージを受けたときの処理。
	/// </summary>
	void Damage();
	void Render2D()override;
private:
	Sprite		m_sprite;				//カーソル用スプライト。
	Sprite		m_damegeSprite;			//被弾したとき用のスプライト。			
	Sprite		m_damegeSprite2;		//被弾したとき用のスプライト２．
	FontRender* m_fontRender = nullptr; //フォント。
	Player*     m_player = nullptr;		//プレイヤー。
	bool		m_hit = false;			//敵に弾丸があたった
	float		m_HP = 999999;
	float		m_maxHP = 999999;
	CVector3	m_fontColor = { 0.0f,0.0f,0.0f };
};

