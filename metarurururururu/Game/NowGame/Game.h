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
class ClearPoint;
class Game : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Game();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Game();
	/// <summary>
	/// 開始関数。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update();
private:
	GameCamera*			m_gameCamera = nullptr;
	Level				m_level;
	SkinModel			m_model;
	ShadowMap			m_shadowMap;				//シャドウマップ。
	sensya* m_sensya = nullptr;
	Player* m_player = nullptr;
	EnemyGeneralCommander* m_EGC = nullptr;
	Enemy* m_enemy = nullptr;
	Ground* m_ground = nullptr;
	ClearPoint* m_clearPoint = nullptr;
};

