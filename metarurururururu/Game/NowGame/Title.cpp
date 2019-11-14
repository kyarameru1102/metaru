#include "stdafx.h"
#include "Title.h"


Title::Title()
{
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(g_graphicsEngine->GetD3DDeviceContext());
	DirectX::CreateDDSTextureFromFile(
		g_graphicsEngine->GetD3DDevice(),
		L"Resource/sprite/Title.dds",
		nullptr,
		&shaderResourceView
	);
}


Title::~Title()
{
}

bool Title::Start()
{
	return true;
}

void Title::Update()
{
	if (g_pad->IsTrigger(enButtonA)) {
		NewGO<Game>(0, "Game");
		DeleteGO(this);
	}
}

void Title::Render()
{
	spriteBatch.get()->Begin();
	spriteBatch.get()->Draw(
		shaderResourceView,
		DirectX::XMFLOAT2(0.0f, 0.0f)
	);
	spriteBatch.get()->End();
}
