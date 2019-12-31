#include "stdafx.h"
#include "Title.h"


Title::Title()
{
}


Title::~Title()
{

}

bool Title::Start()
{
	m_sprite.Init(L"Resource/sprite/Title.dds", 1400, 800);
	return true;
}

void Title::Update()
{
	if (g_pad->IsTrigger(enButtonA)) {
		NewGO<Game>(0, "Game");
		DeleteGO(this);
	}
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void Title::Render2D()
{
	m_sprite.Draw();
}
