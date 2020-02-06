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
	void Render2D()override;
private:
	Sprite		m_sprite;				//�X�v���C�g�B
	FontRender* m_fontRender = nullptr; //�t�H���g�B
	Player*     m_player = nullptr;		//�v���C���[�B
	bool		m_hit = false;			//�G�ɒe�ۂ���������
};

