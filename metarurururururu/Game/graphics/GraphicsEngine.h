#pragma once
/*!
 *@brief	グラフィックスエンジン。
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
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/*!
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
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
	//フレームバッファの幅を取得。
	int GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	//フレームバッファの高さを取得。
	int GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}
	//2D空間のスクリーンの幅。
	int Get2DSpaceScreenWidth() const
	{
		return m_2dSpaceScreenWidth;
	}
	//2D空間のスクリーンの高さ。
	int Get2DSpaceScreenHeight() const
	{
		return m_2dSpaceScreenHeight;
	}
private:
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;								//スプライトバッチ。
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFont;								//スプライトフォント。
	int										m_frameBufferWidth = 0;						//フレームバッファの幅。これが内部解像度。
	int										m_frameBufferHeight = 0;					//フレームバッファの高さ。これが内部解像度。
	const int								m_2dSpaceScreenWidth = 1280;				//2D空間のスクリーンの幅。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
	const int								m_2dSpaceScreenHeight = 720;				//2D空間のスクリーンの高さ。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
	D3D_FEATURE_LEVEL						m_featureLevel;								//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*							m_pd3dDevice = NULL;						//D3D11デバイス。
	IDXGISwapChain*							m_pSwapChain = NULL;						//スワップチェイン。
	ID3D11DeviceContext*					m_pd3dDeviceContext = NULL;					//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView*					m_backBuffer = NULL;						//バックバッファ。
	ID3D11RasterizerState*					m_rasterizerState = NULL;					//ラスタライザステート。
	ID3D11Texture2D*						m_depthStencil = NULL;						//デプスステンシル。
	ID3D11DepthStencilView*					m_depthStencilView = NULL;					//デプスステンシルビュー。
	RenderMode								m_renderMode = EnNormal;
};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン