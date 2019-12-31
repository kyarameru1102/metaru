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
	for (auto& enemy : m_enemyArray) {
		if (enemy->GetCurrentstate() == &EnemyState::m_battlePosture || m_subjectDestroyed)
		{
			m_combatStart = true;
		}
	}
	if (m_combatStart) {
		for (auto& enemy : m_enemyArray) {
			enemy->ChangeState(&EnemyState::m_battlePosture);
		}
	}
}
