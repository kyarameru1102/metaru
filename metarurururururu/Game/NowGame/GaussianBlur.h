#pragma once
#include "RenderTarget.h"
#include "graphics/Shader.h"

/// <summary>
/// ガウシアンブラーをかけるクラス。
/// </summary>
class GaussianBlur
{
public:
	void Init(ID3D11ShaderResourceView* srcTextureSRV, float blurIntensity);
	void Update();
	void Execute(Shader& vs, ID3D11Buffer* vertexBuffer);
	/// <summary>
	/// フルスクリーン描画する関数。
	/// </summary>
	/// <param name="vsShader">Shader	頂点シェーダー。</param>
	/// <param name="psShader">Shader	ピクセルシェーダー。</param>
	void DrowFullScreen(Shader& vsShader, Shader& psShader, Shader& vs, ID3D11Buffer* vertexBuffer);
	ID3D11ShaderResourceView* GetResultTextureSRV()
	{
		return m_downSamplingRT[1].GetRenderTargetSRV();
	}
private:
	static const int NUM_WEIGHTS = 8;	//カウスフィルタの重みの数。
	/// <summary>
	/// ブラー用のパラメータバッファ。
	/// </summary>
	struct SBlurParam {
		float weights[NUM_WEIGHTS];
	};
	ID3D11ShaderResourceView*		m_srcTextureSRV = nullptr;			//ソーステクスチャのSRV
	float							m_blurIntensity = 25.0f;			//ブラーの強さ。
	unsigned int					m_srcTextureWidth = 0;				//ソーステクスチャの幅。
	unsigned int					m_srcTextureHeight = 0;				//ソーステクスチャの高さ。
	RenderTarget					m_downSamplingRT[2];				//ブラーをかけるためのダウンサンプリング用のレンダリングターゲット。
	Shader							m_vsXBlur;							//Xブラー用の頂点シェーダー。
	Shader							m_vsYBlur;							//Yブラー用の頂点シェーダー。
	Shader							m_psBlur;							//ブラー用のピクセルシェーダー。
	bool							m_isInited = false;					//初期化済みフラグ。
	SBlurParam						m_blurParam;						//ブラー用のパラメータ。
	ID3D11Buffer*					m_constantBuffer = nullptr;			//GPUBuffe上の定数バッファ。
};

