#pragma once
#include "level/Level.h"
#include "DirectionLight.h"
#include "NaviMesh.h"

class GameCamera;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void OnDestroy();
	void Render();
private:
	GameCamera*			m_gameCamera = nullptr;
	Level				m_level;
	//NaviMesh			m_navi;
	SkinModel			m_model;
};

