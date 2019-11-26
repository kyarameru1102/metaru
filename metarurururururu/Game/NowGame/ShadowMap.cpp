#include "stdafx.h"
#include "ShadowMap.h"


ShadowMap::ShadowMap()
{
	//�V���h�E�}�b�v�����p�̃����_�����O�^�[�Q�b�g���쐬�B
	//�𑜓x��2048�~2048�B
	//�e�N�X�`���̃t�H�[�}�b�g��R�����݂̂�32bit���������_�^�B
	m_shadowMapRT.Create(
		9182,
		9182,
		DXGI_FORMAT_R32_FLOAT
	);
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::UpdateFromLightTarget(CVector3 lightPos, CVector3 lightTarget)
{
	m_lightCameraTarget = lightTarget;
	m_lightCameraPosition = lightPos;
	//���C�g�̕������v�Z����B
	auto lightDir = m_lightCameraTarget - m_lightCameraPosition;
	if (lightDir.Length() < 0.0001f) {
		//���C�g�J�����̒����_�Ǝ��_���߂�����B
		//���炭�o�O�Ȃ̂ŁA�N���b�V��������B
		std::abort();
	}
	//���K�����āA�����x�N�g���ɕϊ�����B
	lightDir.Normalize();
	//
	UpdateFromLightDir(lightPos, lightDir);
}

void ShadowMap::UpdateFromLightDir(CVector3 lightPos, CVector3 lightDir)
{
	//���C�g�̕����ɂ���āA���C�g�J�����̏���������߂�B
	CVector3 lightCameraUpAxis;
	if (fabsf(lightDir.y) > 0.99998f) {
		//�قڐ^��or�^���������Ă���̂ŁA1,0,0��������Ƃ���B
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		//�^��������ĂȂ��Ƃ��́A0,1,0��������Ƃ���B
		lightCameraUpAxis = CVector3::AxisY();
	}
	//�J�����̏���������܂����̂ŁA���C�g�r���[�s����v�Z����B
	m_lightViewMatrix.MakeLookAt(
		lightPos,
		lightPos + lightDir,
		lightCameraUpAxis
	);

	//���C�g�v���W�F�N�V�����s����쐬����B
	//���z������̉e�𗎂Ƃ������Ȃ�A���s���e�s����쐬����B
	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		5000,
		5000,
		0.1f,
		4000.0f
	);
}

void ShadowMap::RenderToShadowMap()
{
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
		m_shadowMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT.GetDepthStensilView());
	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT.GetViewport());
	//�V���h�E�}�b�v���N���A�B
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT.ClearRenderTarget(clearColor);

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
