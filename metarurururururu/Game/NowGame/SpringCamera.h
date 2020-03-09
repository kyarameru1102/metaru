#pragma once
#include "graphics/Camera.h"
#include "CameraCollisionSolver.h"
class SpringCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	SpringCamera();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~SpringCamera();
	/// <summary>
	/// �������֐��B
	/// </summary>
	/// <param name="camera">Camera �΂˃J�����ɐݒ肷��J�����B</param>
	/// <param name="maxMoveSpeed">float �J�����̍ő�X�s�[�h�B</param>
	/// <param name="isEnableCollisionSolver">bool �R���W����������L���ɂ��邩�ǂ����B</param>
	/// <param name="sphereCollisionRadius">float �R���W��������������ꍇ�̃R���C�_�[�̔��a�B</param>
	void Init(
		Camera& camera,
		float maxMoveSpeed,
		bool isEnableCollisionSolver,
		float sphereCollisionRadius
	);
	/// <summary>
	/// �^�[�Q�b�g��ݒ肷��֐��B
	/// </summary>
	/// <param name="target">CVector3 �^�[�Q�b�g�Ƃ�����W�B</param>
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/// <summary>
	/// �J�����̍��W��ݒ肷��֐��B
	/// </summary>
	/// <param name="position">CVector3 �J�����̍��W</param>
	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update();
	/// <summary>
	/// �΂˃J�����݂̂̍X�V�֐��B
	/// </summary>
	void UpdateSpringCamera();
	/// <summary>
	/// �J�����݂̂̍X�V�֐��B
	/// </summary>
	void UpdateCamera()
	{
		if (m_camera) {
			m_camera->Update();
		}
	}
private:
	Camera*					m_camera = nullptr;
	CVector3				m_target = CVector3::Zero();			//�ڕW�ƂȂ钍���_�B
	CVector3				m_position = CVector3::Zero();			//�ڕW�ƂȂ鎋�_�B
	CVector3				m_targetMoveSpeed = CVector3::Zero();	//�����_�̈ړ����x�B
	CVector3				m_positionMoveSpeed = CVector3::Zero();	//���_�̈ړ����x�B
	float					m_maxMoveSpeed = 0.0f;					//�ō��ړ����x�B
	float					m_targetDampingRate = 0.2f;				//�������B�l���傫���قǃJ�������x��t���Ă���B
	float					m_dampingRate = 1.0f;					//�������B
	float					m_dampingRateVel = 0.0f;
	bool					m_isEnableCollisionSolver = false;		//�R���W�����������L���H
	bool					m_isRefresh = true;						//���t���b�V�����K�v�H
	CameraCollisionSolver	m_cameraCollisionSolver;
};

