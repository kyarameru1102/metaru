#pragma once
class TitleCamera : public IGameObject
{
public:
	bool Start() override;
	void Update() override;
	TitleCamera();
	~TitleCamera();
private:
	CVector3 m_position = CVector3(-1500.0f,50.0f,2300.0f);		//カメラの座標。
	CVector3 m_target = CVector3(-1800.0,150.0f,2600.0f);		//ターゲットの座標。
};

