#include "stdafx.h"
#include "C4.h"
#include "sensya.h"
#include "EnemyGeneralCommander.h"
#include "Player.h"
#include "Enemy.h"

C4::C4()
{
	m_player = FindGO<Player>("player");
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
	if (g_pad[0].IsPress(enButtonLB1) && g_pad[0].IsTrigger(enButtonB)) {
		if ((m_player->GetPosition() - m_position).Length() <= 200.0f) {
			m_player->OutDeath();
		}
		QueryGOs<Enemy>("enemy", [&](Enemy* enemy)
			{
				if ((enemy->GetPosition() - m_position).Length() <= 200.0f) {
					enemy->OutDeath();
				}
				return true;
			}
		);
		//îöî≠ÅB
		//SE
		CSoundSource* SE;
		SE = NewGO<CSoundSource>(0);
		SE->Init(L"Assets/sound/bomb.wav");
		SE->Play(false);
		DeleteGO(this);
	}
}
