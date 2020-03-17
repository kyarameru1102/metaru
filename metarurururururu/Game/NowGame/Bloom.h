#pragma once
#include "RenderTarget.h"
#include "graphics/Shader.h"

class Bloom
{
public:
	Bloom();
	~Bloom();
	void Update();
	void Drow(RenderTarget renderTarget);
private:
	static const int NUM_WEIGHTS = 8;	//カウスフィルタの重みの数。
	/// <summary>
	/// ブラー用のパラメータバッファ。
	/// </summary>
	struct SBlurParam {
		CVector4 offset;
		float weights[NUM_WEIGHTS];
	};
	ID3D11Buffer*			m_blurParamCB = nullptr;			//ブラーパラメータ用の定数バッファ。
	SBlurParam				m_blurParam;						//ブラー用のパラメータ。
	float					m_blurDispersion = 50.0f;			//ブラーの分散具合。大きくするほど強く分散する。
	RenderTarget			m_luminanceRT;						//輝度を抽出するレンダリングターゲット
	RenderTarget			m_downSamplingRT[2];				//ブラーをかけるためのダウンサンプリング用のレンダリングターゲット。
	ID3D11SamplerState*		m_samplerState = nullptr;			//サンプラステート。
	ID3D11BlendState*		m_disableBlendState = nullptr;		//アルファブレンディングを無効にするブレンディングステート。
	ID3D11BlendState*		m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
	Shader					m_vs;								//何もしない頂点シェーダー。
	Shader					m_psLuminance;						//輝度抽出用のピクセルシェーダー。
	Shader					m_vsXBlur;							//Xブラー用の頂点シェーダー。
	Shader					m_vsYBlur;							//Yブラー用の頂点シェーダー。
	Shader					m_psBlur;							//ブラー用のピクセルシェーダー。
	Shader					m_psFinal;							//最終合成用のピクセルシェーダー。
	//フルスクリーン描画用のメンバ変数。
	ID3D11Buffer*			m_vertexBuffer = nullptr;			//頂点バッファ。
	
};

