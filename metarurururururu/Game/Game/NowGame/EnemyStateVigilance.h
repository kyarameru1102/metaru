#pragma once
#include "EnemyState.h"
/// <summary>
/// �x�����Ă����ԁB
/// </summary>
class EnemyStateVigilance : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

