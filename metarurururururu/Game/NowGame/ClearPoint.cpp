#include "stdafx.h"
#include "ClearPoint.h"
#include "Player.h"
#include "Clear.h"


ClearPoint::ClearPoint()
{
	m_player = FindGO<Player>("player");
}


ClearPoint::~ClearPoint()
{
}

bool ClearPoint::Start()
{
	//脱出用のポイントの座標データをロード。
	m_level.Init(L"Assets/level/ClearPos.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"clearpos")) {
				m_position = obiData.position;
				return true;
			}
			return 0;
		}
	);
	return true;
}

void ClearPoint::Update()
{
	CVector3 toPlayer;
	toPlayer = m_player->GetPosition() - m_position;
	float ppp = toPlayer.Length();
	if (toPlayer.Length() <= 3000.0f) {
		NewGO<Clear>(0, "clear");
		m_player->SetClearFlag(true);
		DeleteGO(this);
	}
}
