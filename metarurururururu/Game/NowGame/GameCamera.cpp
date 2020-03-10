#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "ClearPoint.h"

GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
	m_target = CVector3::Zero();
	g_camera3D.SetTarget(m_target);
	g_camera3D.Update();
}

bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");
	//m_cameraCollider.Init(5.0f);
	m_springCamera.Init(
		g_camera3D,
		1000.0f,
		true,
		10.0f
	);
	m_targetCollider.Init(20.0f);
	m_rightLength = -40.0f;
	return true;
}

void GameCamera::Update()
{
	if (FindGO<ClearPoint>("clearPoint") != nullptr && m_clearPoint == nullptr) {
		m_clearPoint = FindGO<ClearPoint>("clearPoint");
		if (!m_clearPoint->isFirstTime()) {
			m_heri = true;
			m_clearPoint->DidIt();
		}
	}
	if (g_pad[0].IsTrigger(enButtonLB3)) {
		m_rightLength *= -1;
		m_migi = true;
	}
	//CVector3 targetAdd = g_camera3D.GetRight() * m_rightLength;
	CVector3 camePos = CVector3::One();
	if (m_player != nullptr) {
		m_target = m_player->GetPosition();
	}
	
	m_target.y += 70.0f;
	CVector3 plpos = m_target;
	//CVector3 nowTarget = m_target;		いらん。
	
	//m_target += targetAdd;
	float x, y;
	if (m_LookInTo) {
		x = g_pad[0].GetRStickXF() * 1;
		y = g_pad[0].GetRStickYF() * 1;
	}
	else {
		x = g_pad[0].GetRStickXF() * 3;
		y = g_pad[0].GetRStickYF() * 3;
	}
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 2.0f * x);
	qRot.Multiply(m_direction);

	CVector3 toCameraPosOld = m_direction;

	CVector3 axisX;
	axisX.Cross(CVector3::AxisY(), m_direction);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_direction);

	CVector3 toPosDir = m_direction;
	toPosDir.Normalize();

	if (toPosDir.y < -0.8f) {
		m_direction = toCameraPosOld;
	}
	if (toPosDir.y > 0.8f) {
		m_direction = toCameraPosOld;
	}
	
	camePos = m_target + m_direction;

	//カメラの右を計算
	CVector3 camRight;
	camRight.Cross(m_direction, CVector3::Up());
	camRight.Normalize();
	m_target += camRight * m_rightLength;
	camePos += camRight * m_rightLength;

	CVector3 targetNewPos;
	//ターゲットのコライダーの座標計算実行。
	//m_targetCollider.Execute(targetNewPos, m_target, nowTarget);　　　イラン。
	CVector3 NewPosition;
	//カメラのコライダーの座標計算実行。
	//m_cameraCollider.Execute(NewPosition, camePos, targetNewPos);
	//m_target = targetNewPos;											いらん。
	//へりきた。
	if (m_heri) {
		m_heriTimer--;
		m_heriPos = m_clearPoint->GetHeriPos();
		m_direction *= 10.0f;
		m_target = m_heriPos;
	}
	if (m_heriTimer <= 0) {
		m_heri = false;
	}
	
	m_springCamera.SetPosition(camePos);
	m_springCamera.SetTarget(m_target);
	m_springCamera.Update();
	CVector3 CameraToTarget = NewPosition - m_target;

	
	if (g_pad[0].IsPress(enButtonLB1))
	{
		m_okEnemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };
		CVector3	enemyPos = { FLT_MAX, FLT_MAX, FLT_MAX };
		m_direction.Normalize();
		m_direction *= 80.0f;
		CVector3 PlayerToEnemy;
		bool hit = false;
		QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
			PlayerToEnemy = m_player->GetPosition() - enemy->GetPosition();
			if (PlayerToEnemy.Length() <= 800.0f && !enemy->GetNotLookOn()) {
				if (m_enemyPos.Length() >= enemy->GetPosition().Length()) {
					m_enemyPos = enemy->GetPosition();
					m_okEnemyPos = m_player->GetPosition() - enemy->GetPosition();
					if (enemy->GetHit())
					{
						hit = true;
					}
				}
			}		
				return true;
		});
		if (m_okEnemyPos.Length() <= 800.0f && !hit) {
			isLookOn(NewPosition);
			m_isLookOn = true;
		}
		else {
			m_enemyPos = {FLT_MAX, FLT_MAX, FLT_MAX};
			m_isLookOn = false;
		}
		
		m_LookInTo = true;
	}
	else {
		m_direction.Normalize();
		m_direction *= 120.0f;
		m_LookInTo = false;
	}
	//銃の反動。
	if (g_pad[0].IsPress(enButtonRB2) && m_player->GetDangan() && !m_isLookOn) {
		float randam = rand() / 8000.0f;
		m_direction.y += -1.0f * randam;
	}
	//反動でカメラが上に向きすぎないように修正。
	if (toPosDir.y < -0.8f) {
		m_direction = toCameraPosOld;
	}
	
	
	////g_camera3D.SetPosition(NewPosition);
	////g_camera3D.SetTarget(targetNewPos);
	////g_camera3D.SetTarget(m_target);
	////カメラの更新。
	////g_camera3D.Update();
}

void GameCamera::isLookOn(CVector3 pos)
{
	m_enemyPos.y += 80.0f;
	m_target = m_target + m_enemyPos;
	m_target /= 2;
	m_direction = m_target - pos;
	m_direction.Normalize();
	m_direction *= -100.0f;
}

