#pragma once
#include "EnemyState.h"
/// <summary>
/// 警戒している状態。
/// </summary>
class EnemyStateVigilance : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

