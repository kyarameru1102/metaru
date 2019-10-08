#pragma once
#include "SphereCollider.h"
//�O�l�̃J�����̔w�i�ւ̂߂肱�݂��Ȃ����N���X�B

class CameraCollisionSolver
{
public:
	CameraCollisionSolver();
	~CameraCollisionSolver();
	/// <summary>
	/// �������֐��B
	/// </summary>
	/// <param name="radius">
	/// �J�����ɂ���X�t�B�A�R���C�_�[�̔��a�B
	/// </param>
	void Init(float radius);
	/// <summary>
	/// �R���W���������̎��s�B
	/// </summary>
	/// <param name="result"> 
	/// �R���W�����������s�������W���i�[�����B
	/// </param>
	/// <param name="position"> 
	/// �R���W�����������s���O�̃J�����̍��W�B
	/// </param>
	/// <param name="target"> 
	/// �J�����̒����_�B
	/// </param>
	/// <returns> 
	/// �R���W���������������Ȃ�����true��Ԃ��B
	/// </returns>
	bool Execute(CVector3& result, const CVector3& position, const CVector3& target);
private:
	SphereCollider			m_collider;			//�R���C�_�[�B
	float					m_radius = 0.0f;	//���a�B
};

