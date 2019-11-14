#pragma once
#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"

class Ground : public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
private:
	SkinModelRender*	m_skinModelRender = nullptr;
	MeshCollider		m_collider;
	CVector3			m_position = CVector3::Zero();
	CQuaternion			m_rotation = CQuaternion::Identity();
	CVector3			m_scale = CVector3::One();
	RigidBody			m_rigidBody;
};

