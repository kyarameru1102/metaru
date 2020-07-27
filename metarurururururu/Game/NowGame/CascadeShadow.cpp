#include "stdafx.h"
#include "CascadeShadow.h"


CascadeShadow::CascadeShadow()
{
	//�V���h�E�}�b�v�����p�̃����_�����O�^�[�Q�b�g���쐬�B
	//�e�N�X�`���̃t�H�[�}�b�g��R�����݂̂�32bit���������_�^�B
	
	m_shadowMapRT[0].Create(
		4096,
		4096,
		DXGI_FORMAT_R32_FLOAT
	);
	
	m_shadowMapRT[1].Create(
		2048,
		2048,
		DXGI_FORMAT_R32_FLOAT
	);

	m_shadowMapRT[2].Create(
		1024,
		1024,
		DXGI_FORMAT_R32_FLOAT
	);
}


CascadeShadow::~CascadeShadow()
{
}

void CascadeShadow::Update()
{
	 m_nearDistance[0] = g_camera3D.GetNear();									
	 m_farDistance[0] = 800.0f;											

	 m_nearDistance[1] = 800.0f;
	 m_farDistance[1] = 3000.0f;
	
	 m_nearDistance[2] = 3000.0f;
	 m_farDistance[2] = 15000.0f;


	for (int i = 0; i < m_numCascade; i++) {
		MatrixCalculation(m_nearDistance[i], m_farDistance[i],i);
	}
	
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

	for (int i = 0; i < m_numCascade; i++) {
		Render(i);
	}

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

void CascadeShadow::MatrixCalculation(float Near, float Far, int i)
{
	CVector3 cameraPos = g_camera3D.GetPosition();								//�J�����̈ʒu�B
	float halfviewAngle = g_camera3D.GetViewAngle() * 0.5f;						//�J�����̉�p�̔����B
	float aspect = g_camera3D.GetWidth() / g_camera3D.GetHeight();				//�A�X�y�N�g��B

	float y1 = tan(halfviewAngle) * Near;										//�߂��ق��B
	float y2 = tan(halfviewAngle) * Far;										//�����ق��B

	halfviewAngle *= aspect;
	float x1 = tan(halfviewAngle) * Near;										//�߂��ق��B
	float x2 = tan(halfviewAngle) * Far;										//�����ق��B

	//8���_�����߂�B
	CVector3 nearCenter = (g_camera3D.GetFront() * Near) + cameraPos;			//�ߕ��ʂ̒��S�B
	CVector3 farCenter = (g_camera3D.GetFront() * Far) + cameraPos;				//�����ʂ̒��S�B

	
	m_eightVertices[i].vertex[0] = (g_camera3D.GetRight() * x1) + nearCenter;
	m_eightVertices[i].vertex[0] += (g_camera3D.GetUp() * y1);					//1�ځB
	m_eightVertices[i].vertex[1] = (g_camera3D.GetRight() * x1) + nearCenter;
	m_eightVertices[i].vertex[1] += (g_camera3D.GetUp() * -y1);					//2�ځB
	m_eightVertices[i].vertex[2] = (g_camera3D.GetRight() * -x1) + nearCenter;
	m_eightVertices[i].vertex[2] += (g_camera3D.GetUp() * y1);					//3�ځB
	m_eightVertices[i].vertex[3] = (g_camera3D.GetRight() * -x1) + nearCenter;
	m_eightVertices[i].vertex[3] += (g_camera3D.GetUp() * -y1);					//4�ځB

	m_eightVertices[i].vertex[4] = (g_camera3D.GetRight() * x2) + farCenter;
	m_eightVertices[i].vertex[4] += (g_camera3D.GetUp() * y2);					//5�ځB
	m_eightVertices[i].vertex[5] = (g_camera3D.GetRight() * x2) + farCenter;
	m_eightVertices[i].vertex[5] += (g_camera3D.GetUp() * -y2);					//6�ځB
	m_eightVertices[i].vertex[6] = (g_camera3D.GetRight() * -x2) + farCenter;
	m_eightVertices[i].vertex[6] += (g_camera3D.GetUp() * y2);					//7�ځB
	m_eightVertices[i].vertex[7] = (g_camera3D.GetRight() * -x2) + farCenter;
	m_eightVertices[i].vertex[7] += (g_camera3D.GetUp() * -y2);					//8�ځB

	//8���_�̒��S�����߂�B
	CVector3 center = CVector3::Zero();
	for (int j = 0; j < 8; j++) {
		center += m_eightVertices[i].vertex[j];
	}
	center /= 8;

	//���C�g�̍��W�����߂�B
	m_lightCameraTarget[i] = center;
	m_lightDirection = { 1.0f, -2.0f, 1.0f };
	m_lightDirection.Normalize();
	m_lightCameraPosition[i] = (m_lightDirection * -8000.0f) + m_lightCameraTarget[i];

	//���C�g�r���[�s����v�Z����B
	m_lightViewMatrix[i].MakeLookAt(
		m_lightCameraPosition[i],
		m_lightCameraTarget[i],
		CVector3::AxisY()
	);

	float xmin = FLT_MAX;		//x�̍ŏ��B
	float ymin = FLT_MAX;		//y�̍ŏ��B
	float xmax = FLT_MIN;		//x�̍ő�B
	float ymax = FLT_MIN;		//y�̍ő�B
	float zmax = FLT_MIN;		//z�̍ő�B

	//8���_�Ƀ��C�g�r���[�s����|����B
	for (int j = 0; j < 8; j++) {
		m_lightViewMatrix[i].Mul(m_eightVertices[i].vertex[j]);
		//��������xy�̍ŏ��ő�̒l��z�̍ő�̒l�����߂�B
		//x�̍ŏ��B
		if (m_eightVertices[i].vertex[j].x < xmin) {
			xmin = m_eightVertices[i].vertex[j].x;
		}
		//y�̍ŏ��B
		if (m_eightVertices[i].vertex[j].y < ymin) {
			ymin = m_eightVertices[i].vertex[j].y;
		}
		//x�̍ő�B
		if (m_eightVertices[i].vertex[j].x > xmax) {
			xmax = m_eightVertices[i].vertex[j].x;
		}
		//y�̍ő�B
		if (m_eightVertices[i].vertex[j].y > ymax) {
			ymax = m_eightVertices[i].vertex[j].y;
		}
		//z�̍ő�B
		if (m_eightVertices[i].vertex[j].z > zmax) {
			zmax = m_eightVertices[i].vertex[j].z;
		}
	}

	//�v���W�F�N�V�����s������B
	float w = xmax - xmin;		//���B
	float h = ymax - ymin;		//�����B

	m_lightProjMatrix[i].MakeOrthoProjectionMatrix(
		w,
		h,
		1.0f,
		zmax
	);
}

void CascadeShadow::Render(int i)
{
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_shadowMapRT[i].GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT[i].GetDepthStensilView());

	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[i].GetViewport());
	//�V���h�E�}�b�v���N���A�B
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT[i].ClearRenderTarget(clearColor);

	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnShadowMap);
	//�V���h�E�L���X�^�[���V���h�E�}�b�v�Ƀ����_�����O�B
	for (auto& caster : m_shadowCasters) {
		caster->Draw(
			m_lightViewMatrix[i],
			m_lightProjMatrix[i]
		);
	}
	g_graphicsEngine->SetRenderMode(GraphicsEngine::EnNormal);

}
