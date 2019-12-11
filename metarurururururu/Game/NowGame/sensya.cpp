#include "stdafx.h"
#include "sensya.h"


sensya::sensya()
{
}


sensya::~sensya()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_sensyaCollider);
}

bool sensya::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_sensyaCollider = NewGO<SkinModelRender>(0);

	m_skinModelRender->Init(L"Assets/modelData/sensya.cmo");
	m_sensyaCollider->Init(L"Assets/modelData/sensyaCollider.cmo");

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);

	m_sensyaCollider->SetPosition(m_position);
	m_sensyaCollider->SetRotation(m_rotation);

	m_sensyaCollider->SetRenderOn(false);

	

	return true;
}

void sensya::Update()
{
	/*m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);*/
}
