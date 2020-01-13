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
	Sprite		m_sprite;				//�X�v���C�g�B
	bool		m_fadeOutOK = false;	//�t�F�[�h�A�E�g���Ă��悢�ꍇtrue�B
};

