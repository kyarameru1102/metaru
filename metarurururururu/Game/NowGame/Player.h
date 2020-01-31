#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerReloadState.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "UI.h"
#include "Human.h"

extern Pad g_pad[Pad::CONNECT_PAD_MAX];

class Bullet;
class GameCamera;
class FPSCamera;
class Player : public Human
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
	/// プレイヤーの回転を返してくる関数。
	/// </summary>
	/// <returns>rotation	回転。</returns>
	CQuaternion GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// クリアしたかどうかのフラグを設定する関数。
	/// </summary>
	/// <param name="flag">
	/// true	クリアしている
	/// false	クリアしていない。
	/// </param>
	void SetClearFlag(bool flag)
	{
		m_clear = flag;
	}
	/// <summary>
	/// 死んでいるかどうか返す関数。
	/// </summary>
	/// <returns>
	/// true	死んでいる。
	/// false	死んでいない。
	/// </returns>
	bool GetDeath() const
	{
		return m_death;
	}
	/// <summary>
	/// クリアしたかどうかのフラグを返す関数。
	/// </summary>
	/// <returns>
	/// true	クリアした。
	/// false	クリアしていない。
	/// </returns>
	bool GetClear() const
	{
		return m_clear;
	}
	/// <summary>
	/// ダメージを受ける処理。
	/// </summary>
	void Damage();
private:
	bool				m_fps = false;								//FPSカメラ状態か？
	GameCamera*			m_gameCamera = nullptr;
	FPSCamera*			m_fpsCamera = nullptr;
	IPlayerState*		m_currentstate = nullptr;					//プレイヤーの現在の状態。
	PlayerStateHoldGun	m_holdGunState;								//銃を構えているステート。
	PlayerStateIdle		m_idleState;								//待機ステート。
	PlayerStateMove		m_moveState;								//移動ステート。
	PlayerReloadState	m_reloadState;								//リロード中のステート。
	CharacterController m_charaCon;									//キャラクターコントローラー。
	enum EnAnimationClip {											
		enAnimationClip_idle,										//待機。
		enAnimationClip_run,										//走る。
		enAnimationClip_shot,										//銃を撃つ。
		enAnimationClip_walk,										//歩く。
		enAnimationClip_hold,										//構える。
		enAnimationClip_shotend,									//撃ち終わり。
		enAnimationClip_reload,										//リロード。
		enAnimationClip_death,										//死亡。
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];			//アニメーションクリップ。
	UI*					m_ui = nullptr;								//UI。
	bool				m_clear = false;							//クリアしたかどうかのフラグ。
	int					m_hp = 999910;								//プレイヤーのHP。
	bool				m_Firing = false;							//銃を撃っているかどうか。
	bool				m_dash = false;								//走っているかどうか。
};