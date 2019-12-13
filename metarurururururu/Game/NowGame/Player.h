#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "UI.h"

extern Pad g_pad[Pad::CONNECT_PAD_MAX];

class Bullet;
class GameCamera;
class FPSCamera;
class Player : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Player();
	/// <summary>
	/// 開始関数。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update();
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	/// <summary>
	/// 移動中のアニメーション処理。
	/// </summary>
	void MoveAnimation();
	/// <summary>
	/// 銃を構えている時の移動処理。
	/// </summary>
	void HoldMove();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// 銃を構えているときの回転処理。
	/// </summary>
	void HoldRotation();
	/// <summary>
	/// 銃を撃つ処理。弾丸を生成する処理。
	/// </summary>
	void Firing();
	/// <summary>
	/// カメラの切り替え処理。FPSカメラに切り替える。
	/// </summary>
	void CameraSwitchFPS();
	/// <summary>
	/// カメラの切り替え処理。TPSカメラに切り替える。
	/// </summary>
	void CameraSwitchTPS();
	/// <summary>
	///ステートを切り替える関数。 
	/// </summary>
	/// <param name="nextState">切り替えたいステート。</param>
	void ChangeState(IPlayerState* nextState);
	/// <summary>
	/// プレイヤーのポジションを設定する関数。
	/// </summary>
	/// <param name="pos">
	/// ポジション。
	/// </param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// プレイヤーのポジションを返してくる関数。
	/// </summary>
	CVector3 GetPosition() const
	{
		return m_position;
	}
	CQuaternion GetRotation() const
	{
		return m_rotation;
	}
private:
	SkinModel			m_model;									//スキンモデル。
	SkinModelRender*	m_skinModelRender = nullptr;				//スキンモデルレンダー。
	CVector3			m_moveSpeed = CVector3::Zero();
	CVector3			m_position = CVector3::Zero();				//座標。
	CVector3			m_Plxz;										//回転用。
	CQuaternion			m_rotation = CQuaternion::Identity();
	bool				m_fps = false;								//FPSカメラ状態か？
	GameCamera*			m_gameCamera = nullptr;
	FPSCamera*			m_fpsCamera = nullptr;
	IPlayerState*		m_currentstate = nullptr;					//プレイヤーの現在の状態。
	PlayerStateHoldGun	m_holdGunState;								//銃を構えているステート。
	PlayerStateIdle		m_idleState;								//待機ステート。
	PlayerStateMove		m_moveState;								//移動ステート。
	CharacterController m_charaCon;									//キャラクターコントローラー。
	enum EnAnimationClip {											
		enAnimationClip_idle,										//待機。
		enAnimationClip_run,										//走る。
		enAnimationClip_shot,										//銃を撃つ。
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];			//アニメーションクリップ。
	UI*					m_ui = nullptr;
};

