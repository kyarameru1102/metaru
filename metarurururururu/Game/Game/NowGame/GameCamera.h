#pragma once
#include "CameraCollisionSolver.h"
#include "SpringCamera.h"
/// <summary>
/// 三人称視点のカメラ。
/// </summary>
class Player;
class Enemy;
class ClearPoint;
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
	ClearPoint*				m_clearPoint = nullptr;
	CVector3				m_enemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };				//敵兵の座標。
	CVector3				m_direction;												//向き。
	float					m_rightLength;
	bool					m_LookInTo = false;											//肩越しのフラグ。
	CVector3				m_okEnemyPos = { FLT_MAX, FLT_MAX , FLT_MAX };
	bool					m_migi = false;
	bool					m_isLookOn = false;											//ロックオンしているかどうか。
	bool					m_heri = false;												//ヘリコプターのほうにカメラを向けるかどうか。
	CVector3				m_heriPos = CVector3::Zero();								//ヘリコプターの座標。
	int						m_heriTimer = 120;											//ヘリコプターを見ている時間。
	SpringCamera			m_springCamera;
};

