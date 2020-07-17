#include "stdafx.h"
#include "CascadeShadow.h"


CascadeShadow::CascadeShadow()
{
	for (int i = 0; i < m_numCascade; i++) {
		//シャドウマップ生成用のレンダリングターゲットを作成。
		//解像度は2048×2048。
		//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
		m_shadowMapRT[i].Create(
			4096,
			4096,
			DXGI_FORMAT_R32_FLOAT
		);
	}
}


CascadeShadow::~CascadeShadow()
{
}

void CascadeShadow::Update()
{
	CVector3 cameraPos = g_camera3D.GetPosition();								//カメラの位置。
	float halfviewAngle = g_camera3D.GetViewAngle() * 0.5f;						//カメラの画角の半分。
	float nearDistance = g_camera3D.GetFar();									//カメラの近平面までの距離。
	float farDistance = g_camera3D.GetNear();									//カメラの遠平面までの距離。

	//横用。
	float x1 = tan(halfviewAngle) * nearDistance;								//近いほう。
	float x2 = tan(halfviewAngle) * farDistance;								//遠いほう。

	//縦用。
	float aspect = g_camera3D.GetHeight() / g_camera3D.GetWidth();				//アスペクト比。
	halfviewAngle *= aspect;
	float y1 = tan(halfviewAngle) * nearDistance;								//近いほう。
	float y2 = tan(halfviewAngle) * farDistance;								//遠いほう。
	
	//8頂点を求める。
	CVector3 nearCenter = (g_camera3D.GetFront() * nearDistance) + cameraPos;	//近平面の中心。
	CVector3 farCenter = (g_camera3D.GetFront() * farDistance) + cameraPos;		//遠平面の中心。
	
	CVector3 vertex[8];
	vertex[0] = (g_camera3D.GetRight() * x1) + nearCenter;
	vertex[0] += (g_camera3D.GetUp() * y1);										//1つ目。
	vertex[1] = (g_camera3D.GetRight() * x1) + nearCenter;
	vertex[1] += (g_camera3D.GetUp() * -y1);									//2つ目。
	vertex[2] = (g_camera3D.GetRight() * -x1) + nearCenter;
	vertex[2] += (g_camera3D.GetUp() * y1);										//3つ目。
	vertex[3] = (g_camera3D.GetRight() * -x1) + nearCenter;
	vertex[3] += (g_camera3D.GetUp() * -y1);									//4つ目。

	vertex[4] = (g_camera3D.GetRight() * x2) + farCenter;
	vertex[4] += (g_camera3D.GetUp() * y2);										//5つ目。
	vertex[5] = (g_camera3D.GetRight() * x2) + farCenter;
	vertex[5] += (g_camera3D.GetUp() * -y2);									//6つ目。
	vertex[6] = (g_camera3D.GetRight() * -x2) + farCenter;
	vertex[6] += (g_camera3D.GetUp() * y2);										//7つ目。
	vertex[7] = (g_camera3D.GetRight() * -x2) + farCenter;
	vertex[7] += (g_camera3D.GetUp() * -y2);									//8つ目。

	//8頂点の中心を求める。
	CVector3 center;
	for (int i = 0; i < 8; i++) {
		center += vertex[i];
	}
	center /= 8;

	//ライトの座標を求める。
	m_lightCameraTarget = center;
	m_lightDirection = { 1.0f, -2.0f, 1.0f };
	m_lightDirection.Normalize();
	m_lightCameraPosition = (m_lightDirection * -2000.0f) + m_lightCameraTarget;

	//ライトビュー行列を計算する。
	m_lightViewMatrix.MakeLookAt(
		m_lightCameraPosition,
		m_lightCameraTarget,
		CVector3::AxisY()
	);

	float xmin = FLT_MAX;		//xの最小。
	float ymin = FLT_MAX;		//yの最小。
	float xmax = FLT_MIN;		//xの最大。
	float ymax = FLT_MIN;		//yの最大。
	float zmax = FLT_MIN;		//zの最大。

	//8頂点にライトビュー行列を掛ける。
	for (int i = 0; i < 8; i++) {
		m_lightViewMatrix.Mul(vertex[i]);
		//そこからxyの最小最大の値とzの最大の値を求める。
		//xの最小。
		if (vertex[i].x < xmin) {
			xmin = vertex[i].x;
		}
		//yの最小。
		if (vertex[i].y < ymin) {
			ymin = vertex[i].y;
		}
		//xの最大。
		if (vertex[i].x > xmax) {
			xmax = vertex[i].x;
		}
		//yの最大。
		if (vertex[i].y > ymax) {
			ymax = vertex[i].y;
		}
		//zの最大。
		if (vertex[i].z > zmax) {
			zmax = vertex[i].z;
		}
	}

	//プロジェクション行列を作る。
	float w = xmax - xmin;		//幅。
	float h = ymax - ymin;		//高さ。

	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		w,
		h,
		1.0f,
		zmax
	);

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
		m_shadowMapRT[0].GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT[0].GetDepthStensilView());

	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[0].GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT[0].ClearRenderTarget(clearColor);

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
