#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/dangan.cmo");
	return true;
}

void Bullet::Update()
{
	m_position += m_moveSpeed;
	m_skinModelRender->SetPosition(m_position);
}
