#include "stdafx.h"
#include "UI.h"


UI::UI()
{
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(g_graphicsEngine->GetD3DDeviceContext());
	DirectX::CreateDDSTextureFromFile(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/sprite/kaasoru.dds",
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
	/*spriteBatch.get()->Begin();
	spriteBatch.get()->Draw(
		shaderResourceView,
		DirectX::XMFLOAT2(0.0f, 0.0f)
	);
	spriteBatch.get()->End();*/
}
