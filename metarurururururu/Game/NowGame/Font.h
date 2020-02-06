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
	void SetRenderState();
	void GetRenderState();
private:
	DirectX::SpriteBatch*		m_spriteBatch = nullptr;			//�X�v���C�g�o�b�`�B
	DirectX::SpriteFont*		m_spriteFont = nullptr;				//�X�v���C�g�t�H���g�B
	CMatrix						m_scaleMat;
	ID3D11DeviceContext*		m_DC = nullptr;						//�f�o�C�X�R���e�L�X�g�B
	ID3D11DepthStencilState*	m_depthState = nullptr;				//���݂̃f�v�X�X�e���V���X�e�[�g�B
	ID3D11RasterizerState*		m_rasterrizerState = nullptr;		//���݂̃��X�^���C�U�X�e�[�g�B
	ID3D11BlendState*			m_blendState = nullptr;				//���݂̃u�����h�X�e�[�g�B
};

