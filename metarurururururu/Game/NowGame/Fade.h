#pragma once
#include "Sprite.h"

class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start()override;
	void Update()override;
	void Render2D()override;
	float GetSpriteAlpha() const
	{
		return m_sprite.GetAlpha();
	}
private:
	Sprite		m_sprite;				//スプライト。
	bool		m_fadeOutOK = false;	//フェードアウトしてもよい場合true。
};

