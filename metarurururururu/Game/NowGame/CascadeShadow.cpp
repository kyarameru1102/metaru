#include "stdafx.h"
#include "CascadeShadow.h"


CascadeShadow::CascadeShadow()
{
	//シャドウマップ生成用のレンダリングターゲットを作成。
	//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
	
	m_shadowMapRT[0].Create(
		4096,
		4096,
		DXGI_FORMAT_R32_FLOAT
	);
	
	m_shadowMapRT[1].Create(
		2048,
		2048,
		DXGI_FORMAT_R32_FLOAT
	);

	m_shadowMapRT[2].Create(
		1024,
		1024,
		DXGI_FORMAT_R32_FLOAT
	);
}


CascadeShadow::~CascadeShadow()
{
}

void CascadeShadow::Update()
{
	 m_nearDistance[0] = g_camera3D.GetNear();									
	 m_farDistance[0] = 800.0f;											

	 m_nearDistance[1] = 800.0f;
	 m_farDistance[1] = 3000.0f;
	
	 m_nearDistance[2] = 3000.0f;
	 m_farDistance[2] = 15000.0f;


	for (int i = 0; i < m_numCascade; i++) {
		MatrixCalculation(m_nearDistance[i], m_farDistance[i],i);
	}
	
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

	for (int i = 0; i < m_numCascade; i++) {
		Render(i);
	}

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

void CascadeShadow::MatrixCalculation(float Near, float Far, int i)
{
	CVector3 cameraPos = g_camera3D.GetPosition();								//カメラの位置。
	float halfviewAngle = g_camera3D.GetViewAngle() * 0.5f;						//カメラの画角の半分。
	float aspect = g_camera3D.GetWidth() / g_camera3D.GetHeight();				//アスペクト比。

	float y1 = tan(halfviewAngle) * Near;										//近いほう。
	float y2 = tan(halfviewAngle) * Far;										//遠いほう。

	halfviewAngle *= aspect;
	float x1 = tan(halfviewAngle) * Near;										//近いほう。
	float x2 = tan(halfviewAngle) * Far;										//遠いほう。

	//8頂点を求める。
	CVector3 nearCenter = (g_camera3D.GetFront() * Near) + cameraPos;			//近平面の中心。
	CVector3 farCenter = (g_camera3D.GetFront() * Far) + cameraPos;				//遠平面の中心。

	
	m_eightVertices[i].vertex[0] = (g_camera3D.GetRight() * x1) + nearCenter;
	m_eightVertices[i].vertex[0] += (g_camera3D.GetUp() * y1);					//1つ目。
	m_eightVertices[i].vertex[1] = (g_camera3D.GetRight() * x1) + nearCenter;
	m_eightVertices[i].vertex[1] += (g_camera3D.GetUp() * -y1);					//2つ目。
	m_eightVertices[i].vertex[2] = (g_camera3D.GetRight() * -x1) + nearCenter;
	m_eightVertices[i].vertex[2] += (g_camera3D.GetUp() * y1);					//3つ目。
	m_eightVertices[i].vertex[3] = (g_camera3D.GetRight() * -x1) + nearCenter;
	m_eightVertices[i].vertex[3] += (g_camera3D.GetUp() * -y1);					//4つ目。

	m_eightVertices[i].vertex[4] = (g_camera3D.GetRight() * x2) + farCenter;
	m_eightVertices[i].vertex[4] += (g_camera3D.GetUp() * y2);					//5つ目。
	m_eightVertices[i].vertex[5] = (g_camera3D.GetRight() * x2) + farCenter;
	m_eightVertices[i].vertex[5] += (g_camera3D.GetUp() * -y2);					//6つ目。
	m_eightVertices[i].vertex[6] = (g_camera3D.GetRight() * -x2) + farCenter;
	m_eightVertices[i].vertex[6] += (g_camera3D.GetUp() * y2);					//7つ目。
	m_eightVertices[i].vertex[7] = (g_camera3D.GetRight() * -x2) + farCenter;
	m_eightVertices[i].vertex[7] += (g_camera3D.GetUp() * -y2);					//8つ目。

	//8頂点の中心を求める。
	CVector3 center = CVector3::Zero();
	for (int j = 0; j < 8; j++) {
		center += m_eightVertices[i].vertex[j];
	}
	center /= 8;

	//ライトの座標を求める。
	m_lightCameraTarget[i] = center;
	m_lightDirection = { 1.0f, -2.0f, 1.0f };
	m_lightDirection.Normalize();
	m_lightCameraPosition[i] = (m_lightDirection * -8000.0f) + m_lightCameraTarget[i];

	//ライトビュー行列を計算する。
	m_lightViewMatrix[i].MakeLookAt(
		m_lightCameraPosition[i],
		m_lightCameraTarget[i],
		CVector3::AxisY()
	);

	float xmin = FLT_MAX;		//xの最小。
	float ymin = FLT_MAX;		//yの最小。
	float xmax = FLT_MIN;		//xの最大。
	float ymax = FLT_MIN;		//yの最大。
	float zmax = FLT_MIN;		//zの最大。

	//8頂点にライトビュー行列を掛ける。
	for (int j = 0; j < 8; j++) {
		m_lightViewMatrix[i].Mul(m_eightVertices[i].vertex[j]);
		//そこからxyの最小最大の値とzの最大の値を求める。
		//xの最小。
		if (m_eightVertices[i].vertex[j].x < xmin) {
			xmin = m_eightVertices[i].vertex[j].x;
		}
		//yの最小。
		if (m_eightVertices[i].vertex[j].y < ymin) {
			ymin = m_eightVertices[i].vertex[j].y;
		}
		//xの最大。
		if (m_eightVertices[i].vertex[j].x > xmax) {
			xmax = m_eightVertices[i].vertex[j].x;
		}
		//yの最大。
		if (m_eightVertices[i].vertex[j].y > ymax) {
			ymax = m_eightVertices[i].vertex[j].y;
		}
		//zの最大。
		if (m_eightVertices[i].vertex[j].z > zmax) {
			zmax = m_eightVertices[i].vertex[j].z;
		}
	}

	//プロジェクション行列を作る。
	float w = xmax - xmin;		//幅。
	float h = ymax - ymin;		//高さ。

	m_lightProjMatrix[i].MakeOrthoProjectionMatrix(
		w,
		h,
		1.0f,
		zmax
	);
}

void CascadeShadow::Render(int i)
{
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_shadowMapRT[i].GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT[i].GetDepthStensilView());

	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[i].GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT[i].ClearRenderTarget(clearColor);

	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnShadowMap);
	//シャドウキャスターをシャドウマップにレンダリング。
	for (auto& caster : m_shadowCasters) {
		caster->Draw(
			m_lightViewMatrix[i],
			m_lightProjMatrix[i]
		);
	}
	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnNormal);

}
