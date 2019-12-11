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

	//��Ԃ̍��W���x���f�[�^�����[�h�B
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
	//�G�L�����ȊO�̃��x���f�[�^�����[�h�B
	m_level.Init(L"Assets/level/stage_test.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"unityChan")) {
				Player* pl = NewGO<Player>(0, "player");
				pl->SetPosition(obiData.position);
				return true;
			}
			//�i�r���b�V�������O�ɒn�ʂ̓����蔻�肪����ƍ���̂�
			//�n�ʂ��������B
			else if (obiData.EqualObjectName(L"ground_test"))
			{
				return true;
			}
			return 0;
		}
	);
	//�G�̑��i�ߓI���݂𐶐��B
	EnemyGeneralCommander* EGC = NewGO<EnemyGeneralCommander>(0, "EnemyGeneralCommander");
	//�G�L�����̃��x���f�[�^�B
	//�G�������ɓ����Ƀi�r���b�V�������������B
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
	//�i�r�Q�[�V�������b�V���𐶐�������ɒn�ʂ𐶐��B
	NewGO<Ground>(0);
	//�J���������B
	m_gameCamera = NewGO<GameCamera>(1, "gameCamera");
	//�J�����̕����ƃv���C���[�Ƃ̋���������B
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
