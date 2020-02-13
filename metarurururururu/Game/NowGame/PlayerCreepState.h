#pragma once
#include "PlayerState.h"
/// <summary>
/// �����Ă���X�e�[�g�B
/// </summary>
class PlayerCreepState : public IPlayerState
{
public:
	PlayerCreepState();
	~PlayerCreepState();
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
		return false;
	}
	//�e���\����邩�ǂ����B
	virtual bool IsPossibleHoldGun()
	{
		return false;
	}
	//�ړ��ɂ���]���邩�ǂ����B
	virtual bool IsRotateByMove()
	{
		return true;
	}

};

