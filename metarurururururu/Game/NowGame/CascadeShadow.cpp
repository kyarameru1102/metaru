#include "stdafx.h"
#include "CascadeShadow.h"


CascadeShadow::CascadeShadow()
{
	for (int i = 0; i < m_numCascade; i++) {
		//�V���h�E�}�b�v�����p�̃����_�����O�^�[�Q�b�g���쐬�B
		//�𑜓x��2048�~2048�B
		//�e�N�X�`���̃t�H�[�}�b�g��R�����݂̂�32bit���������_�^�B
		m_shadowMapRT[i].Create(
			4096,
			4096,
			DXGI_FORMAT_R32_FLOAT
		);
	}
}


CascadeShadow::~CascadeShadow()
{
}

void CascadeShadow::Update()
{
	CVector3 cameraPos = g_camera3D.GetPosition();								//�J�����̈ʒu�B
	float halfviewAngle = g_camera3D.GetViewAngle() * 0.5f;						//�J�����̉�p�̔����B
	float nearDistance = g_camera3D.GetFar();									//�J�����̋ߕ��ʂ܂ł̋����B
	float farDistance = g_camera3D.GetNear();									//�J�����̉����ʂ܂ł̋����B

	//���p�B
	float x1 = tan(halfviewAngle) * nearDistance;								//�߂��ق��B
	float x2 = tan(halfviewAngle) * farDistance;								//�����ق��B

	//�c�p�B
	float aspect = g_camera3D.GetHeight() / g_camera3D.GetWidth();				//�A�X�y�N�g��B
	halfviewAngle *= aspect;
	float y1 = tan(halfviewAngle) * nearDistance;								//�߂��ق��B
	float y2 = tan(halfviewAngle) * farDistance;								//�����ق��B
	
	//8���_�����߂�B
	CVector3 nearCenter = (g_camera3D.GetFront() * nearDistance) + cameraPos;	//�ߕ��ʂ̒��S�B
	CVector3 farCenter = (g_camera3D.GetFront() * farDistance) + cameraPos;		//�����ʂ̒��S�B
	
	CVector3 vertex[8];
	vertex[0] = (g_camera3D.GetRight() * x1) + nearCenter;
	vertex[0] += (g_camera3D.GetUp() * y1);										//1�ځB
	vertex[1] = (g_camera3D.GetRight() * x1) + nearCenter;
	vertex[1] += (g_camera3D.GetUp() * -y1);									//2�ځB
	vertex[2] = (g_camera3D.GetRight() * -x1) + nearCenter;
	vertex[2] += (g_camera3D.GetUp() * y1);										//3�ځB
	vertex[3] = (g_camera3D.GetRight() * -x1) + nearCenter;
	vertex[3] += (g_camera3D.GetUp() * -y1);									//4�ځB

	vertex[4] = (g_camera3D.GetRight() * x2) + farCenter;
	vertex[4] += (g_camera3D.GetUp() * y2);										//5�ځB
	vertex[5] = (g_camera3D.GetRight() * x2) + farCenter;
	vertex[5] += (g_camera3D.GetUp() * -y2);									//6�ځB
	vertex[6] = (g_camera3D.GetRight() * -x2) + farCenter;
	vertex[6] += (g_camera3D.GetUp() * y2);										//7�ځB
	vertex[7] = (g_camera3D.GetRight() * -x2) + farCenter;
	vertex[7] += (g_camera3D.GetUp() * -y2);									//8�ځB

	//8���_�̒��S�����߂�B
	CVector3 center;
	for (int i = 0; i < 8; i++) {
		center += vertex[i];
	}
	center /= 8;

	//���C�g�̍��W�����߂�B
	m_lightCameraTarget = center;
	m_lightDirection = { 1.0f, -2.0f, 1.0f };
	m_lightDirection.Normalize();
	m_lightCameraPosition = (m_lightDirection * -2000.0f) + m_lightCameraTarget;

	//���C�g�r���[�s����v�Z����B
	m_lightViewMatrix.MakeLookAt(
		m_lightCameraPosition,
		m_lightCameraTarget,
		CVector3::AxisY()
	);

	float xmin = FLT_MAX;		//x�̍ŏ��B
	float ymin = FLT_MAX;		//y�̍ŏ��B
	float xmax = FLT_MIN;		//x�̍ő�B
	float ymax = FLT_MIN;		//y�̍ő�B
	float zmax = FLT_MIN;		//z�̍ő�B

	//8���_�Ƀ��C�g�r���[�s����|����B
	for (int i = 0; i < 8; i++) {
		m_lightViewMatrix.Mul(vertex[i]);
		//��������xy�̍ŏ��ő�̒l��z�̍ő�̒l�����߂�B
		//x�̍ŏ��B
		if (vertex[i].x < xmin) {
			xmin = vertex[i].x;
		}
		//y�̍ŏ��B
		if (vertex[i].y < ymin) {
			ymin = vertex[i].y;
		}
		//x�̍ő�B
		if (vertex[i].x > xmax) {
			xmax = vertex[i].x;
		}
		//y�̍ő�B
		if (vertex[i].y > ymax) {
			ymax = vertex[i].y;
		}
		//z�̍ő�B
		if (vertex[i].z > zmax) {
			zmax = vertex[i].z;
		}
	}

	//�v���W�F�N�V�����s������B
	float w = xmax - xmin;		//���B
	float h = ymax - ymin;		//�����B

	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		w,
		h,
		1.0f,
		zmax
	);

	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&oldRenderTargetView,
		&oldDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);

	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_shadowMapRT[0].GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT[0].GetDepthStensilView());

	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[0].GetViewport());
	//�V���h�E�}�b�v���N���A�B
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT[0].ClearRenderTarget(clearColor);

	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnShadowMap);
	//�V���h�E�L���X�^�[���V���h�E�}�b�v�Ƀ����_�����O�B
	for (auto& caster : m_shadowCasters) {
		caster->Draw(
			m_lightViewMatrix,
			m_lightProjMatrix
		);
	}
	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnNormal);

	//�L���X�^�[���N���A�B
	m_shadowCasters.clear();

	//���ɖ߂��B
	d3dDeviceContext->OMSetRenderTargets(
		1,
		&oldRenderTargetView,
		oldDepthStencilView
	);
	d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	//�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̎Q�ƃJ�E���^�������B
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();
}
