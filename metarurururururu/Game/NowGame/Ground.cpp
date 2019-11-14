#include "stdafx.h"
#include "Ground.h"
#include "physics/CollisionAttr.h"

Ground::Ground()
{
}


Ground::~Ground()
{
	DeleteGO(m_skinModelRender);
	g_physics.RemoveRigidBody(m_rigidBody);
}

bool Ground::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(L"Assets/modelData/ground_test.cmo");

	CMatrix mTranse, mRot, mScale, mWorld;
	//���s�ړ��s����쐬�B
	mTranse.MakeTranslation(m_position);
	//��]�s����쐬�B
	mRot.MakeRotationFromQuaternion(m_rotation);
	//�g��s����쐬�B
	mScale.MakeScaling(m_scale);
	//���[���h�s����쐬�B
	mWorld.Mul(mScale, mRot);
	mWorld.Mul(mWorld, mTranse);
	m_collider.CreateFromSkinModel(m_skinModelRender->GetSkinModel(),&mWorld);
	RigidBodyInfo rb;
	rb.collider = &m_collider;
	rb.mass = 0.0f;
	m_rigidBody.Create(rb);

	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_ground);
	g_physics.AddRigidBody(m_rigidBody);
	
	return true;
}

void Ground::Update()
{
}
