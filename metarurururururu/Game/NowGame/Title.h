#pragma once
#include "Sprite.h"

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update()override;
	void Render()override;
	float GetSpriteAlpha() const
	{
		return m_sprite.GetAlpha();
	}
	void SetAlpha(float alpha) {
		m_sprite.DeltaAlpha(alpha);
	}
private:
	Sprite		m_sprite;		//スプライト。
	bool		m_on = false;			//Aボタンが押された時のフラグ。
	int			m_count = 0;
};

