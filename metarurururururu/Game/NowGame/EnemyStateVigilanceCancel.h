#pragma once
#include "EnemyState.h"
/// <summary>
/// �x���Ԑ����������Ă����ԁB
/// </summary>
class EnemyStateVigilanceCancel : public IEnemyState
{
public:
	EnemyStateVigilanceCancel();
	~EnemyStateVigilanceCancel();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

