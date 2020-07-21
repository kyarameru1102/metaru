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
	CMatrix GetLightViewMatrix() const
	{
		return m_lightViewMatrix;
	}
	/// <summary>
	/// ライトプロジェクション行列を取得。
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
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
	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT[0].GetRenderTargetSRV();
	}
	CVector3 GetLightPos()
	{
		return m_lightCameraPosition;
	}
	void Update();
public:
	static const int	m_numCascade = 3;			//レンダリングターゲットの数。
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//ライトカメラの視点。
	CVector3 m_lightCameraTarget = CVector3::Zero();	//ライトカメラの注視点。
	CVector3 m_lightDirection = CVector3::One();		//ライトの方向。
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//ライトビュー行列。
	CMatrix  m_lightProjMatrix;
	RenderTarget m_shadowMapRT[m_numCascade];			//シャドウマップを描画するレンダリングターゲット。
	std::vector<SkinModel*> m_shadowCasters;			//シャドウキャスターの配列。
};

