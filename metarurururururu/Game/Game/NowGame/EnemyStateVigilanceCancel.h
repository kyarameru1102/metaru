#pragma once
#include "EnemyState.h"
/// <summary>
/// �x���Ԑ����������Ă����ԁB
/// </summary>
class EnemyStateVigilanceCancel : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

