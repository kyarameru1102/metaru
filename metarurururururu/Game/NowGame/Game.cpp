#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "DirectionLight.h"
#include <string>
#include "Ground.h"
#include "EnemyGeneralCommander.h"
#include "sensya.h"
#include "Title.h"
#include "C4.h"
#include "Bullet.h"
#include "Fade.h"
#include "Result.h"

Game* Game::m_game = nullptr;

Game::Game()
{
	m_bgm.Init(L"Assets/sound/wind1.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(0.3f);
}


Game::~Game()
{
	DeleteGOs("sensya");
	DeleteGO(m_player);
	DeleteGO(m_EGC);
	DeleteGOs("enemy");
	DeleteGO(m_ground);
	DeleteGOs("gameCamera");
	DeleteGOs("fpsCamera");
	DeleteGOs("bullet");
	DeleteGOs("c4");
	DeleteGO(m_skyModel);
	m_game = nullptr;
}

bool Game::Start()
{
	std::wstring p[10];

	//m_skyModel = NewGO<SkinModelRender>(0);
	//m_skyModel->Init(L"Assets/modelData/sky.cmo");
	//m_skyModel->SetScale({ 60.0f,60.0f,60.0f });

	//戦車の座標レベルデータをロード。
	m_level.Init(L"Assets/level/sensyaPos.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"sensya")) {
				m_sensya = NewGO<sensya>(0, "sensya");
				m_sensya->SetPosition(obiData.position);
				m_sensya->SetRotation(obiData.rotation);
				return true;
			}
			return 0;
		}
	);
	//敵キャラ以外のレベルデータをロード。
	m_level.Init(L"Assets/level/stage_test.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"unityChan")) {
				m_player = NewGO<Player>(0, "player");
				m_player->SetPosition(obiData.position);
				return true;
			}
			//ナビメッシュ生成前に地面の当たり判定があると困るので
			//地面だけ除去。
			else if (obiData.EqualObjectName(L"ground_test"))
			{
				return true;
			}
			return 0;
		}
	);
	//@todo for debug
#if 0
	////敵の総司令的存在を生成。
	m_EGC = NewGO<EnemyGeneralCommander>(0, "EnemyGeneralCommander");
	//敵キャラのレベルデータ。
	//敵生成時に同時にナビメッシュが生成される。
	m_level.Init(L"Assets/level/Enemy_level00.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.FindName(L"takatozin_")) {
				wchar_t last[3];
				size_t l = wcslen(obiData.name);
				wchar_t la = obiData.name[l - 2];
				last[0] = la;
				la = obiData.name[l - 1];
				last[1] = la;
				last[2] = L'\0';

				m_enemy = NewGO<Enemy>(0, "enemy");
				m_enemy->SetPosition(obiData.position);
				m_enemy->SetRotation(obiData.rotation);

				m_enemy->SetPathNum(last);

				m_EGC->PushBackEnemy(m_enemy);

				return true;
			}
			return 0;
		}
	);
#endif
	//ナビゲーションメッシュを生成した後に地面を生成。
	m_ground = NewGO<Ground>(0);
	//カメラ生成。
	m_gameCamera = NewGO<GameCamera>(1, "gameCamera");
	//カメラの方向とプレイヤーとの距離を決定。
	CVector3 direction = { -100.0f, 50.0f, -70.0f };
	direction.Normalize();
	direction *= 120.0f;
	m_gameCamera->Setdirection(direction);

	if (m_game == nullptr) {
		m_game = this;
	}
	return true;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		DeleteGO(this);
		NewGO<Title>(0);
	}
	if (m_player->GetDeath()) {
		m_CorD = en_death;
		m_timer++;
	}
	if (m_player->GetClear()) {
		m_CorD = en_clear;
		m_timer++;
	}
	if (m_timer == 50 && !m_fadeflg) {
		NewGO<Fade>(0, "fade");
		m_fadeflg = true;
		m_timer = 0;
	}
	//ゲームオーバーなら。
	if (m_fadeflg && m_timer == 50) {
		if (m_CorD == en_death) {
			DeleteGO(this);
			NewGO<Title>(0);
			m_timer = 1000;
		}
		else if (m_CorD == en_clear) {
			DeleteGO(this);
			Result* result = nullptr;
			result = NewGO<Result>(0);
			result->SetKillCount(m_player->GetPlayerKillCount());
		}
	}
	//m_skyModel->SetPosition({0.0f,0.0f,0.0f});
}
