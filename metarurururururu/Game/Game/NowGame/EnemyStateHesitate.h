#pragma once
#include "EnemyState.h"
/// <summary>
/// 徘徊している状態。
/// </summary>
class EnemyStateHesitate : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

