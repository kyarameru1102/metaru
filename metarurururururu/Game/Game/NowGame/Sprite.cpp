#include "stdafx.h"
#include "Sprite.h"

struct SVertex {
	float position[4];		//���_���W�B4�v�f�Ȃ͍̂��͋C�ɂ��Ȃ��B
	float uv[2];			//UV���W�B���ꂪ�e�N�X�`�����W
};

Sprite::Sprite()
{
	//�쐬����u�����h�X�e�[�g�̏���ݒ肷��B
	CD3D11_DEFAULT defaultSettings;
	//�f�t�H���g�Z�b�e�B���O�ŏ���������B
	CD3D11_BLEND_DESC blendDesc(defaultSettings);

	//���u�����f�B���O��L���ɂ���B
	blendDesc.RenderTarget[0].BlendEnable = true;

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//�������������s����u�����h�X�e�[�g���쐬�ł���B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBlendState(&blendDesc, &m_blendState);

	CreateDepthStencilState();
}


Sprite::~Sprite()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer != nullptr) {
		m_indexBuffer->Release();
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
	if (m_texture != nullptr) {
		m_texture->Release();
	}
	if (m_cb != nullptr) {
		m_cb->Release();
	}
	if (m_blendState != nullptr) {
		m_blendState->Release();
	}
}

void Sprite::Init(const wchar_t * texFilePath, float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	//���_�o�b�t�@�̏������B
	InitVertexBuffer(w, h);
	//�C���f�b�N�X�o�b�t�@�̏������B
	InitIndexBuffer();
	//�T���v���X�e�[�g�̏������B
	InitSamplerState();
	//�V�F�[�_�[�̃��[�h�B
	m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);

	//�萔�o�b�t�@���������B
	InitConstantBuffer();

	//�e�N�X�`�������[�h�B
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),	//D3D�f�o�C�X�B
		texFilePath,						//�ǂݍ��މ摜�f�[�^�̃t�@�C���p�X�B
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_texture					//�ǂݍ��񂾃e�N�X�`����
									//�A�N�Z�X���邽�߂̃C���^�[�t�F�[�X�̊i�[��B
	);
	
	////�X�e�[�g��ۑ��B
	//m_DC = g_graphicsEngine->GetD3DDeviceContext();
	//GetRenderState();
}
void Sprite::Init(ID3D11ShaderResourceView* srv, float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	//���_�o�b�t�@�̏������B
	InitVertexBuffer(w, h);
	//�C���f�b�N�X�o�b�t�@�̏������B
	InitIndexBuffer();
	//�T���v���X�e�[�g�̏������B
	InitSamplerState();
	//�V�F�[�_�[�̃��[�h�B
	m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);

	//�萔�o�b�t�@���������B
	InitConstantBuffer();

	m_texture = srv;
	m_texture->AddRef();	//�Q�ƃJ�E���^�𑝂₷�B
}
void Sprite::Update(const CVector3 & trans, const CQuaternion & rot, const CVector3 & scale, CVector2 pivot)
{
	//�s�{�b�g�͐^�񒆂�0.0, 0.0�A���オ-1.0f, -1.0�A�E����1.0�A1.0�ɂȂ�悤�ɂ���B
	CVector2 localPivot = pivot;
	localPivot.x -= 0.5f;
	localPivot.y -= 0.5f;
	localPivot.x *= -2.0f;
	localPivot.y *= -2.0f;
	//�摜�̃n�[�t�T�C�Y�����߂�B
	CVector2 halfSize = m_size;
	halfSize.x *= 0.5f;
	halfSize.y *= 0.5f;
	CMatrix PivotTrans;

	PivotTrans.MakeTranslation(
		{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
	);
	CMatrix Trans, Rot, Scale;
	Trans.MakeTranslation(trans);
	Rot.MakeRotationFromQuaternion(rot);
	Scale.MakeScaling(scale);
	m_world.Mul(PivotTrans, Scale);
	m_world.Mul(m_world, Rot);
	m_world.Mul(m_world, Trans);
}

void Sprite::Draw()
{
	//�����_�����O�X�e�[�g��ۑ��B
	m_DC = g_graphicsEngine->GetD3DDeviceContext();

	SetRenderState();
	//GetRenderState();

	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

	unsigned int vertexSize = sizeof(SVertex);	//���_�̃T�C�Y�B
	unsigned int offset = 0;
	d3dDeviceContext->IASetVertexBuffers(	//���_�o�b�t�@��ݒ�B
		0,					//StartSlot�ԍ��B����0�ł����B
		1,					//�o�b�t�@�̐��B����1�ł����B
		&m_vertexBuffer,	//���_�o�b�t�@�B
		&vertexSize,		//���_�̃T�C�Y�B
		&offset				//�C�ɂ��Ȃ��Ă悢�B
	);
	d3dDeviceContext->IASetIndexBuffer(	//�C���f�b�N�X�o�b�t�@��ݒ�B
		m_indexBuffer,			//�C���f�b�N�X�o�b�t�@�B
		DXGI_FORMAT_R32_UINT,	//�C���f�b�N�X�̃t�H�[�}�b�g�B
								//�����32bit�Ȃ̂ŁADXGI_FORMAT_R32_UINT�ł����B
		0						//�I�t�Z�b�g0�ł����B
	);
	//�V�F�[�_�[��ݒ�B
	d3dDeviceContext->VSSetShader(
		(ID3D11VertexShader*)m_vs.GetBody(),
		nullptr,
		0
	);
	d3dDeviceContext->PSSetShader(
		(ID3D11PixelShader*)m_ps.GetBody(),
		nullptr,
		0
	);
	d3dDeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	//�e�N�X�`����ݒ�B
	d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	//�T���v���X�e�[�g��ݒ�B
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//���[���h�r���[�v���W�F�N�V�����s����쐬�B
	ConstantBuffer cb;
	cb.WVP = m_world;
	cb.WVP.Mul(cb.WVP, g_camera2D.GetViewMatrix());
	cb.WVP.Mul(cb.WVP, g_camera2D.GetProjectionMatrix());
	cb.alpha = m_alpha;

	d3dDeviceContext->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//�v���~�e�B�u�̃g�|���W�[��
	//�g���C�A���O���X�g���b�v��ݒ肷��B
	d3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDeviceContext->DrawIndexed(	//�`�施�߁B
		6,				//�C���f�b�N�X���B
		0,				//�J�n�C���f�b�N�X�ԍ��B0�ł����B
		0				//�J�n���_�ԍ��B0�ł����B
	);
	
	//�����_�[�X�e�[�g��ݒ肵�Ȃ����B
	//SetRenderState();
}

void Sprite::InitConstantBuffer()
{
	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = (((sizeof(ConstantBuffer) - 1) / 16) + 1) * 16;	//16�o�C�g�A���C�����g�ɐ؂肠����B
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, NULL, &m_cb);
}

