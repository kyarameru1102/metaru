#pragma once
#include "BeforeTheGame.h"

class Fade;
class Title;
class AfterTitle : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	AfterTitle();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~AfterTitle();
	/// <summary>
	/// 開始関数
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
private:
	Fade* m_fade = nullptr;			//フェード。
	Title* m_title = nullptr;		//タイトル。
	bool m_DeleteOK = false;		//trueならデリートしても大丈夫。
};

