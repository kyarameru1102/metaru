#pragma once
#include "EnemyState.h"
/// <summary>
/// 警戒態勢を解除している状態。
/// </summary>
class EnemyStateVigilanceCancel : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

