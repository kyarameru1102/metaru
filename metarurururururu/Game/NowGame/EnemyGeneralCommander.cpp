#include "stdafx.h"
#include "EnemyGeneralCommander.h"



EnemyGeneralCommander::EnemyGeneralCommander()
{
}


EnemyGeneralCommander::~EnemyGeneralCommander()
{
}

bool EnemyGeneralCommander::Start()
{
	return true;
}

void EnemyGeneralCommander::Update()
{
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		if (enemy->GetCurrentstate() == &EnemyState::m_battlePosture || m_subjectDestroyed)
		{
			m_combatStart = true;
		}
		if (m_combatStart) {
			enemy->ChangeState(&EnemyState::m_battlePosture);
		}
		return true;
	});
		
	
	
}
