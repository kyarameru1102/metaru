#pragma once
#include "EnemyState.h"
/// <summary>
/// Œx‰ú‚µ‚Ä‚¢‚éó‘ÔB
/// </summary>
class EnemyStateVigilance : public IEnemyState
{
public:
	EnemyStateVigilance();
	~EnemyStateVigilance();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

