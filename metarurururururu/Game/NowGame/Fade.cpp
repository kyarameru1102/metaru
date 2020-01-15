#include "stdafx.h"
#include "Fade.h"


Fade::Fade()
{
}


Fade::~Fade()
{
}

bool Fade::Start()
{
	m_sprite.Init(L"Resource/sprite/fade.dds", 1280, 720);
	m_sprite.DeltaAlpha(-1.0f);
	return true;
}

void Fade::Update()
{
	if (m_sprite.GetAlpha() <= 0.9f && !m_fadeOutOK) {
		m_sprite.DeltaAlpha(0.02f);
	}
	else {
		m_fadeOutOK = true;
	}
	if (m_fadeOutOK && m_sprite.GetAlpha() <= 0.1f) {
		DeleteGO(this);
	}
	else if (m_fadeOutOK) {
		m_sprite.DeltaAlpha(-0.02f);
	}
	

	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void Fade::Render2D()
{
	m_sprite.Draw();
}