void Sprite::InitVertexBuffer(float w, float h)
{
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	SVertex vertex[4] = {
		//vertex[0]
		{
			//position[4]
			-halfW,  -halfH, 0.0f, 1.0f,
			//uv[2]
			0.0f, 1.0f
		},
		//vertex[1]
		{
			//position[4]
			halfW, -halfH, 0.0f, 1.0f,
			//uv[2]
			1.0f, 1.0f
		},
		//vertex[2]
		{
			//position[4]
			-halfW,  halfH, 0.0f, 1.0f,
			//uv[2]
			0.0f, 0.0f
		},
		//vertex[3]
		{
			//position[4]
			halfW,  halfH, 0.0f, 1.0f,
			//uv[2]
			1.0f, 0.0f
		},
	};
	//��Œ�`�������_���g�p���Ē��_�o�b�t�@���쐬����B
	//���_�o�b�t�@���쐬���邽�߂ɂ�D3D11_BUFFER_DESC��D3D11_SUBRESOURCE_DATA��ݒ肷��K�v������B
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//�\���̂�0�ŏ���������B
	bd.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@�B
	bd.ByteWidth = sizeof(vertex);				//���_�o�b�t�@�̃T�C�Y�B���_�̃T�C�Y�~���_���ƂȂ�B
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���ꂩ��쐬����o�b�t�@�����_�o�b�t�@�ł��邱�Ƃ��w�肷��B

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//���_�o�b�t�@�̍쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
}

void Sprite::InitIndexBuffer()
{
	//���ꂪ�C���f�b�N�X�B���_�ԍ��B
	int index[6] = {
		0,1,2,		//�O�p�`���
		2,1,3		//�O�p�`���
	};
	//��Œ�`�����C���f�b�N�X���g�p���ăC���f�b�N�X�o�b�t�@���쐬����B
	//�C���f�b�N�X�o�b�t�@�̍쐬�����_�o�b�t�@�Ɠ��l�ɁA
	//D3D11_BUFFER_DESC��D3D11_SUBRESOURCE_DATA��ݒ肷��K�v������B
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//�\���̂�0�ŏ���������B
	bd.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@�B
												//��肠������D3D11_USAGE_DEFAULT�ł悢�B
	bd.ByteWidth = sizeof(index);				//�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//���ꂩ��쐬����o�b�t�@���C���f�b�N�X�o�b�t�@�ł��邱�Ƃ��w�肷��B

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index;

	//�C���f�b�N�X�o�b�t�@�̍쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(
		&bd, &InitData, &m_indexBuffer
	);
}

void Sprite::InitSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void Sprite::SetRenderState()
{
	m_DC->OMSetDepthStencilState(m_depthState, 0);
	m_DC->OMSetBlendState(m_blendState, nullptr, 0xFFFFFFFF);
}

void Sprite::GetRenderState()
{
	m_DC->OMGetDepthStencilState(&m_depthState, 0);
	float blendfactor[4];
	UINT mask;
	m_DC->OMGetBlendState(&m_blendState, blendfactor, &mask);
}

void Sprite::CreateDepthStencilState()
{
	//D3D�f�o�C�X���擾�B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	//�쐬����[�x�X�e���V���X�e�[�g�̒�`��ݒ肵�Ă����B
	D3D11_DEPTH_STENCIL_DESC desc = { 0 };
	desc.DepthEnable = false;					 //Z�e�X�g���L���B
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //Z�o�b�t�@��Z�l��`�����ށB
	desc.DepthFunc = D3D11_COMPARISON_LESS;		 //Z�l����������΃t���[���o�b�t�@�ɕ`�����ށB
	   //�f�v�X�X�e���V���X�e�[�g���쐬�B
	d3dDevice->CreateDepthStencilState(&desc, &m_depthState);
}