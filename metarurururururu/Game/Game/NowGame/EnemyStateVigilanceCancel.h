#pragma once
#include "EnemyState.h"
/// <summary>
/// Œx‰ú‘Ô¨‚ğ‰ğœ‚µ‚Ä‚¢‚éó‘ÔB
/// </summary>
class EnemyStateVigilanceCancel : public IEnemyState
{
public:
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
};

