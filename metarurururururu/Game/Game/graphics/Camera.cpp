#include "stdafx.h"
#include "Camera.h"

Camera g_camera3D;		//3D�J�����B
Camera g_camera2D;		//2D�J�����B

void Camera::Update()
{
	//�r���[�s����v�Z�B
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	if (m_updateProjMatrixFunc == enUpdateProjMatrixFunc_Perspective) {
		//�v���W�F�N�V�����s����v�Z�B
		m_projMatrix.MakeProjectionMatrix(
			m_viewAngle,					//��p�B
			FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
			m_near,
			m_far
		);
	}
	else {
		m_projMatrix.MakeOrthoProjectionMatrix(m_width, m_height, m_near, m_far);
	}
	//�r���[�s����Z�o�B
	m_viewMatrix.MakeLookAt(m_position,m_target,m_up);
	//�r���[�v���W�F�N�V�����s��̍쐬�B
	m_viewProjMatrix.Mul(m_viewMatrix,m_projMatrix);
	//�r���[�s��̋t�s����v�Z�B
	m_viewMatrixInv.Inverse(m_viewMatrix);

	m_front.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	m_right.Set(m_viewMatrixInv.m[0][0], m_viewMatrixInv.m[0][1], m_viewMatrixInv.m[0][2]);

}