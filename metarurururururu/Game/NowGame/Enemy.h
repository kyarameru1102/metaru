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
#include "physics/CapsuleCollider.h"
#include "Human.h"

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
class Enemy : public Human
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
	/// ダメージを受ける処理。
	/// </summary>
	void Damage();
	/// <summary>
	/// 視野角の計算をしている関数。
	/// </summary>
	void ViewingAngle();
	/// <summary>
	/// プレイヤーと敵兵の間に障害物があるかどうか判定する関数。
	/// </summary>
	void ShotPossible();
	/// <summary>
	/// A*のスムージング処理。
	/// </summary>
	void AstarSmooth();
	
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
	IEnemyState*				m_currentstate = nullptr;					//エネミーの現在の状態。
	std::vector<Path>			PathList;
	int							m_currentPath;								//現在のパス。
	Player*                     m_player;									//プレイヤー。
	float						m_timer = 0.0f;
	CVector3					m_oldMoveSpeed;
	bool						m_discovery = false;						//発見したかどうか。
	CharacterController			m_charaCon;									//キャラクターコントローラー。
	Level						m_level;
	wchar_t						m_initPath[3];
	Astar						m_astar;									//A*(ナビメッシュも生成している。
	int							AstarEXEcount = 0;
	CVector3					m_beforeAstar = CVector3::Zero();
	enum EnAnimationClip {
		enAnimationClip_idle,												//待機。
		enAnimationClip_run,												//走る。
		enAnimationClip_shot,												//銃を撃つ。
		enAnimationClip_walk,												//歩く。
		enAnimationClip_reload,												//リロード。
		enAnimationClip_death,												//死亡。
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];					//アニメーションクリップ。
	CapsuleCollider		m_collider;											//カプセルコライダー。
	bool				m_onFiring = false;									//銃を撃っているか。
	int					m_hp = 3;											//敵兵のHP。
	int					m_time = 0;
	CVector3			m_smoothPos = CVector3::Zero();
	float				m_angle = 0.0f;										//視野角用の角度を記録。
	float				m_toPlayerLen = 0.0f;								//プレイヤーまでの距離。
	bool				m_hit = false;										//プレイヤーとの間に障害物があるかどうかのフラグ。trueならある。				
	bool				m_relodeOn = true;									//リロード処理に入るかどうか。
};

