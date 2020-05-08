#include "stdafx.h"
#include "sensya.h"
#include "Player.h"
#include "C4.h"
#include "Clear.h"
#include "ClearPoint.h"

sensya::sensya()
{
}


sensya::~sensya()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_sensyaCollider);
	g_physics.RemoveRigidBody(m_rigidBody);
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
	m_sensyaCollider->SetShadowCaster(false);

	m_meshCollider.CreateFromSkinModel(m_sensyaCollider->GetSkinModel(), nullptr);
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider; //剛体に形状(コライダー)を設定する。
	rbInfo.mass = 0.0f;
	rbInfo.pos = m_position;
	rbInfo.rot = m_rotation;
	rbInfo.scl = m_scl;
	m_rigidBody.Create(rbInfo);
	//剛体を物理ワールドに追加する。
	g_physics.AddRigidBody(m_rigidBody);

	m_player = FindGO<Player>("player");

	return true;
}

void sensya::Update()
{
	QueryGOs<C4>("c4", [&](C4* c4) 
		{
		if ((c4->GetPosition() - m_position).Length() <= 500.0f) {
			m_c4On = true;
		}
		return true;
		}
	);
	if (m_c4On) 
	{
		if (g_pad[0].IsPress(enButtonLB1) && g_pad[0].IsTrigger(enButtonB))
		{
			NewGO<ClearPoint>(0, "clearPoint");
			DeleteGO(this);
		}
	}
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
}
