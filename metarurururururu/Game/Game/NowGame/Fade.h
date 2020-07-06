#pragma once
#include "Sprite.h"

class Fade : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Fade();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Fade();
	/// <summary>
	/// 開始関数。
	/// </summary>
	/// <returns></returns>
	bool Start()override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update()override;
	void Render2D()override;
	/// <summary>
	/// 現在のアルファ値を返す関数。
	/// </summary>
	/// <returns>m_sprite.GetAlpha()　アルファ値</returns>
	float GetSpriteAlpha() const
	{
		return m_sprite.GetAlpha();
	}
private:
	Sprite		m_sprite;				//スプライト。
	bool		m_fadeOutOK = false;	//フェードアウトしてもよい場合true。
};

