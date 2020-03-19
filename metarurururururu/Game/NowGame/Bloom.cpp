#include "stdafx.h"
#include "Bloom.h"


struct SVertex {
	float position[4];		//頂点座標。
	float uv[2];			//UV座標。これがテクスチャ座標
};


Bloom::Bloom()
{
	//レンダリングターゲットの初期化。
	{
		//輝度抽出用のレンダリングターゲットを作成する。
		m_luminanceRT.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			DXGI_FORMAT_R16G16B16A16_FLOAT
		);
		//ぼかした画像を合成するためのレンダリングターゲット
		//解像度は1/2でよい。
		m_blurCombineRT.Create(
			FRAME_BUFFER_W / 2,
			FRAME_BUFFER_H / 2,
			DXGI_FORMAT_R16G16B16A16_FLOAT
		);
	}

	//シェーダーの初期化。
	{
		m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
		m_psLuminance.Load("Assets/shader/bloom.fx", "PSSamplingLuminance", Shader::EnType::PS);
		m_psFinal.Load("Assets/shader/bloom.fx", "PSFinal", Shader::EnType::PS);
		m_psCombine.Load("Assets/shader/bloom.fx", "PSCombine", Shader::EnType::PS);
	}

	//aブレンディングステートを初期化。
	{
		CD3D11_DEFAULT defaultSettings;
		//デフォルトセッティングで初期化する。
		CD3D11_BLEND_DESC blendDesc(defaultSettings);
		auto device = g_graphicsEngine->GetD3DDevice();

		device->CreateBlendState(&blendDesc, &m_disableBlendState);

		//最終合成用のブレンドステートを作成する。
		//最終合成は加算合成。
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		device->CreateBlendState(&blendDesc, &m_finalBlendState);
	}

	//サンプラステートの初期化。
	{
		D3D11_SAMPLER_DESC Sdesc;
		ZeroMemory(&Sdesc, sizeof(Sdesc));
		Sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		Sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&Sdesc, &m_samplerState);
	}

	//輝度テクスチャをぼかすためのガウシアンブラーを初期化する。
	ID3D11ShaderResourceView* srcBlurTexture = m_luminanceRT.GetRenderTargetSRV();
	for (auto& gaussianBlur : m_gausianBlur) {
		gaussianBlur.Init(srcBlurTexture, 25.0f);
		//次のガウスブラーで使用するソーステクスチャを設定する。
		srcBlurTexture = gaussianBlur.GetResultTextureSRV();
	}

	//フルスクリーン描画のための四角形プリミティブを初期化。
	//頂点バッファを初期化。
	SVertex vertex[4] = {
		//頂点１
		{
			//座標　position[4]
			-1.0f,  -1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 1.0f
		},
		//頂点２
		{
			//座標　position[4]
			1.0f, -1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 1.0f
		},
		//頂点３
		{
			//座標　position[4]
			-1.0f,  1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 0.0f
		},
		//頂点４
		{
			//座標　position[4]
			1.0f,  1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 0.0f
		},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//構造体を0で初期化する。
	bd.Usage = D3D11_USAGE_DEFAULT;				//バッファーで想定されている読み込みおよび書き込みの方法。
												//取りあえずはD3D11_USAGE_DEFAULTでよい。
	bd.ByteWidth = sizeof(vertex);				//頂点バッファのサイズ。頂点のサイズ×頂点数となる。
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//これから作成するバッファが頂点バッファであることを指定する。

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//頂点バッファの作成。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
}


Bloom::~Bloom()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
	if (m_disableBlendState != nullptr) {
		m_disableBlendState->Release();
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
	if (m_finalBlendState != nullptr) {
		m_finalBlendState->Release();
	}
}


void Bloom::Drow(RenderTarget& renderTarget)
{

	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//輝度を抽出。
	{
		//αブレンドを無効にする。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);

		//輝度抽出用のレンダリングターゲットに変更する。
		g_graphicsEngine->ChangeRenderTarget(&m_luminanceRT, m_luminanceRT.GetViewport());
		//レンダリングターゲットのクリア。
		float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_luminanceRT.ClearRenderTarget(clearColor);

		//シーンをテクスチャとする。
		auto mainRTTexSRV = renderTarget.GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &mainRTTexSRV);

		//フルスクリーン描画。
		DrowFullScreen(m_vs,m_psLuminance);
	}
	//輝度テクスチャをガウシアンブラーでぼかす。
	{
		for (auto& gaussianBlur : m_gausianBlur) {
			gaussianBlur.Execute(m_vs,m_vertexBuffer);
		}
	}
	//ぼかした画像を合成用のレンダリングターゲットに合成。
	{
		//レンダリングターゲットを合成用に変更。
		g_graphicsEngine->ChangeRenderTarget(&m_blurCombineRT, m_blurCombineRT.GetViewport());
		//ガウシアンブラーをかけたテクスチャをt0～t3レジスタに設定する。
		for (int registerNo = 0; registerNo < NUM_DOWN_SAMPLE; registerNo++) {
			auto srv = m_gausianBlur[registerNo].GetResultTextureSRV();
			deviceContext->PSSetShaderResources(registerNo, 1, &srv);
		}

		//フルスクリーン描画。
		DrowFullScreen(m_vs, m_psCombine);
	}
	//ぼかしたテクスチャをメインレンダリングターゲットに合成加算。
	{
		auto mainRT = &renderTarget;
		g_graphicsEngine->ChangeRenderTarget(mainRT, mainRT->GetViewport());

		//XYブラーをかけたテクスチャをt0レジスタに設定する。
		auto srv = m_blurCombineRT.GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &srv);

		//加算合成用のブレンディングステートを設定する。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);

		//フルスクリーン描画。
		DrowFullScreen(m_vs, m_psFinal);

		//ブレンディングステートを戻す。
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);

	}


}

void Bloom::DrowFullScreen(Shader& vsShader, Shader& psShader)
{
	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//プリミティブのトポロジーとして、トライアングルストリップを設定する。
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	unsigned int vertexSize = sizeof(SVertex);
	unsigned int offset = 0;
	//輝度抽出用のシェーダーを設定する。
	deviceContext->VSSetShader(
		(ID3D11VertexShader*)vsShader.GetBody(), nullptr, 0
	);
	deviceContext->PSSetShader(
		(ID3D11PixelShader*)psShader.GetBody(), nullptr, 0
	);
	deviceContext->IASetInputLayout(m_vs.GetInputLayout());

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->Draw(4, 0);
}


