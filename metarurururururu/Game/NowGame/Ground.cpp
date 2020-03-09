#include "stdafx.h"
#include "Ground.h"
#include "physics/CollisionAttr.h"

Ground::Ground()
{
}


Ground::~Ground()
{
	DeleteGO(m_skinModelRender1);
	DeleteGO(m_skinModelRender2);
	DeleteGO(m_absoluteWall);
	g_physics.RemoveRigidBody(m_rigidBody);
	g_physics.RemoveRigidBody(m_absoluteWallRigidBody);
}

bool Ground::Start()
{
	m_skinModelRender1 = NewGO<SkinModelRender>(0);
	m_skinModelRender1->Init(L"Assets/modelData/ground_test.cmo");
	m_skinModelRender2 = NewGO<SkinModelRender>(0);
	m_skinModelRender2->Init(L"Assets/modelData/ground_back.cmo");
	m_absoluteWall = NewGO<SkinModelRender>(0);
	m_absoluteWall->Init(L"Assets/modelData/AbsoluteWall.cmo");

	CMatrix mTranse, mRot, mScale, mWorld;
	//平行移動行列を作成。
	mTranse.MakeTranslation(m_position);
	//回転行列を作成。
	mRot.MakeRotationFromQuaternion(m_rotation);
	//拡大行列を作成。
	mScale.MakeScaling(m_scale);
	//ワールド行列を作成。
	mWorld.Mul(mScale, mRot);
	mWorld.Mul(mWorld, mTranse);
	//コライダーに登録
	m_collider.CreateFromSkinModel(m_skinModelRender1->GetSkinModel(),&mWorld);
	m_absoluteWallCollider.CreateFromSkinModel(m_absoluteWall->GetSkinModel(), &mWorld);
	//剛体を生成。
	RigidBodyInfo rb;
	rb.collider = &m_collider;
	rb.mass = 0.0f;
	m_rigidBody.Create(rb);
	rb.collider = &m_absoluteWallCollider;
	m_absoluteWallRigidBody.Create(rb);
	//コリジョン属性変更
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_ground);
	m_absoluteWallRigidBody.GetBody()->setUserIndex(enCollisionAttr_ground);

	g_physics.AddRigidBody(m_rigidBody);
	g_physics.AddRigidBody(m_absoluteWallRigidBody);
	
	return true;
}

void Ground::Update()
{
}
