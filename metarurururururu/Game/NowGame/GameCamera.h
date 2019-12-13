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
	/// デストラクタより先に呼ばれる関数。
	/// DeleteGO関数が呼ばれたタイミングで呼ばれる関数。
	/// </summary>
	void OnDestroy() override;
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
	Enemy*					m_enemy = nullptr;
	CVector3				m_direction;					//向き。
	CameraCollisionSolver	m_cameraCollider;				//カメラのコライダー。
	float					m_rightLength;
	CameraCollisionSolver	m_targetCollider;				//注視点のコライダー
	bool					m_lockOn = false;					//ロックオンのフラグ。
};

