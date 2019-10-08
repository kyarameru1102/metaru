#pragma once
class Font
{
public:
	Font();
	~Font();
	//�t�H���g��ݒ�B
	void SetFont(DirectX::SpriteFont* font)
	{
		m_spriteFont = font;
	}
	//�`��J�n�B
	void Begin();
	//�`��I���B
	void End();
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="text">text �\���������e�L�X�g�B</param>
	/// <param name="position">position ���s�ړ��B</param>
	/// <param name="color">color �J���[�B</param>
	/// <param name="rotation">rotation ��]�B</param>
	/// <param name="scale">scale �g��B</param>
	/// <param name="pivot">
	/// pivot ��_�B
	///	�@0.5,0.5�ŉ摜�̒��S����_�B
	///	�@0.0,0.0�ŉ摜�̍����B
	///	�@1.0,1.0�ŉ摜�̉E��B
	/// </param>
	void Draw(
		wchar_t const* text,
		const CVector2& position,
		const CVector4& color = {1.0f,1.0f,1.0f,1.0f},
		float rotation = 0.0f,
		float scale = 1.0f,
		CVector2 pivot = { 0.5f, 0.5f }
	);
private:
	DirectX::SpriteBatch*	m_spriteBatch = nullptr;	//�X�v���C�g�o�b�`�B
	DirectX::SpriteFont*	m_spriteFont = nullptr;		//�X�v���C�g�t�H���g�B
	CMatrix m_scaleMat;
};

