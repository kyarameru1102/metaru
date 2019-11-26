#include "stdafx.h"
#include "ShadowMap.h"


ShadowMap::ShadowMap()
{
	//シャドウマップ生成用のレンダリングターゲットを作成。
	//解像度は2048×2048。
	//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
	m_shadowMapRT.Create(
		9182,
		9182,
		DXGI_FORMAT_R32_FLOAT
	);
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::UpdateFromLightTarget(CVector3 lightPos, CVector3 lightTarget)
{
	m_lightCameraTarget = lightTarget;
	m_lightCameraPosition = lightPos;
	//ライトの方向を計算する。
	auto lightDir = m_lightCameraTarget - m_lightCameraPosition;
	if (lightDir.Length() < 0.0001f) {
		//ライトカメラの注視点と視点が近すぎる。
		//恐らくバグなので、クラッシュさせる。
		std::abort();
	}
	//正規化して、方向ベクトルに変換する。
	lightDir.Normalize();
	//
	UpdateFromLightDir(lightPos, lightDir);
}

void ShadowMap::UpdateFromLightDir(CVector3 lightPos, CVector3 lightDir)
{
	//ライトの方向によって、ライトカメラの上方向を決める。
	CVector3 lightCameraUpAxis;
	if (fabsf(lightDir.y) > 0.99998f) {
		//ほぼ真上or真下を向いているので、1,0,0を上方向とする。
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		//真上を向いてないときは、0,1,0を上方向とする。
		lightCameraUpAxis = CVector3::AxisY();
	}
	//カメラの上方向が決まったので、ライトビュー行列を計算する。
	m_lightViewMatrix.MakeLookAt(
		lightPos,
		lightPos + lightDir,
		lightCameraUpAxis
	);

	//ライトプロジェクション行列を作成する。
	//太陽光からの影を落としたいなら、平行投影行列を作成する。
	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		5000,
		5000,
		0.1f,
		4000.0f
	);
}

void ShadowMap::RenderToShadowMap()
{
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//現在のレンダリングターゲットをバックアップしておく。
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&oldRenderTargetView,
		&oldDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);

	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_shadowMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT.GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT.ClearRenderTarget(clearColor);

	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnShadowMap);
	//シャドウキャスターをシャドウマップにレンダリング。
	for (auto& caster : m_shadowCasters) {
		caster->Draw(
			m_lightViewMatrix,
			m_lightProjMatrix
		);
	}
	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnNormal);

	//キャスターをクリア。
	m_shadowCasters.clear();

	//元に戻す。
	d3dDeviceContext->OMSetRenderTargets(
		1,
		&oldRenderTargetView,
		oldDepthStencilView
	);
	d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	//レンダリングターゲットとデプスステンシルの参照カウンタを下す。
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();
}
