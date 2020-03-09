#include "stdafx.h"
#include "Result.h"
#include "Title.h"

Result::Result()
{
}


Result::~Result()
{
}

bool Result::Start()
{
	if (m_killCount <= 0) {
		m_sprite.Init(L"Assets/sprite/rankS.dds", 1280.0f, 720.0f);
	}
	else if (m_killCount <= 2) {
		m_sprite.Init(L"Assets/sprite/rankA.dds", 1280.0f, 720.0f);
	}
	else {
		m_sprite.Init(L"Assets/sprite/rankB.dds", 1280.0f, 720.0f);
	}
	m_sprite.SetAlpha(0.0f);
	return true;
}

void Result::Update()
{
	m_sprite.DeltaAlpha(0.02f);
	if (g_pad[0].IsPressAnyKey()) {
		DeleteGO(this);
		NewGO<Title>(0);
	}
	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}

void Result::Render2D()
{
	m_sprite.Draw();
}
