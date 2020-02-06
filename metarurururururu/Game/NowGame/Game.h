#pragma once
#include "level/Level.h"
#include "DirectionLight.h"
#include "NaviMesh.h"
#include "sound/SoundSource.h"

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
	/// �R���X�g���N�^�B
	/// </summary>
	Game();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Game();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update();
	static Game* GetGame() {
		return m_game;
	}
	Player* GetPlayer() const {
		return m_player;
	}
	sensya* GetSensya() const {
		return m_sensya;
	}
	EnemyGeneralCommander* GetEGC() const {
		return m_EGC;
	}
	Enemy* GetEnemy() const {
		return m_enemy;
	}

private:
	static Game*			m_game;
	GameCamera*				m_gameCamera = nullptr;
	Level					m_level;
	SkinModel				m_model;
	ShadowMap				m_shadowMap;				//�V���h�E�}�b�v�B
	sensya*					m_sensya = nullptr;
	Player*					m_player = nullptr;
	EnemyGeneralCommander*	m_EGC = nullptr;
	Enemy*					m_enemy = nullptr;
	Ground*					m_ground = nullptr;
	ClearPoint*				m_clearPoint = nullptr;
	int						m_timer = 0;
	bool					m_fadeflg = false;
	CSoundSource			m_bgm;						//BGM�B
	CSoundSource			m_se;						//SE�B
};

static inline Game* GetGame() {
	return Game::GetGame();
}
