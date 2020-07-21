#pragma once
class Camera
{
public:
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<�����ˉe�s��B���ߖ@���������G����肽���Ȃ炱�����B
		enUpdateProjMatrixFunc_Ortho,			//!<���s���e�B�Q�c�I�ȕ\�����������Ȃ炱�����B
	};
	/*!
	 * @brief	�J�����s��A�v���W�F�N�V�����s��̍X�V�B
	 *@details
	 * ���ݐݒ肳��Ă���A�����_�A���_�A��p�Ȃǂ̏������Ƃ�
	 * �J�����s��ƃv���W�F�N�V�����s����X�V���܂��B
	 * ���̊֐����Ă΂Ȃ��ƁA�J�����͓����Ȃ��̂Œ��ӂ��K�v�ł��B
	 */
	void Update();
	/*!
	 * @brief	�J�����s����擾�B
	 */
	CMatrix GetViewMatrix()
	{
		return m_viewMatrix;
	}
	/*!
	 * @brief	�v���W�F�N�V�����s����擾�B
	 */
	CMatrix GetProjectionMatrix() 
	{
		return m_projMatrix;
	}
	/*!
	 * @brief	���_���擾�B
	 */
	CVector3 GetPosition()
	{
		return m_position;
	}
	/*!
	 * @brief	�����_���擾�B
	 */
	CVector3 GetTarget()
	{
		return m_target;
	}
	/*!
	 * @brief	�J�����̏�������擾�B
	 */
	CVector3 GetUp()
	{
		return m_up;
	}
	/// <summary>
	/// �J�����̉E�������擾�B
	/// </summary>
	CVector3 GetRight()
	{
		return m_right;
	}
	/// <summary>
	/// �J�����̌��������擾�B
	/// </summary>
	CVector3 GetBack()
	{
		return m_back;
	}
	/// <summary>
	/// �J�����̑O���������擾�B
	/// </summary>
	CVector3 GetFront()
	{
		return m_front;
	}
	/*!
	 * @brief	���_��ݒ�B
	 */
	void SetPosition( CVector3 pos )
	{
		m_position = pos;
	}
	/*!
	 * @brief	�����_��ݒ�B
	 */
	void SetTarget( CVector3 target )
	{
		m_target = target;
	}
	/*!
	 * @brief	�J�����̏������ݒ�B
	 */
	void SetUp( CVector3 up )
	{
		m_up = up;
	}
	/*!
	 * @brief	�����ʂ�ݒ�B
	 */
	void SetFar( float f )
	{
		m_far = f;
	}
	/*!
	 * @brief	�ߕ��ʂ�ݒ�B
	 */
	void SetNear( float n )
	{
		m_near = n;
	}
	/*!
	 * @brief	��p��ݒ�B
	 */
	void SetViewAngle( float angle )
	{
		m_viewAngle = angle;
	}
	/// <summary>
	/// ���s���e�̕���ݒ�B
	/// </summary>
	/// <remarks>
	/// m_updateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho�̎��Ɏg�p�����B
	/// </remarks>
	/// <param name="w">��</param>
	void SetWidth(float w)
	{
		m_width = w;
	}
	/// <summary>
	/// ���s���e�̍�����ݒ�B
	/// <remarks>
	/// </remarks>
	/// m_updateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho�̎��Ɏg�p�����B
	/// </summary>
	/// <param name="h">����</param>
	void SetHeight(float h)
	{
		m_height = h;
	}
	/// <summary>
	/// �ˉe�s��̌v�Z�̎d����ݒ�B
	/// </summary>
	/// <param name="func">EnUpdateProjMatrixFunc���Q�ƁB</param>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjMatrixFunc = func;
	}
private:
	CMatrix	m_viewMatrix = CMatrix::Identity();		//�r���[�s��B
	CMatrix m_projMatrix = CMatrix::Identity();		//�v���W�F�N�V�����s��B
	CMatrix m_viewProjMatrix = CMatrix::Identity();	//�r���[�v���W�F�N�V�����s��B
	CMatrix		m_viewMatrixInv;					//�r���[�s��̋t�s��B
	CVector3 m_target = CVector3::Zero();			//�����_�B
	CVector3 m_position = CVector3::Zero();			//���_�B
	CVector3 m_up = CVector3::Up();					//�J�����̏�����B
	CVector3 m_right = CVector3::Right();			//�J�����̉E�����B
	CVector3 m_back = CVector3::Back();				//�J�����̌������B
	CVector3 m_front = CVector3::Front();			//�J�����̑O�������B
	float m_viewAngle = CMath::DegToRad(60.0f);		//��p�B
	float m_far = 10000.0f;							//�������ʂ܂ł̋����B
	float m_near = 1.0f;							//�ߕ��ʂ܂ł̋����B
	float m_width = 1280.0f;						//���s���e�̕��B
	float m_height = 720.0f;						//���s���e�̍����B
	EnUpdateProjMatrixFunc m_updateProjMatrixFunc = enUpdateProjMatrixFunc_Perspective;
};

extern Camera g_camera3D;		//3D�J�����B
extern Camera g_camera2D;		//2D�J�����B