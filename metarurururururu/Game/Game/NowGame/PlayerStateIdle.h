#pragma once
#include "PlayerState.h"
/// <summary>
/// �ҋ@��ԁB
/// </summary>
class PlayerStateIdle : public IPlayerState
{
public:
	PlayerStateIdle();
	~PlayerStateIdle();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
	//���G���ǂ����B
	virtual bool IsInvincible()
	{
		return false;
	}
	//�����邩�ǂ����B
	virtual bool IsPossibleMove()
	{
		return true;
	}
	//�e�����Ă邩�ǂ����B
	virtual bool IsPossibleGunShoot()
	{
		return true;
	}
	//�e���\����邩�ǂ����B
	virtual bool  IsPossibleHoldGun()
	{
		return true;
	}
	//�ړ��ɂ���]���邩�ǂ����B
	virtual bool IsRotateByMove()
	{
		return true;
	}
};

