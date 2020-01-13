#pragma once
#include "EnemyState.h"
#include "EnemyStateBattlePosture.h"
#include "EnemyStateHesitate.h"
#include "EnemyStateVigilance.h"
#include "EnemyStateVigilanceCancel.h"
#include "Path.h"
#include "character/CharacterController.h"
#include <vector>
#include <string>
#include "Astar.h"
#include "AstarDebug.h"
#include "physics/CapsuleCollider.h"

struct EnemyState
{
public:
	static EnemyStateBattlePosture		m_battlePosture;							//戦闘態勢のステート。
	static EnemyStateHesitate			m_hesitate;									//徘徊ステート。
	static EnemyStateVigilance			m_vigilance;								//警戒ステート。
	static EnemyStateVigilanceCancel	m_vigilanceCancel;							//警戒解除ステート。
};

/// <summary>
/// 敵兵のクラス。
/// </summary>
//このクラスのインスタンスが生成されるとナビゲーションメッシュが生成される。

class Player;
class Enemy : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Enemy();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Enemy();
	/// <summary>
	/// 開始関数。
	/// </summary>
	bool Start() override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 初期化関数。
	/// </summary>
	void Init();
	/// <summary>
	/// エネミーのパス移動の関数。
	/// </summary>
	void PathMove();
	/// <summary>
	/// 警戒態勢になった時の移動の関数。
	/// </summary>
	void VigilanceMove();
	/// <summary>
	/// 警戒態勢が解除された時の移動の関数。
	/// </summary>
	void VigilanceCancelMove();
	/// <summary>
	/// 戦闘時の移動の関数。
	/// </summary>
	void BattleMove();
	/// <summary>
	/// 銃を撃つ処理。弾丸を生成する処理。
	/// </summary>
	void Firing();
	/// <summary>
	/// 移動中のアニメーション処理。
	/// </summary>
	void MoveAnimation();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// ステートを切り替える関数。
	/// </summary>
	/// <param name="nextState">
	/// 変更先のステート
	/// </param>
	void ChangeState(IEnemyState* nextState);
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
	void AddPath(Path path)
	{
		PathList.push_back(path);
	}
	void SetPathNum(const wchar_t* path)
	{
		wcscpy(m_initPath,path);
	}
	/// <summary>
	/// 現在のステートを返してくる関数。
	/// </summary>
	/// <returns>
	/// 現在のステート。
	/// </returns>
	IEnemyState* GetCurrentstate() const
	{
		return m_currentstate;
	}
private:
	SkinModel					m_model;									//スキンモデル。
	SkinModelRender*			m_skinModelRender = nullptr;				//スキンモデルレンダー。
	CVector3					m_position = CVector3::Zero();				//座標。
	CVector3					m_moveSpeed = CVector3::Zero();
	CQuaternion					m_rotation = CQuaternion::Identity();
	IEnemyState*				m_currentstate = nullptr;					//エネミーの現在の状態。
	std::vector<Path>			PathList;
	int							m_currentPath;								//現在のパス。
	CVector3					m_Enxz;
	Player*                     m_player;
	float						m_timer = 0.0f;
	CVector3					m_oldMoveSpeed;
	bool						m_discovery = false;						//発見したかどうか。
	CharacterController			m_charaCon;									//キャラクターコントローラー。
	Level						m_level;
	wchar_t						m_initPath[3];
	Astar						m_astar;									//A*(ナビメッシュも生成している。
	Astar						m_astar2;
	AstarDebug*					m_astarDebug = nullptr;
	int							AstarEXEcount = 0;
	CVector3					m_beforeAstar = CVector3::Zero();
	enum EnAnimationClip {
		enAnimationClip_idle,												//待機。
		enAnimationClip_run,												//走る。
		enAnimationClip_shot,												//銃を撃つ。
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];					//アニメーションクリップ。
	CapsuleCollider		m_collider;											//カプセルコライダー。
	bool				m_onFiring = false;									//銃を撃っているか。
};

