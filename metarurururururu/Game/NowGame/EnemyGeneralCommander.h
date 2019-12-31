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
	void SetSubjectDestroyed(bool subjectDestroyed)
	{
		m_subjectDestroyed = subjectDestroyed;
	}
private:
	std::vector<Enemy*>			m_enemyArray;								//敵兵の配列。
	bool						m_combatStart = false;						//戦闘態勢かどうか。	
	bool						m_subjectDestroyed = false;					//戦車が破壊されたかどうか。
};

