#pragma once
#include "EnemyState.h"
/// <summary>
/// �퓬�Ԑ���ԁB
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

