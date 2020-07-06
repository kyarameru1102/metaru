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
	/// <summary>
	/// ���݂̃A���t�@�l��Ԃ��B
	/// </summary>
	/// <returns></returns>
	float GetAlpha() const
	{
		return m_sprite.GetAlpha();
	}
private:
	Sprite		m_sprite;			//�X�v���C�g�B
	int			m_timer = 0;		//�^�C�}�[�B
};

