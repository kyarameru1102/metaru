#pragma once
#include "Sprite.h"
#include "sound/SoundSource.h"


class Title : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Title();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Title();
	/// <summary>
	/// 開始関数。
	/// </summary>
	/// <returns></returns>
	bool Start()override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update()override;
	void Render()override;
private:
	Sprite			m_sprite;				//スプライト。
	bool			m_on = false;			//Aボタンが押された時のフラグ。
	int				m_count = 0;
	CSoundSource	m_se;					//SE。
};

