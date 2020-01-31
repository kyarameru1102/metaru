#include "stdafx.h"
#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

bool UI::Start()
{
	m_sprite.Init(L"Assets/sprite/kaasoru.dds", 100, 100);
	return true;
}

void UI::Update()
{
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void UI::Render2D()
{
	m_sprite.Draw();
}
