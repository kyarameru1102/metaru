#pragma once
#include "RenderTarget.h"
#include "graphics/Shader.h"
#include "GaussianBlur.h"

class Bloom
{
public:
	Bloom();
	~Bloom();
	/// <summary>
	/// ブルームの描画処理。
	/// </summary>
	/// <param name="renderTarget">RenderTarget	レンダリングターゲット。</param>
	void Drow(RenderTarget& renderTarget);
	/// <summary>
	///フルスクリーン描画する関数。
	/// </summary>
	/// <param name="vsShader">Shader	頂点シェーダー。</param>
	/// <param name="psShader">Shader	ピクセルシェーダー。</param>
	void DrowFullScreen(Shader& vsShader, Shader& psShader);
private:
	RenderTarget			m_luminanceRT;						//輝度を抽出するレンダリングターゲット
	RenderTarget			m_blurCombineRT;					//ブラーの画像を合成するレンダリングターゲット。
	ID3D11SamplerState*		m_samplerState = nullptr;			//サンプラステート。
	ID3D11BlendState*		m_disableBlendState = nullptr;		//アルファブレンディングを無効にするブレンディングステート。
	ID3D11BlendState*		m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
	Shader					m_vs;								//何もしない頂点シェーダー。
	Shader					m_psLuminance;						//輝度抽出用のピクセルシェーダー。
	Shader					m_psFinal;							//最終合成用のピクセルシェーダー。
	Shader					m_psCombine;						//ぼかし画像合成用のピクセルシェーダー。
	//フルスクリーン描画用のメンバ変数。
	ID3D11Buffer*			m_vertexBuffer = nullptr;			//頂点バッファ。

	static const int NUM_DOWN_SAMPLE = 4;				//ダウンサンプリングの回数。
	GaussianBlur m_gausianBlur[NUM_DOWN_SAMPLE];	//輝度をぼかすためのガウシアンブラー。
													//1/2→1/4→1/8→1/16の解像度までダウンサンプリングされていきます。
};

