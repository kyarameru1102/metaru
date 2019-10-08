#include "stdafx.h"
#include "Font.h"


Font::Font()
{
}


Font::~Font()
{
}

void Font::Begin()
{
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
