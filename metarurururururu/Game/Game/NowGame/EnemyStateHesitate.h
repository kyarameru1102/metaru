#pragma once
#include "EnemyState.h"
/// <summary>
/// œpœj‚µ‚Ä‚¢‚éó‘ÔB
/// </summary>
class EnemyStateHesitate : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

