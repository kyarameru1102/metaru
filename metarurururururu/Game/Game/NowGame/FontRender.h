#pragma once
#include "Font.h"
class FontRender : public IGameObject
{
public:
	/// <summary>
	/// �t�H���g��ݒ肷��֐��B
	/// </summary>
	/// <param name="font">
	/// �t�H���g�@SpriteFont*
	/// </param>
	void SetFont(DirectX::SpriteFont* font)
	{
		m_font.SetFont(font);
	}
	/// <summary>
	/// �e�L�X�g��ݒ肷��֐��B
	/// </summary>
	/// <param name="text">
	/// �e�L�X�g�@const wchar_t*
	/// </param>
	void SetText(const wchar_t* text)
	{
		if (wcscmp(m_text.c_str(), text) != 0) {
			//�����񂪕ς�����B
			m_text = text;
		}

	}
	/// <summary>
	/// ���W��ݒ肷��֐��B
	/// </summary>
	/// <param name="pos">
	/// ���W�@CVector2
	/// </param>
	void SetPosition(const CVector2& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �J���[��ݒ肷��֐��B
	/// </summary>
	/// <param name="color">
	/// �J���[�@CVector3
	/// </param>
	void SetColor(const CVector3& color)
	{
		m_color = color;
	}
	/// <summary>
	/// ��]��ݒ肷��֐��B
	/// </summary>
	/// <param name="rotation">
	/// ��]�@float
	/// </param>
	void SetRotation(float rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �g�嗦��ݒ肷��֐��B
	/// </summary>
	/// <param name="scale">
	/// �X�P�[���@float
	/// </param>
	void SetScale(float scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �s�{�b�g��ݒ肷��֐��B
	/// </summary>
	/// <param name="pivot">
	/// �s�{�b�g�@CVector2
	/// </param>
	void SetPivot(const CVector2& pivot)
	{
		m_pivot = pivot;
	}

	void RenderFont();
private:
	Font m_font;
	std::wstring m_text;						//�e�L�X�g�B
	const wchar_t* m_textUnsafe = nullptr;		//�A���Z�[�t�ł̃e�L�X�g�B
	CVector2 m_position = CVector2::Zero();		//���W�Bx = 0.0f, y = 0.0f�ŉ�ʂ̒��S�B
	CVector4 m_color = CVector4::White();		//�J���[�B
	float m_rotation = 0.0f;					//��]�B
	float m_scale = 1.0f;						//�g�嗦�B
	CVector2 m_pivot;							//�s�{�b�g�B
};

