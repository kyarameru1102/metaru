#pragma once
class Camera
{
public:
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<透視射影行列。遠近法が効いた絵を作りたいならこっち。
		enUpdateProjMatrixFunc_Ortho,			//!<平行投影。２Ｄ的な表現がしたいならこっち。
	};
	/*!
	 * @brief	カメラ行列、プロジェクション行列の更新。
	 *@details
	 * 現在設定されている、注視点、視点、画角などの情報をもとに
	 * カメラ行列とプロジェクション行列を更新します。
	 * この関数を呼ばないと、カメラは動かないので注意が必要です。
	 */
	void Update();
	/*!
	 * @brief	カメラ行列を取得。
	 */
	CMatrix GetViewMatrix()
	{
		return m_viewMatrix;
	}
	/*!
	 * @brief	プロジェクション行列を取得。
	 */
	CMatrix GetProjectionMatrix() 
	{
		return m_projMatrix;
	}
	/*!
	 * @brief	視点を取得。
	 */
	CVector3 GetPosition()
	{
		return m_position;
	}
	/*!
	 * @brief	注視点を取得。
	 */
	CVector3 GetTarget()
	{
		return m_target;
	}
	/*!
	 * @brief	カメラの上方向を取得。
	 */
	CVector3 GetUp()
	{
		return m_up;
	}
	/// <summary>
	/// カメラの右方向を取得。
	/// </summary>
	CVector3 GetRight()
	{
		return m_right;
	}
	/// <summary>
	/// カメラの後ろ方向を取得。
	/// </summary>
	CVector3 GetBack()
	{
		return m_back;
	}
	/// <summary>
	/// カメラの前方方向を取得。
	/// </summary>
	CVector3 GetFront()
	{
		return m_front;
	}
	/*!
	 * @brief	視点を設定。
	 */
	void SetPosition( CVector3 pos )
	{
		m_position = pos;
	}
	/*!
	 * @brief	注視点を設定。
	 */
	void SetTarget( CVector3 target )
	{
		m_target = target;
	}
	/*!
	 * @brief	カメラの上方向を設定。
	 */
	void SetUp( CVector3 up )
	{
		m_up = up;
	}
	/*!
	 * @brief	遠平面を設定。
	 */
	void SetFar( float f )
	{
		m_far = f;
	}
	/*!
	 * @brief	近平面を設定。
	 */
	void SetNear( float n )
	{
		m_near = n;
	}
	/*!
	 * @brief	画角を設定。
	 */
	void SetViewAngle( float angle )
	{
		m_viewAngle = angle;
	}
	/// <summary>
	/// 平行投影の幅を設定。
	/// </summary>
	/// <remarks>
	/// m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
	/// </remarks>
	/// <param name="w">幅</param>
	void SetWidth(float w)
	{
		m_width = w;
	}
	/// <summary>
	/// 平行投影の高さを設定。
	/// <remarks>
	/// </remarks>
	/// m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
	/// </summary>
	/// <param name="h">高さ</param>
	void SetHeight(float h)
	{
		m_height = h;
	}
	/// <summary>
	/// 射影行列の計算の仕方を設定。
	/// </summary>
	/// <param name="func">EnUpdateProjMatrixFuncを参照。</param>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjMatrixFunc = func;
	}
private:
	CMatrix	m_viewMatrix = CMatrix::Identity();		//ビュー行列。
	CMatrix m_projMatrix = CMatrix::Identity();		//プロジェクション行列。
	CMatrix m_viewProjMatrix = CMatrix::Identity();	//ビュープロジェクション行列。
	CMatrix		m_viewMatrixInv;					//ビュー行列の逆行列。
	CVector3 m_target = CVector3::Zero();			//注視点。
	CVector3 m_position = CVector3::Zero();			//視点。
	CVector3 m_up = CVector3::Up();					//カメラの上方向。
	CVector3 m_right = CVector3::Right();			//カメラの右方向。
	CVector3 m_back = CVector3::Back();				//カメラの後ろ方向。
	CVector3 m_front = CVector3::Front();			//カメラの前方方向。
	float m_viewAngle = CMath::DegToRad(60.0f);		//画角。
	float m_far = 10000.0f;							//遠い平面までの距離。
	float m_near = 1.0f;							//近平面までの距離。
	float m_width = 1280.0f;						//平行投影の幅。
	float m_height = 720.0f;						//平行投影の高さ。
	EnUpdateProjMatrixFunc m_updateProjMatrixFunc = enUpdateProjMatrixFunc_Perspective;
};

extern Camera g_camera3D;		//3Dカメラ。
extern Camera g_camera2D;		//2Dカメラ。