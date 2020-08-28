#include "stdafx.h"
#include "Title.h"
#include "Fade.h"
#include "AfterTitle.h"
#include "Ground.h"
#include "Sky.h"
#include "TitleCamera.h"

Title::Title()
{
	m_se.Init(L"Assets/sound/gunfire.wav");
}


Title::~Title()
{
	DeleteGO(m_ground);
	DeleteGO(m_sky);
	DeleteGO(m_titleCamera);
	DeleteGO(m_skinModelRender);
}

bool Title::Start()
{
	m_animClips[enAnimationClip_idle].Load(L"Assets/animData/heisi_idle_L.tka");
	m_animClips[enAnimationClip_idle].SetLoopFlag(true);

	m_sprite.Init(L"Assets/sprite/Break_Down_Tank.dds", 1400, 800);

	//スペキュラマップ。
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/modelData/heisi_spec4.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_specMapSRV);

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/heisi.cmo", true, m_animClips, enAnimationClip_Num, EnFbxUpAxis::enFbxUpAxisZ);
	m_skinModelRender->SetShadowReciever(true);
	m_skinModelRender->GetSkinModel().SetSpecularMap(m_specMapSRV, SkinModel::Alpha);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle, Body::enUpperBody, 0.0f);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle, Body::enLowerBody, 0.0f);

	//地面生成。
	m_ground = NewGO<Ground>(0);
	//カメラ生成。
	m_titleCamera = NewGO<TitleCamera>(0);
	//空生成。
	m_sky = NewGO<Sky>(0);
	m_sky->SetScale(m_skyScale);

	return true;
}

void Title::Update()
{

	if (g_pad->IsTrigger(enButtonA) && !m_on) {
		m_on = true;
		NewGO<AfterTitle>(0, "afterTitle");
		m_se.Play(false);
	}
	if (m_on) {
		m_count++;
		m_sprite.DeltaAlpha(-0.02f);
	}
	if (m_count >= 50) {
		DeleteGO(this);
	}
	CVector3 camera = CVector3(-1500.0f, 0.0f, 2300.0f);
	camera.Normalize();
	m_rotation.SetRotation({ 0.0f,1.0f,0.0f }, atan2f(-camera.x, -camera.z));
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
	//m_sprite.Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	g_camera2D.Update();
}

void Title::Render()
{
	//m_sprite.Draw();
}
