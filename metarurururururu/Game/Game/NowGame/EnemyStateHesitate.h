#pragma once
#include "EnemyState.h"
/// <summary>
/// �p�j���Ă����ԁB
/// </summary>
class EnemyStateHesitate : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

