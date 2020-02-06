#pragma once
#include "BeforeTheGame.h"

class Fade;
class Title;
class AfterTitle : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	AfterTitle();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~AfterTitle();
	/// <summary>
	/// �J�n�֐�
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update() override;
private:
	Fade* m_fade = nullptr;			//�t�F�[�h�B
	Title* m_title = nullptr;		//�^�C�g���B
	bool m_DeleteOK = false;		//true�Ȃ�f���[�g���Ă����v�B
};

