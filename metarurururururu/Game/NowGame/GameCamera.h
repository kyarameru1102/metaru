#pragma once
#include "CameraCollisionSolver.h"
/// <summary>
/// 三人称視点のカメラ。
/// </summary>
class Player;
class Enemy;
class GameCamera : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	GameCamera();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~GameCamera();
	/// <summary>
	/// 開始関数。
	/// </summary>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
	/// <summary>
	/// ロックオンしているときの関数。
	/// </summary>
	/// <param name="pos">カメラのポジション。</param>
	void isLookOn(CVector3 pos);
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
	CVector3 Getdirection() const
	{
		return m_direction;
	}
private:
	CVector3				m_target = CVector3::Zero();
	Player*					m_player = nullptr;
	CVector3				m_enemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };						//敵兵の座標。
	CVector3				m_direction;					//向き。
	CameraCollisionSolver	m_cameraCollider;				//カメラのコライダー。
	float					m_rightLength;
	CameraCollisionSolver	m_targetCollider;				//注視点のコライダー
	bool					m_LookInTo = false;				//ロックオンのフラグ。
	CVector3				m_okEnemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };
	bool					m_migi = false;
};

