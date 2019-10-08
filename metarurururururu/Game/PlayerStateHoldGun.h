#pragma once
#include "PlayerState.h"
/// <summary>
/// 銃を構えている状態。
/// </summary>
class PlayerStateHoldGun : public IPlayerState
{
public:
	PlayerStateHoldGun();
	~PlayerStateHoldGun();
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
	virtual bool IsPossibleHoldGun()
	{
		return true;
	}
	//移動により回転するかどうか。
	virtual bool IsRotateByMove()
	{
		return false;
	}
};

