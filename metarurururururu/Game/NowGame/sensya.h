#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"

class Player;
class sensya : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	sensya();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~sensya();
	/// <summary>
	/// 開始関数。
	/// </summary>
	bool Start()override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update()override;
	/// <summary>
	/// 座標を設定する関数。
	/// </summary>
	/// <param name="pos">ポジション</param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 角度を設定する関数。
	/// </summary>
	/// <param name="rot">ローテーション</param>
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 座標を返す関数。
	/// </summary>
	/// <returns>ポジション</returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
private:
	MeshCollider m_meshCollider;									//メッシュコライダー。
	RigidBody m_rigidBody;											//剛体。
	SkinModelRender*	m_skinModelRender = nullptr;				//スキンモデルレンダー。
	SkinModelRender*	m_sensyaCollider = nullptr;					//戦車のコライダー用のスキンモデルレンダー。
	CVector3			m_position = CVector3::Zero();				//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();		//回転。
	CVector3			m_scl = CVector3::One();					//拡大率。
	Player*				m_player = nullptr;							//プレイヤー。
	bool				m_c4On = false;								//c4があるかどうかのフラグ。
};

