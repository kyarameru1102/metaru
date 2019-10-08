#pragma once
#include "SphereCollider.h"
//三人称カメラの背景へのめりこみをなくすクラス。

class CameraCollisionSolver
{
public:
	CameraCollisionSolver();
	~CameraCollisionSolver();
	/// <summary>
	/// 初期化関数。
	/// </summary>
	/// <param name="radius">
	/// カメラにつけるスフィアコライダーの半径。
	/// </param>
	void Init(float radius);
	/// <summary>
	/// コリジョン解決の実行。
	/// </summary>
	/// <param name="result"> 
	/// コリジョン解決を行った座標が格納される。
	/// </param>
	/// <param name="position"> 
	/// コリジョン解決を行う前のカメラの座標。
	/// </param>
	/// <param name="target"> 
	/// カメラの注視点。
	/// </param>
	/// <returns> 
	/// コリジョン解決をおこなったらtrueを返す。
	/// </returns>
	bool Execute(CVector3& result, const CVector3& position, const CVector3& target);
private:
	SphereCollider			m_collider;			//コライダー。
	float					m_radius = 0.0f;	//半径。
};

