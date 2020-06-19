#include "stdafx.h"
#include "Sky.h"


Sky::Sky()
{
}


Sky::~Sky()
{
	DeleteGO(m_skinModelRender);
	m_skyCube->Release();
}

bool Sky::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/sky.cmo");
	if (m_skyCube != nullptr) {
		m_skyCube->Release();
		m_skyCube = nullptr;
	}
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/modelData/skyCubeMap.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_skyCube);
	//�Q�ƃJ�E���^���グ�Ă����Ȃ��ƁA����ς݂̃e�N�X�`���ɃA�N�Z�X�����Ⴄ�B
	if (m_skyCube != nullptr) {
		m_skyCube->AddRef();
	}
	m_psSkyShader.Load("Assets/shader/model.fx", "PSMain_SkyCube", Shader::EnType::PS);
	//�}�e���A���ɃN�G���������āA�V�F�[�_�[�ƃe�N�X�`���������ւ��邺�B
	m_skinModelRender->FindMaterial([&](ModelEffect* mat) {
		mat->SetRender3DModelPSShader(m_psSkyShader);
		mat->SetAlbedoTexture(m_skyCube);
		});
	return true;
}

void Sky::Update()
{
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetPosition(m_position);
}
