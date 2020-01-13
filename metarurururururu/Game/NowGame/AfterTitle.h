#pragma once
class Fade;
class Title;
class AfterTitle : public IGameObject
{
public:
	AfterTitle();
	~AfterTitle();
	bool Start() override;
	void Update() override;
private:
	Fade* m_fade = nullptr;			//フェード。
	Title* m_title = nullptr;		//タイトル。
};

