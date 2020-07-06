#pragma once
#include "Sprite.h"

class Fade : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Fade();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Fade();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	/// <returns></returns>
	bool Start()override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update()override;
	void Render2D()override;
	/// <summary>
	/// ���݂̃A���t�@�l��Ԃ��֐��B
	/// </summary>
	/// <returns>m_sprite.GetAlpha()�@�A���t�@�l</returns>
	float GetSpriteAlpha() const
	{
		return m_sprite.GetAlpha();
	}
private:
	Sprite		m_sprite;				//�X�v���C�g�B
	bool		m_fadeOutOK = false;	//�t�F�[�h�A�E�g���Ă��悢�ꍇtrue�B
};

