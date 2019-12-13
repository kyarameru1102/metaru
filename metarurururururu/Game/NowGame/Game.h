#pragma once
#include "level/Level.h"
#include "DirectionLight.h"
#include "NaviMesh.h"

class GameCamera;
class sensya;
class Player;
class EnemyGeneralCommander;
class Enemy;
class Ground;
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
	ShadowMap			m_shadowMap;				//シャドウマップ。
	sensya* m_sensya = nullptr;
	Player* m_player = nullptr;
	EnemyGeneralCommander* m_EGC = nullptr;
	Enemy* m_enemy = nullptr;
	Ground* m_ground = nullptr;
};

