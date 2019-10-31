#include "stdafx.h"
#include "AstarDebug.h"


AstarDebug::AstarDebug()
{
}


AstarDebug::~AstarDebug()
{
}

bool AstarDebug::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/takatozin.cmo");
	return true;
}

void AstarDebug::Update()
{
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_Quaternion);
	m_skinModelRender->SetScale(m_scale);
}
