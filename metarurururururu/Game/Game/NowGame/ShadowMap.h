#pragma once
#include "RenderTarget.h"

class ShadowMap
{
public:
	//コンストラクタ。
	ShadowMap();
	//デストラクタ。
	~ShadowMap();
	static inline ShadowMap& GetShadowMap()
	{
		static ShadowMap shadowmap;
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
	/// 更新(ライトカメラの注視点を指定するバージョン。
	/// </summary>
	/// UpdateFromLightTargetかUpdateFromLightDirのどちらかを毎フレーム呼ぶ。
	/// <param name="lightPos">ライトカメラの視点の座標。</param>
	/// <param name="lightTarget">ライトカメラの注視点の座標。</param>
	void UpdateFromLightTarget(CVector3 lightPos, CVector3 lightTarget);
	/// <summary>
	/// 更新(ライトカメラの向きを指定するバージョン。
	/// </summary>
	/// 毎フレーム呼ぶ。
	/// <param name="lightPos">ライトカメラの視点の座標</param>
	/// <param name="lightDir">ライトの方向</param>
	void UpdateFromLightDir(CVector3 lightPos, CVector3 lightDir);
	/// <summary>
	/// シャドウマップに描画されるシャドウキャスターは
	/// RegistShadowCaster関数を使用して登録されたキャスターのみです。
	/// この関数を呼び出すと、シャドウキャスターのリストはクリアされます。
	/// </summary>
	void RenderToShadowMap();
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
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//ライトカメラの視点。
	CVector3 m_lightCameraTarget = CVector3::Zero();	//ライトカメラの注視点。
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//ライトビュー行列。
	CMatrix m_lightProjMatrix = CMatrix::Identity();	//ライトプロジェクション行列。
	RenderTarget m_shadowMapRT;						//シャドウマップを描画するレンダリングターゲット。
	std::vector< SkinModel*> m_shadowCasters;	//シャドウキャスターの配列。
};

