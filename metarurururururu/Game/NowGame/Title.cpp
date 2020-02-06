#include "stdafx.h"
#include "Title.h"
#include "Fade.h"
#include "AfterTitle.h"


Title::Title()
{
	m_se.Init(L"Assets/sound/gunfire.wav");
}


Title::~Title()
{
}

bool Title::Start()
{
	m_sprite.Init(L"Assets/sprite/Break_Down_Tank.dds", 1400, 800);
	return true;
}

void Title::Update()
{
	if (g_pad->IsTrigger(enButtonA) && !m_on) {
		m_on = true;
		NewGO<AfterTitle>(0, "afterTitle");
		m_se.Play(false);
	}
	if (m_on) {
		m_count++;
		m_sprite.DeltaAlpha(-0.02f);
	}
	if (m_count >= 50) {
		DeleteGO(this);
	}
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void Title::Render()
{
	m_sprite.Draw();
}
