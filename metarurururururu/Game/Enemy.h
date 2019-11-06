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
/// �G���̃N���X�B
/// </summary>
//���̃N���X�̃C���X�^���X�����������ƃi�r�Q�[�V�������b�V�������������B

class Player;
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	//�������֐��B
	void Init();
	//�G�l�~�[�̃p�X�ړ��̊֐��B
	void PathMove();
	//�퓬�Ԑ����̍s���B
	void BattleMove();
	//��]�����B
	void Rotation();
	//�X�e�[�g��؂�ւ���֐��B
	void ChangeState(IEnemyState* nextState);
	//�|�W�V������ݒ肷��֐��B
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//�|�W�V������Ԃ��Ă���֐��B
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
	SkinModel					m_model;									//�X�L�����f���B
	SkinModelRender*			m_skinModelRender = nullptr;				//�X�L�����f�������_�[�B
	CVector3					m_position = CVector3::Zero();				//���W�B
	CVector3					m_moveSpeed = CVector3::Zero();
	CQuaternion					m_rotation = CQuaternion::Identity();
	IEnemyState*				m_currentstate = nullptr;					//�G�l�~�[�̌��݂̏�ԁB
	EnemyStateBattlePosture		m_battlePosture;							//�퓬�Ԑ��̃X�e�[�g�B
	EnemyStateHesitate			m_hesitate;									//�p�j�X�e�[�g�B
	EnemyStateVigilance			m_vigilance;								//�x���X�e�[�g�B
	EnemyStateVigilanceCancel	m_vigilanceCancel;							//�x�������X�e�[�g�B
	std::vector<Path>			PathList;
	int							m_currentPath;								//���݂̃p�X�B
	CVector3					m_Enxz;
	Player*                     m_player;
	float						m_timer = 0.0f;
	CVector3					m_oldMoveSpeed;
	bool						m_discovery = false;						//�����������ǂ����B
	CharacterController			m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	Level						m_level;
	wchar_t						m_initPath[3];
	Astar						m_astar;									//A*(�i�r���b�V�����������Ă���B
	AstarDebug*					m_astarDebug = nullptr;
	int							AstarEXEcount = 0;
	CVector3					m_beforeAstar = CVector3::Zero();
};

