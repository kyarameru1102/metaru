#pragma once
#include <vector>
#include "Enemy.h"
#include "EnemyStateHesitate.h"
#include "EnemyStateVigilance.h"
#include "EnemyStateVigilanceCancel.h"
#include "EnemyStateBattlePosture.h"

class Enemy;
class EnemyGeneralCommander : public IGameObject
{
public:
	EnemyGeneralCommander();
	~EnemyGeneralCommander();
	bool Start() override;
	void Update() override;
	void PushBackEnemy(Enemy* enemy)
	{
		m_enemyArray.push_back(enemy);
	}
private:
	std::vector<Enemy*>			m_enemyArray;								//“G•º‚Ì”z—ñB
	bool						m_combatStart = false;						//í“¬‘Ô¨‚©‚Ç‚¤‚©B				
};

