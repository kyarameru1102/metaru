#include "stdafx.h"
#include "Clear.h"


Clear::Clear()
{
}


Clear::~Clear()
{
}

bool Clear::Start()
{
	m_sprite.Init(L"Resource/sprite/Clear.dds", 1280.0f, 720.0f);
	m_sprite.DeltaAlpha(-1.0f);
	return true;
}

void Clear::Update()
{
	m_sprite.DeltaAlpha(0.04f);
	
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void Clear::Render2D()
{
	m_sprite.Draw();
}
