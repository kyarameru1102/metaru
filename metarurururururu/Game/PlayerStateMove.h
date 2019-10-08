#pragma once
#include "PlayerState.h"
/// <summary>
/// �ړ����Ă����ԁB
/// </summary>
class PlayerStateMove : public IPlayerState
{
public:
	PlayerStateMove();
	~PlayerStateMove();
	virtual void Enter();
	virtual void Leave();
	virtual void Update();
	//�W�����v�ł��邩�ǂ����B
	virtual bool IsPossibleJump()
	{
		return true;
	}
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

