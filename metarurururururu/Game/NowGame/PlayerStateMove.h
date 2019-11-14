#pragma once
#include "PlayerState.h"
/// <summary>
/// 移動している状態。
/// </summary>
class PlayerStateMove : public IPlayerState
{
public:
	PlayerStateMove();
	~PlayerStateMove();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
	//ジャンプできるかどうか。
	virtual bool IsPossibleJump()
	{
		return true;
	}
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
		return true;
	}
	//銃を構えれるかどうか。
	virtual bool  IsPossibleHoldGun()
	{
		return true;
	}
	//移動により回転するかどうか。
	virtual bool IsRotateByMove()
	{
		return true;
	}
};

