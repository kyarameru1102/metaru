#pragma once
#include<queue>

class GameTime
{
public:
	GameTime();
	~GameTime();
	//インスタンスを取得する関数。
	static GameTime& GetInstance()
	{
		static GameTime t;
		return t;
	}
	const float GetFrameDeltaTime() const
	{
		return 1.0/30.0f;
	}
	void PushFrameDeltaTime(float deltaTime)
	{
		m_frameDeltaTimeQue.push_back(deltaTime);
		if (m_frameDeltaTimeQue.size() > 30.0f) {
			float totalTime = 0.0f;
			for (auto time : m_frameDeltaTimeQue) {
				totalTime += time;
			}
			//平均値をとる。
			m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
			m_frameDeltaTimeQue.pop_front();
		}
	}
private:
	std::list<float> m_frameDeltaTimeQue;
	float		m_frameDeltaTime = 1.0f / 60.0f;		//1フレームの経過時間。
};
static GameTime& GameTime()
{
	return GameTime::GetInstance();
}

