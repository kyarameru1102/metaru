#include "stdafx.h"
#include "UI.h"
#include "Player.h"

UI::UI()
{
}


UI::~UI()
{
	
}

bool UI::Start()
{
	Game* game = GetGame();
	m_player = game->GetPlayer();
	m_sprite.Init(L"Assets/sprite/kaasoru.dds", 100, 100);
	m_fontRender = NewGO<FontRender>(0);
	return true;
}

void UI::Update()
{
	int ammo = m_player->GetAmmo();
	wchar_t zan[10];
	swprintf_s(zan, L"%d/30", ammo);
	m_fontRender->SetText(zan);
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void UI::Render2D()
{
	m_sprite.Draw();
}

void UI::OnDestroy()
{
	DeleteGO(m_fontRender);
}
