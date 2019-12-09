#include "stdafx.h"
#include "sensya.h"


sensya::sensya()
{
}


sensya::~sensya()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_sensyaCollider);
}

bool sensya::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_sensyaCollider = NewGO<SkinModelRender>(0);

	m_skinModelRender->Init(L"Assets/modelData/sensya.cmo");
	m_sensyaCollider->Init(L"Assets/modelData/sensyaCollider.cmo");

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);

	m_sensyaCollider->SetPosition(m_position);
	m_sensyaCollider->SetRotation(m_rotation);

	m_sensyaCollider->SetRenderOn(false);

	//���b�V���R���C�_�[���쐬�B
	m_meshCollider.CreateFromSkinModel(m_sensyaCollider->GetSkinModel(), nullptr);
	//���̂��쐬�B
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider;//���̂̌`���ݒ肷��B
	rbInfo.mass = 0.0f;
	rbInfo.pos = m_position;
	rbInfo.rot = m_rotation;
	rbInfo.scl = m_scl;
	m_rigidBody.Create(rbInfo);
	//���̂𕨗����[���h�ɓo�^����B
	g_physics.AddRigidBody(m_rigidBody);

	return true;
}

void sensya::Update()
{
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}
