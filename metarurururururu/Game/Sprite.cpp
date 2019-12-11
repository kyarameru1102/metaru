#include "stdafx.h"
#include "Sprite.h"

struct SVertex {
	float position[4];		//頂点座標。4要素なのは今は気にしない。
	float uv[2];			//UV座標。これがテクスチャ座標
};

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	/*if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer != nullptr) {
		m_indexBuffer->Release();
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
	if (m_texture != nullptr) {
		m_texture->Release();
	}
	if (m_cb != nullptr) {
		m_cb->Release();
	}*/
}

void Sprite::Init(const wchar_t * texFilePath, float w, float h)
{
}

void Sprite::Update(const CVector3 & trans, const CQuaternion & rot, const CVector3 & scale, CVector2 pivot)
{
}

void Sprite::Draw()
{
}

void Sprite::InitConstantBuffer()
{
}

void Sprite::InitVertexBuffer(float w, float h)
{
	//float halfW = w * 0.5f;
	//float halfH = h * 0.5f;

	//SVertex vertex[4] = {
	//	//vertex[0]
	//	{
	//		//position[4]
	//		-halfW,  -halfH, 0.0f, 1.0f,
	//		//uv[2]
	//		0.0f, 1.0f
	//	},
	//	//vertex[1]
	//	{
	//		//position[4]
	//		halfW, -halfH, 0.0f, 1.0f,
	//		//uv[2]
	//		1.0f, 1.0f
	//	},
	//	//vertex[2]
	//	{
	//		//position[4]
	//		-halfW,  halfH, 0.0f, 1.0f,
	//		//uv[2]
	//		0.0f, 0.0f
	//	},
	//	//vertex[3]
	//	{
	//		//position[4]
	//		halfW,  halfH, 0.0f, 1.0f,
	//		//uv[2]
	//		1.0f, 0.0f
	//	},
	//};
	////上で定義した頂点を使用して頂点バッファを作成する。
	////頂点バッファを作成するためにはD3D11_BUFFER_DESCとD3D11_SUBRESOURCE_DATAを設定する必要がある。
	//D3D11_BUFFER_DESC bd;
}

void Sprite::InitIndexBuffer()
{
}

void Sprite::InitSamplerState()
{
}
