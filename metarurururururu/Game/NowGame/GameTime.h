#pragma once
#include<queue>

class GameTime
{
public:
	GameTime();
	~GameTime();
	//�C���X�^���X���擾����֐��B
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
			//���ϒl���Ƃ�B
			m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
			m_frameDeltaTimeQue.pop_front();
		}
	}
private:
	std::list<float> m_frameDeltaTimeQue;
	float		m_frameDeltaTime = 1.0f / 60.0f;		//1�t���[���̌o�ߎ��ԁB
};
static GameTime& GameTime()
{
	return GameTime::GetInstance();
}

