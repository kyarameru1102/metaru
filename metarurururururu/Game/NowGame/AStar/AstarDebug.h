#pragma once
class AstarDebug : public IGameObject
{
public:
	AstarDebug();
	~AstarDebug();
	bool Start()override;
	void Update()override;
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetScale(CVector3 scl)
	{
		m_scale = scl;
	}
	void SetQuaternion(CQuaternion scl0)
	{
		m_Quaternion = scl0;
	}
private:
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_Quaternion = CQuaternion::Identity();
	SkinModelRender* m_skinModelRender = nullptr;
};

