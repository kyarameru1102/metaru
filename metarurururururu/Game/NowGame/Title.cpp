#include "stdafx.h"
#include "Title.h"


Title::Title()
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(g_graphicsEngine->GetD3DDeviceContext());
	DirectX::CreateDDSTextureFromFile(
		g_graphicsEngine->GetD3DDevice(),
		L"Resource/sprite/Title.dds",
		nullptr,
		&m_shaderResourceView
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
	m_spriteBatch.get()->Begin();
	m_spriteBatch.get()->Draw(
		m_shaderResourceView,
		DirectX::XMFLOAT2(0.0f, 0.0f)
	);
	m_spriteBatch.get()->End();
}
