#include "stdafx.h"
#include "DirectionLight.h"


DirectionLight::DirectionLight()
{
}


DirectionLight::~DirectionLight()
{
	//ライト用の定数バッファの解放。
	if (m_lightCb != nullptr) {
		m_lightCb->Release();
	}
}

void DirectionLight::Init()
{
	m_dirLight.direction = { 1.0f,0.0f,0.0f,0.0f };
	m_dirLight.Color = { 1.0f,1.0f,1.0f,1.0f };
}

bool DirectionLight::Start()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
	//作成。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	//ライト。
	bufferDesc.ByteWidth = sizeof(SDirectionLight);
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
	return true;
}

void DirectionLight::Update()
{
}

void DirectionLight::Render()
{
	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//ライト用の定数バッファを更新。
	deviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_dirLight, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_lightCb);
	
}
