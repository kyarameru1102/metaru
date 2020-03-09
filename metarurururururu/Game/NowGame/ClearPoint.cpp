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
	DeleteGO(m_skinModelRender);
}

bool ClearPoint::Start()
{
	//脱出用のポイントの座標データをロード。
	m_level.Init(L"Assets/level/ClearPos.tkl",
		[&](LevelObjectData& obiData)->int {
			if (obiData.EqualObjectName(L"Box000")) {
				m_position = obiData.position;
				return true;
			}
			return 0;
		}
	);
	m_animClips[enAnimationClip_fry].Load(L"Assets/animData/heri_fry.tka");
	m_animClips[enAnimationClip_fry].SetLoopFlag(true);
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heri.cmo", m_animClips, enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->SetShadowReciever(true);
	m_heriPos = m_position;
	m_heriPos.y += 3000.0f;
	return true;
}

void ClearPoint::Update()
{

	if (m_player->GetDeath()) {
		DeleteGO(this);
	}
	if (m_heriPos.y > 0.0f) {
		m_heriPos.y -= 10.0f;
	}
	CVector3 toPlayer;
	toPlayer = m_player->GetPosition() - m_position;
	float ppp = toPlayer.Length();
	if (toPlayer.Length() <= 300.0f) {
		if (m_clear == nullptr) {
			m_clear = NewGO<Clear>(0, "clear");
		}
		m_player->SetClearFlag(true);
	}
	if (m_clear != nullptr) {
		if (m_clear->GetAlpha() <= 0.0f) {
			DeleteGO(this);
		}
	}
	m_skinModelRender->PlayAnimation(enAnimationClip_fry, Body::enWholeBody, 0.3);
	m_skinModelRender->SetPosition(m_heriPos);
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });
}
