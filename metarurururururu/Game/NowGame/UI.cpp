#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"

UI::UI()
{
}


UI::~UI()
{
	DeleteGO(m_fontRender);
}

bool UI::Start()
{
	Game* game = GetGame();
	m_player = game->GetPlayer();
	m_sprite.Init(L"Assets/sprite/kaasoru.dds", 100, 100);
	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetPosition({ 450.0f,0.0f });
	return true;
}

void UI::Update()
{
	if (m_sprite.GetAlpha() <= 0.5f) {
		m_sprite.DeltaAlpha(0.5f);
	}
	int ammo = m_player->GetAmmo();
	wchar_t zan[10];
	swprintf_s(zan, L"%d/30", ammo);
	m_fontRender->SetText(zan);
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		if (enemy->GetDamage())
		{
			m_sprite.DeltaAlpha(-0.5f);
		}
		return true;
	});
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void UI::Render2D()
{
	m_sprite.Draw();
}