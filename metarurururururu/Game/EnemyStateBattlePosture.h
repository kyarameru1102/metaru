#pragma once
#include "EnemyState.h"
/// <summary>
/// í“¬‘Ô¨ó‘ÔB
/// </summary>
class EnemyStateBattlePosture : public IEnemyState
{
public:
	EnemyStateBattlePosture();
	~EnemyStateBattlePosture();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

