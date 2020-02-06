#include "stdafx.h"
#include "Font.h"


Font::Font()
{
	m_spriteBatch = g_graphicsEngine->GetSpriteBatch();
	m_spriteFont = g_graphicsEngine->GetSpriteFont();
	auto& ge = g_graphicsEngine;
	m_scaleMat.MakeScaling(
		{
			ge->GetFrameBufferWidth() / static_cast<float>(ge->Get2DSpaceScreenWidth()),
			ge->GetFrameBufferHeight() / static_cast<float>(ge->Get2DSpaceScreenHeight()),
			1.0f
		}
	);
}


Font::~Font()
{
}

void Font::Begin()
{
	//レンダーステートを保存しておく。
	m_DC = g_graphicsEngine->GetD3DDeviceContext();
	GetRenderState();

	m_spriteBatch->Begin(
		DirectX::SpriteSortMode_Deferred,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		m_scaleMat
	);
}

void Font::End()
{
	m_spriteBatch->End();
	//レンダーステートを設定しなおす。
	SetRenderState();
}

void Font::Draw(
	wchar_t const * text,
	const CVector2 & position,
	const CVector4 & color,
	float rotation,
	float scale,
	CVector2 pivot)
{
	if (text == nullptr) {
		return;
	}
	pivot.y = 1.0f - pivot.y;
	DirectX::XMFLOAT2 tkFloat2Zero(0, 0);
	//座標系をスプライトと合わせる。
	CVector2 pos = position;
	float frameBufferHalfWidth = 1280 * 0.5f;
	float frameBufferHalfHeight = 1280 * 0.5f;
	pos.x += frameBufferHalfWidth;
	pos.y = -pos.y + frameBufferHalfHeight;

	m_spriteFont->DrawString(
		m_spriteBatch,
		text,
		pos.vec,
		color,
		rotation,
		DirectX::XMFLOAT2(pivot.x, pivot.y),
		scale
	);
}

void Font::SetRenderState()
{
	m_DC->OMSetDepthStencilState(m_depthState, 0);
	m_DC->OMSetBlendState(m_blendState, nullptr, 0xFFFFFFFF);
	m_DC->RSSetState(m_rasterrizerState);
}

void Font::GetRenderState()
{
	m_DC->OMGetDepthStencilState(&m_depthState, 0);
	float blendfactor[4];
	UINT mask;
	m_DC->OMGetBlendState(&m_blendState, blendfactor, &mask);
	m_DC->RSGetState(&m_rasterrizerState);
}
