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
	std::vector<Enemy*>			m_enemyArray;								//�G���̔z��B
	bool						m_combatStart = false;						//�퓬�Ԑ����ǂ����B	
	bool						m_subjectDestroyed = false;					//��Ԃ��j�󂳂ꂽ���ǂ����B
};

