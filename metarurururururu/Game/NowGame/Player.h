#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "SkinModelRender.h"
#include "Bullet.h"

extern Pad g_pad[Pad::CONNECT_PAD_MAX];

class Bullet;
class GameCamera;
class FPSCamera;
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void OnDestroy();
	//移動処理。
	void Move();
	//移動中のアニメーション処理。
	void MoveAnimation();
	//銃を構えている時の移動処理。
	void HoldMove();
	//回転処理。
	void Rotation();
	//銃を構えているときの回転処理。
	void HoldRotation();
	//銃を撃つ処理。弾丸を生成する処理。
	void Firing();
	//カメラの切り替え処理。FPSカメラに切り替える。
	void CameraSwitchFPS();
	//カメラの切り替え処理。TPSカメラに切り替える。
	void CameraSwitchTPS();
	//ステートを切り替える関数。
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
	CVector3 GetPosition()
	{
		return m_position;
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
		enAnimationClip_idle,
		enAnimationClip_run,
		enAnimationClip_shot,
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];			//アニメーションクリップ。
	//Bullet*				m_bullet = nullptr;							//弾丸。
};

