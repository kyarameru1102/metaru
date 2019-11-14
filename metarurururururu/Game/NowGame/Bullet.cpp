#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/bullet.cmo");
	m_player = FindGO<Player>("player");
	m_position = m_player->GetPosition();
	m_position.y += 70.0f;
	m_skinModelRender->SetPosition(m_position);
}


Bullet::~Bullet()
{
	
}

bool Bullet::Start()
{
	
	return true;
}

void Bullet::Update()
{
	m_position += m_moveSpeed;
	CVector3 toPlayer;
	toPlayer = m_position - m_player->GetPosition();
	m_toPlayerLen = toPlayer.Length();
	if (m_toPlayerLen > 15000.0f) {
		DeleteGO(m_skinModelRender);
		DeleteGO(this);
	}
	m_skinModelRender->SetPosition(m_position);
}
