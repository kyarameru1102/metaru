#pragma once

class Player;
class Clear;
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
	CVector3 GetHeriPos() const
	{
		return m_heriPos;
	}
	bool isFirstTime()
	{
		return m_isFirstTime;
	}
	void DidIt()
	{
		m_isFirstTime = true;
	}
private:
	CVector3			m_position = CVector3::Zero();
	Level				m_level;
	Player*				m_player = nullptr;
	Clear*				m_clear = nullptr;
	CVector3			m_heriPos = CVector3::Zero();
	SkinModelRender*	m_skinModelRender = nullptr;
	bool				m_isFirstTime = false;
	enum EnAnimationClip {
		enAnimationClip_fry,											//飛ぶ。
		enAnimationClip_Num,
	};
	AnimationClip			m_animClips[enAnimationClip_Num];			//アニメーションクリップ。
};

