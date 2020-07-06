#include "stdafx.h"
#include "BeforeTheGame.h"



BeforeTheGame::BeforeTheGame()
{
}


BeforeTheGame::~BeforeTheGame()
{
}

bool BeforeTheGame::Start()
{
	m_sprite.Init(L"Assets/sprite/black.dds", 1280, 720);
	//m_sprite.SetAlpha(0.0f);
	
	m_missionSprite.Init(L"Assets/sprite/Mission.dds", 1280, 200);
	m_missionPos.x = 1500.0f;
	m_missionPos.y = 200.0f;
	m_missionSprite.Update(m_missionPos, CQuaternion::Identity(), CVector3::One());

	m_moziSprite1.Init(L"Assets/sprite/mozi.dds", 1280, 500);
	m_mozi1Pos.y = -100.0f;
	m_moziSprite1.DeltaAlpha(-1.0f);

	m_moziSprite2.Init(L"Assets/sprite/mozi2.dds", 1280, 100);
	m_mozi2Pos.y = -250.0f;
	m_moziSprite2.DeltaAlpha(-1.0f);

	return true;
}

void BeforeTheGame::Update()
{
	if (m_missionPos.x >= 10.0f) {
		m_missionPos.x -= 20.0f;
		m_mission = false;
	}
	else {
		m_mission = true;
	}
	if (m_mission && !m_sabOn) {
		m_moziSprite1.DeltaAlpha(0.02f);
	}
	if (m_moziSprite1.GetAlpha() >= 1.0f) {
		m_sabOn = true;
	}
	if (m_sabOn && !m_OK) {
		m_moziSprite2.DeltaAlpha(0.1f);
	}
	if (m_moziSprite2.GetAlpha() >= 1.0f) {
		m_OK = true;
	}
	if (m_OK) {
		if (g_pad[0].IsPressAnyKey()) {
			m_SceneTransition = true;
		}
	}
	if (m_SceneTransition) {
		if (!m_fadeOk) {
			m_fade = NewGO<Fade>(0);
		}
		m_fadeOk = true;
	}
	if (m_fadeOk) {
		m_moziSprite1.DeltaAlpha(-0.01f);
		m_sprite.DeltaAlpha(-0.01f);
		m_missionSprite.DeltaAlpha(-0.01f);
		m_moziSprite2.DeltaAlpha(-0.01f);
		if (m_fade->GetSpriteAlpha() >= 0.98f) {
			m_newGame = true;
		}
	}
	if (m_newGame) {
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	m_moziSprite2.Update(m_mozi2Pos, CQuaternion::Identity(), CVector3::One());
	m_missionSprite.Update(m_missionPos, CQuaternion::Identity(), CVector3::One());
	m_moziSprite1.Update(m_mozi1Pos, CQuaternion::Identity(), CVector3::One());
}

void BeforeTheGame::Render2D()
{	
	m_sprite.Draw();
	m_moziSprite2.Draw();
	m_missionSprite.Draw();
	m_moziSprite1.Draw();
}
