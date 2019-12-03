#include "stdafx.h"
#include "UI.h"


UI::UI()
{
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(g_graphicsEngine->GetD3DDeviceContext());
	DirectX::CreateDDSTextureFromFile(
		g_graphicsEngine->GetD3DDevice(),
		L"Resource/sprite/kaasoru.dds",
		nullptr,
		&shaderResourceView
	);
}


UI::~UI()
{
}

bool UI::Start()
{
	return true;
}

void UI::Update()
{
}

void UI::Render()
{
	spriteBatch.get()->Begin();
	spriteBatch.get()->Draw(
		shaderResourceView,
		DirectX::XMFLOAT2(FRAME_BUFFER_W / 2, FRAME_BUFFER_H / 2),
		nullptr,
		DirectX::Colors::White,
		0.0f,
		DirectX::XMFLOAT2(30, 10),
		0.0f
	);
	spriteBatch.get()->End();
}
