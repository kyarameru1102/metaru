#pragma once
#include "EnemyState.h"
/// <summary>
/// �x�����Ă����ԁB
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

