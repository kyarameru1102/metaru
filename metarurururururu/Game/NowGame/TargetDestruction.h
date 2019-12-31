#pragma once
#include "Sprite.h"
class TargetDestruction : public IGameObject
{
public:
	TargetDestruction();
	~TargetDestruction();
	bool Start() override;
	void Update() override;
	void Render2D() override;
private:
	Sprite		m_sprite;			//�X�v���C�g�B
	float		m_alph = 0.0f;		//�A���t�@�l�B
	int			m_timer = 0.0f;		//�^�C�}�[�B
	bool		m_done = false;		//�����������ǂ����B
};

