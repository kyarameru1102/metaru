#pragma once
#include "Sprite.h"
#include "FontRender.h"

class Player;
class UI : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	UI();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~UI();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �v���C���[���_���[�W���󂯂��Ƃ��̏����B
	/// </summary>
	void Damage();
	void Render2D()override;
private:
	Sprite		m_sprite;				//�J�[�\���p�X�v���C�g�B
	Sprite		m_damegeSprite;			//��e�����Ƃ��p�̃X�v���C�g�B			
	Sprite		m_damegeSprite2;		//��e�����Ƃ��p�̃X�v���C�g�Q�D
	FontRender* m_fontRender = nullptr; //�t�H���g�B
	Player*     m_player = nullptr;		//�v���C���[�B
	bool		m_hit = false;			//�G�ɒe�ۂ���������
	float		m_HP = 999999;
	float		m_maxHP = 999999;
	CVector3	m_fontColor = { 0.0f,0.0f,0.0f };
};

