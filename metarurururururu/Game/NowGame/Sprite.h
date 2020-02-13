#pragma once
#include "graphics/Shader.h"

/// <summary>
/// スプライトクラス。
/// </summary>
class Sprite
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Sprite();

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Sprite();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="texFilePath">テクスチャのファイルパス。</param>
	/// <param name="w">画像の幅。</param>
	/// <param name="h">画像の高さ。</param>
	void Init(const wchar_t* texFilePath, float w, float h);

	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="trans">平行移動。</param>
	/// <param name="rot">回転。</param>
	/// <param name="scale">拡大率。</param>
	/// <param name="pivot">
	/// ピボット。
	/// 0.5, 0.5で画像の中心が基点。
	///	0.0, 0.0で画像の左下。
	///	1.0, 1.0で画像の右上。
	/// </param>
	void Update(const CVector3& trans,
				const CQuaternion& rot,
				const CVector3& scale,
				CVector2 pivot = { 0.5f,0.5f });

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw();
	/// <summary>
	/// アルファ値を設定する関数。
	/// </summary>
	/// <param name="delta">deltaに渡した値に応じてアルファ値を変更する。</param>
	void DeltaAlpha(float delta)
	{
		m_alpha += delta;
		//数値の境界チェック。
		if (m_alpha > 1.0f) {
			m_alpha = 1.0f;
		}
		else if (m_alpha < 0.0f) {
			m_alpha = 0.0f;
		}
	}
	/// <summary>
	/// アルファ値を設定する関数。
	/// </summary>
	/// <param name="alpha">alphaに渡した値をそのまま適用する。</param>
	void SetAlpha(float alpha)
	{
		m_alpha = 1.0f;
		m_alpha += alpha;
		if (m_alpha > 1.0f) {
			m_alpha = 1.0f;
		}
		else if (m_alpha < 0.0f) {
			m_alpha = 0.0f;
		}
	}
	/// <summary>
	/// 現在のアルファ値を返す関数。
	/// </summary>
	/// <returns>m_alpha　アルファ値。</returns>
	float GetAlpha() const
	{
		return m_alpha;
	}
private:

	/// <summary>
	/// 定数バッファの初期化。
	/// </summary>
	void InitConstantBuffer();

	/// <summary>
	/// 頂点バッファの初期化。
	/// </summary>
	/// <param name="w">画像の幅</param>
	/// <param name="h">画像の高さ</param>
	void InitVertexBuffer(float w, float h);

	/// <summary>
	/// インデックスバッファの初期化。
	/// </summary>
	void InitIndexBuffer();

	/// <summary>
	/// サンプラステートの初期化。
	/// </summary>
	void InitSamplerState();
	void SetRenderState();
	void GetRenderState();
private:
	struct ConstantBuffer {
		CMatrix WVP;		//ワールドビュープロジェクション行列。
		float alpha;		//α値。
	};
	ID3D11Buffer*				m_vertexBuffer = NULL;					//頂点バッファ。
	ID3D11Buffer*				m_indexBuffer = NULL;					//インデックスバッファ。
	Shader						m_vs;									//頂点シェーダー。
	Shader						m_ps;									//ピクセルシェーダー。
	ID3D11ShaderResourceView*	m_texture = NULL;						//テクスチャ。
	ID3D11SamplerState*			m_samplerState = NULL;					//サンプラステート。
	CVector3					m_position = CVector3::Zero();			//座標。
	CQuaternion					m_rotation = CQuaternion::Identity();	//回転
	CVector3					m_scale = CVector3::One();				//拡大率。
	CMatrix						m_world = CMatrix::Identity();			//ワールド行列。
	CVector2					m_size = CVector2::Zero();				//画像のサイズ。
	ID3D11Buffer*				m_cb = nullptr;							//定数バッファ。
	float						m_alpha = 1.0f;							//スプライトのα値。
	ID3D11DeviceContext*		m_DC = nullptr;							//デバイスコンテキスト。
	ID3D11DepthStencilState*	m_depthState = nullptr;					//現在のデプスステンシルステート。
	ID3D11RasterizerState*		m_rasterrizerState = nullptr;			//現在のラスタライザステート。
	ID3D11BlendState*			m_blendState = nullptr;					//現在のブレンドステート。
};

