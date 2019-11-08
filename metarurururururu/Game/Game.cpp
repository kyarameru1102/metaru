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

	//�G�L�����ȊO�̃��x���f�[�^�����[�h�B
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
	//�G�L�����̃��x���f�[�^�B
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
	//�i�r�Q�[�V�������b�V���𐶐�������ɒn�ʂ𐶐��B
	NewGO<Ground>(0);

	m_gameCamera = NewGO<GameCamera>(1, "gameCamera");
	//�J�����̕����ƃv���C���[�Ƃ̋���������B
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
	////�V���h�E�}�b�v�B
	//auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	////���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	//ID3D11RenderTargetView* oldRenderTargetView;
	//ID3D11DepthStencilView* oldDepthStencilView;
	//d3dDeviceContext->OMGetRenderTargets(
	//	1,
	//	&oldRenderTargetView,
	//	&oldDepthStencilView
	//);
	////�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	//unsigned int numViewport = 1;
	//D3D11_VIEWPORT oldViewports;
	//d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);
	////�V���h�E�}�b�v�Ƀ����_�����O�B
	//m_shadowMap.RenderToShadowMap();

	////���ɖ߂��B
	//d3dDeviceContext->OMSetRenderTargets(
	//	1,
	//	&oldRenderTargetView,
	//	oldDepthStencilView
	//);
	//d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	////�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̎Q�ƃJ�E���^�������B
	//oldRenderTargetView->Release();
	//oldDepthStencilView->Release();

}
