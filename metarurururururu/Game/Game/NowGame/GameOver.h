#pragma once
#include "Sprite.h"

class GameOver : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	GameOver();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~GameOver();
	/// <summary>
	/// 開始関数。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
	void Render2D() override;
private:
	Sprite		m_sprite;			//スプライト。
	float		m_alph = 0.0f;		//アルファ値。
	int			m_timer = 0;		//タイマー。
};

