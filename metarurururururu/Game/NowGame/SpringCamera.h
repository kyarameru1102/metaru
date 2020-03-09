#pragma once
#include "graphics/Camera.h"
#include "CameraCollisionSolver.h"
class SpringCamera
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	SpringCamera();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~SpringCamera();
	/// <summary>
	/// 初期化関数。
	/// </summary>
	/// <param name="camera">Camera ばねカメラに設定するカメラ。</param>
	/// <param name="maxMoveSpeed">float カメラの最大スピード。</param>
	/// <param name="isEnableCollisionSolver">bool コリジョン処理を有効にするかどうか。</param>
	/// <param name="sphereCollisionRadius">float コリジョン処理をする場合のコライダーの半径。</param>
	void Init(
		Camera& camera,
		float maxMoveSpeed,
		bool isEnableCollisionSolver,
		float sphereCollisionRadius
	);
	/// <summary>
	/// ターゲットを設定する関数。
	/// </summary>
	/// <param name="target">CVector3 ターゲットとする座標。</param>
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/// <summary>
	/// カメラの座標を設定する関数。
	/// </summary>
	/// <param name="position">CVector3 カメラの座標</param>
	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update();
	/// <summary>
	/// ばねカメラのみの更新関数。
	/// </summary>
	void UpdateSpringCamera();
	/// <summary>
	/// カメラのみの更新関数。
	/// </summary>
	void UpdateCamera()
	{
		if (m_camera) {
			m_camera->Update();
		}
	}
private:
	Camera*					m_camera = nullptr;
	CVector3				m_target = CVector3::Zero();			//目標となる注視点。
	CVector3				m_position = CVector3::Zero();			//目標となる視点。
	CVector3				m_targetMoveSpeed = CVector3::Zero();	//注視点の移動速度。
	CVector3				m_positionMoveSpeed = CVector3::Zero();	//視点の移動速度。
	float					m_maxMoveSpeed = 0.0f;					//最高移動速度。
	float					m_targetDampingRate = 0.2f;				//減衰率。値が大きいほどカメラが遅れ付いてくる。
	float					m_dampingRate = 1.0f;					//減衰率。
	float					m_dampingRateVel = 0.0f;
	bool					m_isEnableCollisionSolver = false;		//コリジョン処理が有効？
	bool					m_isRefresh = true;						//リフレッシュが必要？
	CameraCollisionSolver	m_cameraCollisionSolver;
};

