#include "stdafx.h"
#include "GameOver.h"


GameOver::GameOver()
{
}


GameOver::~GameOver()
{
}

bool GameOver::Start()
{
	m_sprite.Init(L"Assets/sprite/GameOver.dds", 1280.0f, 720.0f);
	m_sprite.DeltaAlpha(-1.0f);
	return true;
}

void GameOver::Update()
{
	m_timer++;
	m_sprite.DeltaAlpha(0.04f);

	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
	if (m_timer == 100) {
		m_sprite.DeltaAlpha(-1.00f);
		DeleteGO(this);
	}
}

void GameOver::Render2D()
{
	m_sprite.Draw();
}
