#include "stdafx.h"
#include "C4.h"


C4::C4()
{
}

C4::~C4()
{
}

bool C4::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/c4.cmo");

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
	return true;
}

void C4::Update()
{
}
