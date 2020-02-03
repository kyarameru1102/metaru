#pragma once
/*!
 *@brief	�O���t�B�b�N�X�G���W���B
 */
class GraphicsEngine
{
public:
	enum RenderMode {
		EnNormal,
		EnShadowMap
	};
	
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	�������B
	 *@param[in]	hWnd		�E�B���h�E�n���h���B
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	����B
	 */
	void Release();
	/*!
	 *@brief	D3D11�f�o�C�X���擾�B
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11�f�o�C�X�R���e�L�X�g���擾�B
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/*!
	 *@brief	�`��J�n�B
	 */
	void BegineRender();
	/*!
	 *@brief	�`��I���B
	 */
	void EndRender();
	void SetRenderMode(RenderMode renderMode)
	{
		m_renderMode = renderMode;
	}
	RenderMode GetRenderMode()
	{
		return m_renderMode;
	}
	DirectX::SpriteBatch* GetSpriteBatch() const
	{
		return m_spriteBatch.get();
	}
	DirectX::SpriteFont* GetSpriteFont() const
	{
		return m_spriteFont.get();
	}
	//�t���[���o�b�t�@�̕����擾�B
	int GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	//�t���[���o�b�t�@�̍������擾�B
	int GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}
	//2D��Ԃ̃X�N���[���̕��B
	int Get2DSpaceScreenWidth() const
	{
		return m_2dSpaceScreenWidth;
	}
	//2D��Ԃ̃X�N���[���̍����B
	int Get2DSpaceScreenHeight() const
	{
		return m_2dSpaceScreenHeight;
	}
private:
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;								//�X�v���C�g�o�b�`�B
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFont;								//�X�v���C�g�t�H���g�B
	int										m_frameBufferWidth = 0;						//�t���[���o�b�t�@�̕��B���ꂪ�����𑜓x�B
	int										m_frameBufferHeight = 0;					//�t���[���o�b�t�@�̍����B���ꂪ�����𑜓x�B
	const int								m_2dSpaceScreenWidth = 1280;				//2D��Ԃ̃X�N���[���̕��B2D�͂��̍��W�n��O��ɕ\������Ă���΁A�𑜓x���ς���Ă����v�B
	const int								m_2dSpaceScreenHeight = 720;				//2D��Ԃ̃X�N���[���̍����B2D�͂��̍��W�n��O��ɕ\������Ă���΁A�𑜓x���ς���Ă����v�B
	D3D_FEATURE_LEVEL						m_featureLevel;								//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
	ID3D11Device*							m_pd3dDevice = NULL;						//D3D11�f�o�C�X�B
	IDXGISwapChain*							m_pSwapChain = NULL;						//�X���b�v�`�F�C���B
	ID3D11DeviceContext*					m_pd3dDeviceContext = NULL;					//D3D11�f�o�C�X�R���e�L�X�g�B
	ID3D11RenderTargetView*					m_backBuffer = NULL;						//�o�b�N�o�b�t�@�B
	ID3D11RasterizerState*					m_rasterizerState = NULL;					//���X�^���C�U�X�e�[�g�B
	ID3D11Texture2D*						m_depthStencil = NULL;						//�f�v�X�X�e���V���B
	ID3D11DepthStencilView*					m_depthStencilView = NULL;					//�f�v�X�X�e���V���r���[�B
	RenderMode								m_renderMode = EnNormal;
};

extern GraphicsEngine* g_graphicsEngine;			//�O���t�B�b�N�X�G���W��