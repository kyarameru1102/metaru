#pragma once
class Human : public IGameObject
{
public:
	/// <summary>
	/// ポジションを設定する関数。
	/// </summary>
	/// <param name="pos">
	/// 座標。
	/// </param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot) {
		m_rotation = rot;
	}
	/// <summary>
	/// ポジションを返してくる関数。
	/// </summary>
	/// <returns>
	/// 座標。
	/// </returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 残弾を表示する。
	/// </summary>
	/// <returns>
	/// ammo  残弾。
	/// </returns>
	int GetAmmo() const
	{
		return m_ammo;
	}
protected:
	SkinModel			m_model;									//スキンモデル。
	SkinModelRender*	m_skinModelRender = nullptr;				//スキンモデルレンダー。
	CVector3			m_moveSpeed = CVector3::Zero();				//移動速度及び移動方向。
	CVector3			m_position = CVector3::Zero();				//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();		//回転。
	bool				m_death = false;							//死んでいるかどうか。
	int					m_ammo = 30;								//残弾。
	int					m_shotTimer = 0;							//銃を撃つ時のタイマー。
	bool				m_shotTimerOn = false;						//タイマーを始めていいかどうか。
	int					m_reloadTimer = 130;						//リロードをするときのタイマー。
};