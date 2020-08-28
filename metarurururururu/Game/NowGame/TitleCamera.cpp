#include "stdafx.h"
#include "TitleCamera.h"


TitleCamera::TitleCamera()
{
}


TitleCamera::~TitleCamera()
{
}


bool TitleCamera::Start()
{

	return true;
}

void TitleCamera::Update()
{
	g_camera3D.SetPosition(m_position);
	g_camera3D.SetTarget(m_target);
	//カメラの更新。
	g_camera3D.Update();
}