#include "stdafx.h"
#include "Bloom.h"


struct SVertex {
	float position[4];		//���_���W�B
	float uv[2];			//UV���W�B���ꂪ�e�N�X�`�����W
};


Bloom::Bloom()
{
	//�����_�����O�^�[�Q�b�g�̏������B
	//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_luminanceRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	//�u���[�������邽�߂̃_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g���쐬�B
	//���u���[�p�B
	m_downSamplingRT[0].Create(
		FRAME_BUFFER_W * 0.5f,	//���̉𑜓x���t���[���o�b�t�@�̔����ɂ���B
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	//�c�u���[�p�B
	m_downSamplingRT[1].Create(
		FRAME_BUFFER_W * 0.5f,	//���̉𑜓x���t���[���o�b�t�@�̔����ɂ���B
		FRAME_BUFFER_H * 0.5f,	//�c�̉𑜓x���t���[���o�b�t�@�̔����ɂ���B
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	//�V�F�[�_�[�̏������B
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_psLuminance.Load("Assets/shader/bloom.fx", "PSSamplingLuminance", Shader::EnType::PS);
	m_vsXBlur.Load("Assets/shader/bloom.fx", "VSXBlur", Shader::EnType::VS);
	m_vsYBlur.Load("Assets/shader/bloom.fx", "VSYBlur", Shader::EnType::VS);
	m_psBlur.Load("Assets/shader/bloom.fx", "PSBlur", Shader::EnType::PS);
	m_psFinal.Load("Assets/shader/bloom.fx", "PSFinal", Shader::EnType::PS);

	//a�u�����f�B���O�X�e�[�g���������B
	CD3D11_DEFAULT defaultSettings;
	//�f�t�H���g�Z�b�e�B���O�ŏ���������B
	CD3D11_BLEND_DESC blendDesc(defaultSettings);
	auto device = g_graphicsEngine->GetD3DDevice();

	device->CreateBlendState(&blendDesc, &m_disableBlendState);

	//�ŏI�����p�̃u�����h�X�e�[�g���쐬����B
	//�ŏI�����͉��Z�����B
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	device->CreateBlendState(&blendDesc, &m_finalBlendState);

	//�萔�o�b�t�@�̏������B
	D3D11_BUFFER_DESC Bdesc;
	ZeroMemory(&Bdesc, sizeof(Bdesc));
	Bdesc.Usage = D3D11_USAGE_DEFAULT;
	Bdesc.ByteWidth = (((sizeof(SBlurParam) - 1) / 16) + 1) * 16;	//16�o�C�g�A���C�����g�ɐ؂肠����B
	Bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Bdesc.CPUAccessFlags = 0;
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&Bdesc, NULL, &m_blurParamCB);

	//�T���v���X�e�[�g�̏������B
	D3D11_SAMPLER_DESC Sdesc;
	ZeroMemory(&Sdesc, sizeof(Sdesc));
	Sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	Sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	Sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&Sdesc, &m_samplerState);

	//�t���X�N���[���`��̂��߂̎l�p�`�v���~�e�B�u���������B
	//���_�o�b�t�@���������B
	SVertex vertex[4] = {
		//���_�P
		{
			//���W�@position[4]
			-1.0f,  -1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			0.0f, 1.0f
		},
		//���_�Q
		{
			//���W�@position[4]
			1.0f, -1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			1.0f, 1.0f
		},
		//���_�R
		{
			//���W�@position[4]
			-1.0f,  1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			0.0f, 0.0f
		},
		//���_�S
		{
			//���W�@position[4]
			1.0f,  1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			1.0f, 0.0f
		},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//�\���̂�0�ŏ���������B
	bd.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@�B
												//��肠������D3D11_USAGE_DEFAULT�ł悢�B
	bd.ByteWidth = sizeof(vertex);				//���_�o�b�t�@�̃T�C�Y�B���_�̃T�C�Y�~���_���ƂȂ�B
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���ꂩ��쐬����o�b�t�@�����_�o�b�t�@�ł��邱�Ƃ��w�肷��B

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//���_�o�b�t�@�̍쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
}


Bloom::~Bloom()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
}


void Bloom::Update()
{
	//�K�E�X�t�B���^�̏d�݂��X�V����B
	float total = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_blurParam.weights[i] = expf(-0.5f*(float)(i*i) / m_blurDispersion);
		total += 2.0f*m_blurParam.weights[i];

	}
	// �K�i���B�d�݂̃g�[�^����1.0�ɂȂ�悤�ɁA�S�̂̏d�݂ŏ��Z���Ă���B
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_blurParam.weights[i] /= total;
	}
}

void Bloom::Drow(RenderTarget& renderTarget)
{
	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//�P�x�𒊏o�B
	//���u�����h�𖳌��ɂ���B
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);

	//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX����B
	g_graphicsEngine->ChangeRenderTarget(&m_luminanceRT, m_luminanceRT.GetViewport());
	//�����_�����O�^�[�Q�b�g�̃N���A�B
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_luminanceRT.ClearRenderTarget(clearColor);

	//�V�[�����e�N�X�`���Ƃ���B
	auto mainRTTexSRV = renderTarget.GetRenderTargetSRV();
	deviceContext->PSSetShaderResources(0, 1, &mainRTTexSRV);

	//�t���X�N���[���`��B
	//�v���~�e�B�u�̃g�|���W�[�Ƃ��āA�g���C�A���O���X�g���b�v��ݒ肷��B
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	unsigned int vertexSize = sizeof(SVertex);
	unsigned int offset = 0;
	//�P�x���o�p�̃V�F�[�_�[��ݒ肷��B
	deviceContext->VSSetShader(
		(ID3D11VertexShader*)m_vs.GetBody(), nullptr, 0
	);
	deviceContext->PSSetShader(
		(ID3D11PixelShader*)m_psLuminance.GetBody(), nullptr, 0
	);
	deviceContext->IASetInputLayout(m_vs.GetInputLayout());

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->Draw(4, 0);
}
