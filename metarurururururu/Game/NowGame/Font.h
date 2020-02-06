#pragma once
class Font
{
public:
	Font();
	~Font();
	//フォントを設定。
	void SetFont(DirectX::SpriteFont* font)
	{
		m_spriteFont = font;
	}
	//描画開始。
	void Begin();
	//描画終了。
	void End();
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="text">text 表示したいテキスト。</param>
	/// <param name="position">position 平行移動。</param>
	/// <param name="color">color カラー。</param>
	/// <param name="rotation">rotation 回転。</param>
	/// <param name="scale">scale 拡大。</param>
	/// <param name="pivot">
	/// pivot 基点。
	///	　0.5,0.5で画像の中心が基点。
	///	　0.0,0.0で画像の左下。
	///	　1.0,1.0で画像の右上。
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
	DirectX::SpriteBatch*		m_spriteBatch = nullptr;			//スプライトバッチ。
	DirectX::SpriteFont*		m_spriteFont = nullptr;				//スプライトフォント。
	CMatrix						m_scaleMat;
	ID3D11DeviceContext*		m_DC = nullptr;						//デバイスコンテキスト。
	ID3D11DepthStencilState*	m_depthState = nullptr;				//現在のデプスステンシルステート。
	ID3D11RasterizerState*		m_rasterrizerState = nullptr;		//現在のラスタライザステート。
	ID3D11BlendState*			m_blendState = nullptr;				//現在のブレンドステート。
};

