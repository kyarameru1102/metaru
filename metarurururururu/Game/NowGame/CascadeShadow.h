#pragma once
#include "RenderTarget.h"

class CascadeShadow
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	CascadeShadow();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~CascadeShadow();
	static inline CascadeShadow& GetCascadeShadowMap()
	{
		static CascadeShadow shadowmap;
		return shadowmap;
	};
	/// <summary>
	/// ライトビュー行列を取得。
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightViewMatrix(int i) const
	{
		return m_lightViewMatrix[i];
	}
	/// <summary>
	/// ライトプロジェクション行列を取得。
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix(int i) const
	{
		return m_lightProjMatrix[i];
	}
	/// <summary>
	/// シャドウキャスターを登録。
	/// </summary>
	/// 常に影を生成したい場合は毎フレーム呼ぶ。
	/// <param name="shadowCaster">シャドウキャスター。</param>
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}
	/// <summary>
	/// シャドウマップのSRVを取得。
	/// </summary>
	/// <returns>シャドウマップのSRV</returns>
	ID3D11ShaderResourceView* GetShadowMapSRV(int i)
	{
		return m_shadowMapRT[i].GetRenderTargetSRV();
	}
	
	void Update();
public:
	static const int	m_numCascade = 3;					//レンダリングターゲットの数。
private:
	/// <summary>
	/// 8頂点用の構造体。
	/// </summary>
	struct EightVertices {
		CVector3 vertex[8];
	};
	void MatrixCalculation(float Near,float Far,int i);		//行列関係の計算。
	void Render(int i);
private:
	EightVertices m_eightVertices[m_numCascade];
	CVector3 m_lightCameraPosition[m_numCascade];			//ライトカメラの視点。
	CVector3 m_lightCameraTarget[m_numCascade];				//ライトカメラの注視点。
	CVector3 m_lightDirection = CVector3::One();			//ライトの方向。
	CMatrix m_lightViewMatrix[m_numCascade];				//ライトビュー行列。
	CMatrix  m_lightProjMatrix[m_numCascade];
	RenderTarget m_shadowMapRT[m_numCascade];				//シャドウマップを描画するレンダリングターゲット。
	std::vector<SkinModel*> m_shadowCasters;				//シャドウキャスターの配列。
	float m_nearDistance[m_numCascade];						//カメラの近平面までの距離。
	float m_farDistance[m_numCascade];						//カメラの遠平面までの距離。
};

