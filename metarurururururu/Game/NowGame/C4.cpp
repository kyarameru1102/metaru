#include "stdafx.h"
#include "C4.h"
#include "sensya.h"
#include "EnemyGeneralCommander.h"

C4::C4()
{
}

C4::~C4()
{
	DeleteGO(m_skinModelRender);
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
	if (g_pad[0].IsPress(enButtonLB1) && g_pad[0].IsTrigger(enButtonY)) {
		//”š”­—\’èB
		DeleteGO(this);
	}
}
