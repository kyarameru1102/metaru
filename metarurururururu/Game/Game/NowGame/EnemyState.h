#pragma once
/// <summary>
/// �G�l�~�[�̃X�e�[�g�p�^�[���B
/// </summary>
class IEnemyState	{
public:
	virtual void Enter() = 0;				//�؂�ւ�����B
	virtual void Leave() = 0;				//�I������B
	virtual void Update() = 0;				//�p�����B
};
