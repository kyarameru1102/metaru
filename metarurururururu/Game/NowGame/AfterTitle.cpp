#include "stdafx.h"
#include "AfterTitle.h"
#include "Fade.h"
#include "Title.h"
#include "Game.h"


AfterTitle::AfterTitle()
{
	m_title = FindGO<Title>("title");
}


AfterTitle::~AfterTitle()
{
	int a = 0;
}

bool AfterTitle::Start()
{
	m_fade = NewGO<Fade>(0, "fade");
	return true;
}

void AfterTitle::Update()
{
	if (m_fade->GetSpriteAlpha() >= 0.9f && !m_DeleteOK) {
		NewGO<Game>(0, "Game");
		m_DeleteOK = true;
	}
	if (m_fade->GetSpriteAlpha() <= 0.1f && m_DeleteOK) {
		DeleteGO(this);
	}
}
