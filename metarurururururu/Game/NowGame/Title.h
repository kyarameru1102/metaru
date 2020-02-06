#pragma once
#include "Sprite.h"
#include "sound/SoundSource.h"


class Title : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Title();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Title();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	/// <returns></returns>
	bool Start()override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update()override;
	void Render()override;
private:
	Sprite			m_sprite;				//�X�v���C�g�B
	bool			m_on = false;			//A�{�^���������ꂽ���̃t���O�B
	int				m_count = 0;
	CSoundSource	m_se;					//SE�B
};

