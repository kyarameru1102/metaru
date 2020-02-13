#pragma once
#include "PlayerState.h"
/// <summary>
/// 伏せているステート。
/// </summary>
class PlayerCreepState : public IPlayerState
{
public:
	PlayerCreepState();
	~PlayerCreepState();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();

	//無敵かどうか。
	virtual bool IsInvincible()
	{
		return false;
	}
	//動けるかどうか。
	virtual bool IsPossibleMove()
	{
		return true;
	}
	//銃が撃てるかどうか。
	virtual bool IsPossibleGunShoot()
	{
		return false;
	}
	//銃を構えれるかどうか。
	virtual bool IsPossibleHoldGun()
	{
		return false;
	}
	//移動により回転するかどうか。
	virtual bool IsRotateByMove()
	{
		return true;
	}

};

