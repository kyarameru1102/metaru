#pragma once
#include "Sprite.h"

class Clear : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Clear();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Clear();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update() override;
	void Render2D() override;
private:
	Sprite		m_sprite;			//�X�v���C�g�B
	float		m_alph = 0.0f;		//�A���t�@�l�B
	int			m_timer = 0;		//�^�C�}�[�B
};

