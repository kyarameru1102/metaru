#pragma once

class Player;
class ClearPoint : public IGameObject
{
public:
	ClearPoint();
	~ClearPoint();
	bool Start()override;
	void Update()override;
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
private:
	CVector3 m_position = CVector3::Zero();
	Level m_level;
	Player* m_player = nullptr;
};

