#pragma once
/// <summary>
/// �v���C���[�̃X�e�[�g�p�^�[��
/// </summary>
class IPlayerState	{
public:
	virtual void Enter() = 0;				//�؂�ւ�����B
	virtual void Leave() = 0;				//�I������B
	virtual void Update() = 0;				//�p�����B
	virtual bool IsInvincible() = 0;		//���G���ǂ����B
	virtual bool IsPossibleMove() = 0;		//�����邩�ǂ����B
	virtual bool IsPossibleGunShoot() = 0;	//�e�����Ă邩�ǂ����B
	virtual bool IsPossibleHoldGun() = 0;	//�e���\����邩�ǂ����B
	virtual bool IsRotateByMove() = 0;		//�L�����N�^�[���ړ��ɂ���]���邩�B
};