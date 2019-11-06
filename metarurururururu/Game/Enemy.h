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
/// <summary>
/// 敵兵のクラス。
/// </summary>
//このクラスのインスタンスが生成されるとナビゲーションメッシュが生成される。

class Player;
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	//初期化関数。
	void Init();
	//エネミーのパス移動の関数。
	void PathMove();
	//戦闘態勢時の行動。
	void BattleMove();
	//回転処理。
	void Rotation();
	//ステートを切り替える関数。
	void ChangeState(IEnemyState* nextState);
	//ポジションを設定する関数。
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//ポジションを返してくる関数。
	CVector3 GetPosition()
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
private:
	SkinModel					m_model;									//スキンモデル。
	SkinModelRender*			m_skinModelRender = nullptr;				//スキンモデルレンダー。
	CVector3					m_position = CVector3::Zero();				//座標。
	CVector3					m_moveSpeed = CVector3::Zero();
	CQuaternion					m_rotation = CQuaternion::Identity();
	IEnemyState*				m_currentstate = nullptr;					//エネミーの現在の状態。
	EnemyStateBattlePosture		m_battlePosture;							//戦闘態勢のステート。
	EnemyStateHesitate			m_hesitate;									//徘徊ステート。
	EnemyStateVigilance			m_vigilance;								//警戒ステート。
	EnemyStateVigilanceCancel	m_vigilanceCancel;							//警戒解除ステート。
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
	AstarDebug*					m_astarDebug = nullptr;
	int							AstarEXEcount = 0;
	CVector3					m_beforeAstar = CVector3::Zero();
};

