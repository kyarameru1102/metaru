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
	static EnemyStateBattlePosture		m_battlePosture;							//�퓬�Ԑ��̃X�e�[�g�B
	static EnemyStateHesitate			m_hesitate;									//�p�j�X�e�[�g�B
	static EnemyStateVigilance			m_vigilance;								//�x���X�e�[�g�B
	static EnemyStateVigilanceCancel	m_vigilanceCancel;							//�x�������X�e�[�g�B
};

/// <summary>
/// �G���̃N���X�B
/// </summary>
//���̃N���X�̃C���X�^���X�����������ƃi�r�Q�[�V�������b�V�������������B

class Player;
class Enemy : public Human
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Enemy();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Enemy();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	bool Start() override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �������֐��B
	/// </summary>
	void Init();
	/// <summary>
	/// �G�l�~�[�̃p�X�ړ��̊֐��B
	/// </summary>
	void PathMove();
	/// <summary>
	/// �x���Ԑ��ɂȂ������̈ړ��̊֐��B
	/// </summary>
	void VigilanceMove();
	/// <summary>
	/// �x���Ԑ����������ꂽ���̈ړ��̊֐��B
	/// </summary>
	void VigilanceCancelMove();
	/// <summary>
	/// �퓬���̈ړ��̊֐��B
	/// </summary>
	void BattleMove();
	/// <summary>
	/// �e���������B�e�ۂ𐶐����鏈���B
	/// </summary>
	void Firing();
	/// <summary>
	/// �ړ����̃A�j���[�V���������B
	/// </summary>
	void MoveAnimation();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �X�e�[�g��؂�ւ���֐��B
	/// </summary>
	/// <param name="nextState">
	/// �ύX��̃X�e�[�g
	/// </param>
	void ChangeState(IEnemyState* nextState);
	/// <summary>
	/// �_���[�W���󂯂鏈���B
	/// </summary>
	void Damage();
	/// <summary>
	/// ����p�̌v�Z�����Ă���֐��B
	/// </summary>
	void ViewingAngle();
	/// <summary>
	/// �v���C���[�ƓG���̊Ԃɏ�Q�������邩�ǂ������肷��֐��B
	/// </summary>
	void ShotPossible();
	/// <summary>
	/// A*�̃X���[�W���O�����B
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
	/// ���݂̃X�e�[�g��Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>
	/// ���݂̃X�e�[�g�B
	/// </returns>
	IEnemyState* GetCurrentstate() const
	{
		return m_currentstate;
	}
private:
	IEnemyState*				m_currentstate = nullptr;					//�G�l�~�[�̌��݂̏�ԁB
	std::vector<Path>			PathList;
	int							m_currentPath;								//���݂̃p�X�B
	Player*                     m_player;									//�v���C���[�B
	float						m_timer = 0.0f;
	CVector3					m_oldMoveSpeed;
	bool						m_discovery = false;						//�����������ǂ����B
	CharacterController			m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	Level						m_level;
	wchar_t						m_initPath[3];
	Astar						m_astar;									//A*(�i�r���b�V�����������Ă���B
	int							AstarEXEcount = 0;
	CVector3					m_beforeAstar = CVector3::Zero();
	enum EnAnimationClip {
		enAnimationClip_idle,												//�ҋ@�B
		enAnimationClip_run,												//����B
		enAnimationClip_shot,												//�e�����B
		enAnimationClip_walk,												//�����B
		enAnimationClip_reload,												//�����[�h�B
		enAnimationClip_death,												//���S�B
		enAnimationClip_Num,
	};
	AnimationClip		m_animClips[enAnimationClip_Num];					//�A�j���[�V�����N���b�v�B
	CapsuleCollider		m_collider;											//�J�v�Z���R���C�_�[�B
	bool				m_onFiring = false;									//�e�������Ă��邩�B
	int					m_hp = 3;											//�G����HP�B
	int					m_time = 0;
	CVector3			m_smoothPos = CVector3::Zero();
	float				m_angle = 0.0f;										//����p�p�̊p�x���L�^�B
	float				m_toPlayerLen = 0.0f;								//�v���C���[�܂ł̋����B
	bool				m_hit = false;										//�v���C���[�Ƃ̊Ԃɏ�Q�������邩�ǂ����̃t���O�Btrue�Ȃ炠��B				
	bool				m_relodeOn = true;									//�����[�h�����ɓ��邩�ǂ����B
};

