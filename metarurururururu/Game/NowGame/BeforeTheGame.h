#pragma once
#include "FontRender.h"
#include "Sprite.h"
#include "Fade.h"

class BeforeTheGame : public IGameObject
{
public:
	/// <summary>
	///	�R���X�g���N�^�B 
	/// </summary>
	BeforeTheGame();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~BeforeTheGame();
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
	Sprite		m_sprite;							//�Œ�X�v���C�g�B
	Sprite		m_missionSprite;					//Mission!�X�v���C�g�B
	Sprite		m_moziSprite1;						//�����X�v���C�g���C���B
	Sprite		m_moziSprite2;						//�����X�v���C�g�T�u�B
	CVector3	m_missionPos = CVector3::Zero();	//Mission!�̍��W�B
	CVector3	m_mozi1Pos = CVector3::Zero();		//���C���̍��W�B
	CVector3	m_mozi2Pos = CVector3::Zero();		//�T�u�̍��W�B
	bool		m_mission = false;
	bool		m_sabOn = false;
	int			m_timer = 0;
	bool		m_OK = false;
	bool		m_SceneTransition = false;
	Fade*		m_fade = nullptr;
	bool		m_fadeOk = false;
	bool		m_newGame = false;
};

