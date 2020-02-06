#pragma once
#include "FontRender.h"
#include "Sprite.h"
#include "Fade.h"

class BeforeTheGame : public IGameObject
{
public:
	/// <summary>
	///	コンストラクタ。 
	/// </summary>
	BeforeTheGame();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~BeforeTheGame();
	/// <summary>
	/// 開始関数。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
	void Render2D()override;
private:
	Sprite		m_sprite;							//固定スプライト。
	Sprite		m_missionSprite;					//Mission!スプライト。
	Sprite		m_moziSprite1;						//文字スプライトメイン。
	Sprite		m_moziSprite2;						//文字スプライトサブ。
	CVector3	m_missionPos = CVector3::Zero();	//Mission!の座標。
	CVector3	m_mozi1Pos = CVector3::Zero();		//メインの座標。
	CVector3	m_mozi2Pos = CVector3::Zero();		//サブの座標。
	bool		m_mission = false;
	bool		m_sabOn = false;
	int			m_timer = 0;
	bool		m_OK = false;
	bool		m_SceneTransition = false;
	Fade*		m_fade = nullptr;
	bool		m_fadeOk = false;
	bool		m_newGame = false;
};

