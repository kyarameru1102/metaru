#pragma once
#include "CameraCollisionSolver.h"

class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void Render() override;
	/// <summary>
	/// カメラの向きを設定する。
	/// </summary>
	/// <param name="direction">
	/// 向き。
	/// </param>
	void Setdirection(CVector3 direction)
	{
		m_direction = direction;
	}
	/// <summary>
	/// カメラの向いている方向を返す。
	/// </summary>
	/// <returns>
	/// 向き。
	/// </returns>
	CVector3 Getdirection()
	{
		return m_direction;
	}
private:
	CVector3				m_target = CVector3::Zero();
	Player*					m_player = nullptr;
	CVector3				m_direction;					//向き。
	CameraCollisionSolver	m_cameraCollider;				//カメラのコライダー。
	float					m_rightLength;
	CameraCollisionSolver	m_targetCollider;				//注視点のコライダー
};

