#include "stdafx.h"
#include "TargetDestruction.h"


TargetDestruction::TargetDestruction()
{
}


TargetDestruction::~TargetDestruction()
{
	
}

bool TargetDestruction::Start()
{
	m_sprite.Init(L"Assets/sprite/SuccessfulDestructionOfTarget.dds", 1280.0f, 720.0f);
	m_sprite.DeltaAlpha(-1.0f);
	return true;
}

void TargetDestruction::Update()
{
	if (m_timer >= 140) {
		DeleteGO(this);
	}
	m_timer++;
	if (m_timer <= 50) {
		m_sprite.DeltaAlpha(0.02f);
	}
	else if(m_timer >= 90){
		m_sprite.DeltaAlpha(-0.02f);
	}

	m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void TargetDestruction::Render2D()
{
	m_sprite.Draw();
}