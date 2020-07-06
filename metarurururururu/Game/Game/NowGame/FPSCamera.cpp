#include "stdafx.h"
#include "FPSCamera.h"
#include "Player.h"


FPSCamera::FPSCamera()
{
}


FPSCamera::~FPSCamera()
{
}

bool FPSCamera::Start()
{
	g_camera3D.SetNear(12.4f);
	m_player = FindGO<Player>("player");
	return true;
}

void FPSCamera::Update()
{
	m_position = m_player->GetPosition();
	m_position.y += 85.0f;

	float x = g_pad[0].GetRStickXF() * 0.5;
	float y = g_pad[0].GetRStickYF() * 0.5;

	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 2.0f * x);
	qRot.Multiply(m_direction);

	CVector3 directionOld = m_direction;

	CVector3 axisX;
	axisX.Cross(CVector3::AxisY(), m_direction);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * -y);
	qRot.Multiply(m_direction);

	CVector3 toPosDir = m_direction;
	toPosDir.Normalize();
	//反動処理。
	if (g_pad[0].IsPress(enButtonRB2) && m_player->GetDangan()) {
		m_direction.y += 1.0f;
	}
	if (toPosDir.y < -0.8f) {
		m_direction = directionOld;
	}
	if (toPosDir.y > 0.8f) {
		m_direction = directionOld;
	}


	CVector3 target = m_position + m_direction;

	g_camera3D.SetPosition(m_position);
	g_camera3D.SetTarget(target);
	//カメラの更新。
	g_camera3D.Update();
}
