#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
	m_player = FindGO<Player>("player");
	m_enemy = FindGO<Enemy>("enemy");
}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/bullet.cmo");
	m_position.y += 70.0f;
	return true;
}

void Bullet::Update()
{
	float randam = rand() / 10000.0f;
	m_moveSpeed *= 0.5 + randam;
	m_position += m_moveSpeed;
	if (m_havePlayer) {
		m_toPos = m_position - m_player->GetPosition();
	}
	if (m_haveEnemy)  {
		m_toPos = m_position - m_enemy->GetPosition();
		}
	m_toPlayerLen = m_toPos.Length();
	if (m_toPlayerLen > 15000.0f) {
		DeleteGO(this);
	}
	CVector3 Vector = m_moveSpeed;
	Vector.Normalize();
	CQuaternion Rot = CQuaternion::Identity();
	float kakudo = acos(Vector.Dot(CVector3::AxisY()));
	if (kakudo > 0.0f || kakudo < -FLT_MIN)
	{
		kakudo = CMath::RadToDeg(kakudo);
		CVector3 jiku;
		jiku.Cross(CVector3::AxisY(), Vector);
		if (kakudo > 0.0f || kakudo < -FLT_MIN)
		{
			jiku.Normalize();
			Rot.SetRotationDeg(jiku, kakudo);
		}

	}
	m_skinModelRender->SetRotation(Rot);
	m_skinModelRender->SetScale({ 10.0f,10.0f,10.0f });
	m_skinModelRender->SetPosition(m_position);
}
