#pragma once
class C4 : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	C4();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~C4();
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
private:
	SkinModelRender*	m_skinModelRender = nullptr;				//スキンモデルレンダー。
	CVector3			m_position = CVector3::Zero();				//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();		//回転。
};

