#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlayerStateHoldGun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerReloadState.h"
#include "PlayerCreepState.h"
#include "PlayerCreepMoveState.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "UI.h"
#include "Human.h"
#include "physics/CapsuleCollider.h"

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
	/// ダメージを受ける処理。
	/// </summary>
	void Damage();
	/// <summary>
	/// リロード処理。
	/// </summary>
	void Reload();
	/// <summary>
	/// 足音の処理。
	/// </summary>
	void FootStep();
	/// <summary>
	/// アニメーションイベントで呼ぶ関数。
	/// </summary>
	void OnAnimationEvent();
	/// <summary>
	/// 自然回復の処理。
	/// </summary>
	void NaturalRecovery();
	/// <summary>
	/// 伏せているときの移動処理。
	/// </summary>
	void CreepMove();
	/// <summary>
	/// 伏せているときの処理。
	/// </summary>
	void Creep();
	/// <summary>
	/// C4を設置する関数。
	/// </summary>
	void InstallationC4();
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
	/// 弾丸が出ているか返す関数。
	/// </summary>
	/// <returns>
	/// true	出ている。
	/// false	出ていない。
	/// </returns>
	bool GetDangan() const
	{
		return m_dangan;
	}
	/// <summary>
	/// 現在伏せている状態かどうか返す関数。
	/// </summary>
	/// <returns>
	/// true	伏せている。
	/// false	伏せていない。
	/// </returns>
	bool GetCreep() const
	{
		return m_creep;
	}
	/// <summary>
	/// 現在のHPを返す。
	/// </summary>
	/// <returns>
	/// m_hp　体力。
	/// </returns>
	int GetHP() const
	{
		return m_hp;
	}
	
private:
	bool					m_fps = false;								//FPSカメラ状態か？
	GameCamera*				m_gameCamera = nullptr;						//TPSカメラ。
	FPSCamera*				m_fpsCamera = nullptr;						//FPSカメラ。
	IPlayerState*			m_currentstate = nullptr;					//プレイヤーの現在の状態。
	PlayerStateHoldGun		m_holdGunState;								//銃を構えているステート。
	PlayerStateIdle			m_idleState;								//待機ステート。
	PlayerStateMove			m_moveState;								//移動ステート。
	PlayerReloadState		m_reloadState;								//リロード中のステート。
	PlayerCreepState		m_creepState;								//伏せ待機ステート。
	PlayerCreepMoveState	m_creepMoveState;							//伏せ移動ステート。
	CharacterController		m_charaCon;									//キャラクターコントローラー。
	enum EnAnimationClip {											
		enAnimationClip_idle,											//待機。
		enAnimationClip_run,											//走る。
		enAnimationClip_shot,											//銃を撃つ。
		enAnimationClip_walk,											//歩く。
		enAnimationClip_hold,											//構える。
		enAnimationClip_shotend,										//撃ち終わり。
		enAnimationClip_reload,											//リロード。
		enAnimationClip_death,											//死亡。
		enAnimationClip_creep_idle,										//伏せ待機。
		enAnimationClip_creep_forward,									//匍匐前進。
		enAnimationClip_Num,
	};
	AnimationClip			m_animClips[enAnimationClip_Num];			//アニメーションクリップ。
	UI*						m_ui = nullptr;								//UI。
	bool					m_clear = false;							//クリアしたかどうかのフラグ。
	int						m_hp = 5;									//現在のプレイヤーのHP。
	int						m_maxHP;									//HPの最大値。
	bool					m_Firing = false;							//銃を撃っているかどうか。
	bool					m_dash = false;								//走っているかどうか。
	bool					m_dangan = false;							//弾丸が出ているかどうか。
	int						m_maxAmmo;									//銃に装填できる最大弾数。
	bool					m_naturalRecoveryTrigger = false;			//自然回復するフラグ。
	int						m_naturalRecoveryCount = 0;					//自然回復までの時間。
	bool					m_creep = false;							//伏せているかどうか。
};