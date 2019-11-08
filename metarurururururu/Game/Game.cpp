#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "DirectionLight.h"
#include <string>
#include "Ground.h"


Game::Game()
{
}


Game::~Game()
{
	//DeleteGO(m_dirLight);
}

bool Game::Start()
{
	std::wstring p[10];

	//敵キャラ以外のレベルデータをロード。
	m_level.Init(L"Assets/level/stage_test.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"unityChan")) {
				Player* pl = NewGO<Player>(0, "player");
				pl->SetPosition(obiData.position);
				return true;
			}
			else if (obiData.EqualObjectName(L"ground_test"))
			{
				return true;
			}
			return 0;
		}
	);
	//敵キャラのレベルデータ。
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

				return true;
			}
			return 0;
		}
	);
	//ナビゲーションメッシュを生成した後に地面を生成。
	NewGO<Ground>(0);

	m_gameCamera = NewGO<GameCamera>(1, "gameCamera");
	//カメラの方向とプレイヤーとの距離を決定。
	CVector3 direction = { 0.0f, 150.0f, 100.0f };
	m_gameCamera->Setdirection(direction);

	

	return true;
}

void Game::Update()
{
	if (g_pad[0].IsTrigger(enButtonA)) {
	//	m_navi.Create(m_model);
	}
}

void Game::OnDestroy()
{
}

void Game::Render()
{
	////シャドウマップ。
	//auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	////現在のレンダリングターゲットをバックアップしておく。
	//ID3D11RenderTargetView* oldRenderTargetView;
	//ID3D11DepthStencilView* oldDepthStencilView;
	//d3dDeviceContext->OMGetRenderTargets(
	//	1,
	//	&oldRenderTargetView,
	//	&oldDepthStencilView
	//);
	////ビューポートもバックアップを取っておく。
	//unsigned int numViewport = 1;
	//D3D11_VIEWPORT oldViewports;
	//d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);
	////シャドウマップにレンダリング。
	//m_shadowMap.RenderToShadowMap();

	////元に戻す。
	//d3dDeviceContext->OMSetRenderTargets(
	//	1,
	//	&oldRenderTargetView,
	//	oldDepthStencilView
	//);
	//d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	////レンダリングターゲットとデプスステンシルの参照カウンタを下す。
	//oldRenderTargetView->Release();
	//oldDepthStencilView->Release();

}
