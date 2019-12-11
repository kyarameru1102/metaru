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


Game::Game()
{
}


Game::~Game()
{
}

bool Game::Start()
{
	std::wstring p[10];

	//戦車の座標レベルデータをロード。
	m_level.Init(L"Assets/level/sensyaPos.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"sensya")) {
				sensya* sen = NewGO<sensya>(0, "sensya");
				sen->SetPosition(obiData.position);
				sen->SetRotation(obiData.rotation);
				return true;
			}
			return 0;
		}
	);
	//敵キャラ以外のレベルデータをロード。
	m_level.Init(L"Assets/level/stage_test.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"unityChan")) {
				Player* pl = NewGO<Player>(0, "player");
				pl->SetPosition(obiData.position);
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
	//敵の総司令的存在を生成。
	EnemyGeneralCommander* EGC = NewGO<EnemyGeneralCommander>(0, "EnemyGeneralCommander");
	//敵キャラのレベルデータ。
	//敵生成時に同時にナビメッシュが生成される。
	m_level.Init(L"Assets/level/Enemy_level01.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.FindName(L"takatozin_")) {
				wchar_t last[3];
				size_t l = wcslen(obiData.name);
				wchar_t la = obiData.name[l - 2];
				last[0] = la;
				la = obiData.name[l - 1];
				last[1] = la;
				last[2] = L'\0';

				Enemy* en = NewGO<Enemy>(0, "enemy");
				en->SetPosition(obiData.position);

				en->SetPathNum(last);

				EGC->PushBackEnemy(en);

				return true;
			}
			return 0;
		}
	);
	//ナビゲーションメッシュを生成した後に地面を生成。
	NewGO<Ground>(0);
	//カメラ生成。
	m_gameCamera = NewGO<GameCamera>(1, "gameCamera");
	//カメラの方向とプレイヤーとの距離を決定。
	CVector3 direction = { 0.0f, 100.0f, 70.0f };
	direction.Normalize();
	direction *= 120.0f;
	m_gameCamera->Setdirection(direction);

	return true;
}

void Game::Update()
{
}

void Game::OnDestroy()
{
}

void Game::Render()
{
}
