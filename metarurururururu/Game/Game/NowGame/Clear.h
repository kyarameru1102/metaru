#pragma once
#include "Sprite.h"

class Clear : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Clear();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Clear();
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
	/// <summary>
	/// 現在のアルファ値を返す。
	/// </summary>
	/// <returns></returns>
	float GetAlpha() const
	{
		return m_sprite.GetAlpha();
	}
private:
	Sprite		m_sprite;			//スプライト。
	int			m_timer = 0;		//タイマー。
};

