#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"

UI::UI()
{
	m_sprite.Init(L"Assets/sprite/kaasoru.dds", 100.0f, 100.0f);
	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetPosition({ 450.0f,0.0f });
	m_damegeSprite.Init(L"Assets/sprite/damage.dds", 1280.0f, 720.0f);
	m_damegeSprite.DeltaAlpha(-1.0f);
	m_damegeSprite2.Init(L"Assets/sprite/damage2.dds", 1280.0f, 720.0f);
	m_damegeSprite2.DeltaAlpha(-1.0f);
}


UI::~UI()
{
	DeleteGO(m_fontRender);
}

bool UI::Start()
{
	Game* game = GetGame();
	m_player = game->GetPlayer();
	m_maxHP = m_player->GetHP();
	return true;
}

void UI::Update()
{
	Damage();

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
	m_damegeSprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	m_damegeSprite2.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void UI::Damage()
{
	m_HP = m_player->GetHP();
	m_HP /= m_maxHP;
	m_HP *= -1.0f;
	m_damegeSprite.SetAlpha(m_HP);
	if (m_player->GetHP() < m_maxHP) {
		m_damegeSprite2.DeltaAlpha(1.0f);
	}
	else {
		m_damegeSprite2.DeltaAlpha(-1.0f);
	}
}

void UI::Render2D()
{
	if (g_pad[0].IsPress(enButtonLB1) || g_pad[0].IsPress(enButtonLB2)) {
		m_sprite.Draw();
	}
	m_damegeSprite.Draw();
	m_damegeSprite2.Draw();
}