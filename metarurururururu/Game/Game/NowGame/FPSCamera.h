#pragma once
/// <summary>
/// 一人称視点のカメラ。
/// </summary>
class Player;
class FPSCamera : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	FPSCamera();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~FPSCamera();
	/// <summary>
	/// 開始関数。
	/// </summary>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
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
	CVector3	m_position = CVector3::Zero();
	Player*		m_player = nullptr;
	CVector3	m_direction;					//向き。
};

