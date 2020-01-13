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
	Sprite		m_sprite;		//�X�v���C�g�B
	bool		m_on = false;			//A�{�^���������ꂽ���̃t���O�B
	int			m_count = 0;
};

