#pragma once
#include "physics/ICollider.h"

//���̂̃R���C�_�[�B
class SphereCollider : public ICollider
{
public:
	SphereCollider();
	~SphereCollider();
	/// <summary>
	/// ���̃R���C�_�[�쐬�N���X�B
	/// </summary>
	/// <param name="radius">
	/// ���̂̔��a�B
	/// </param>
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
private:
	btSphereShape*		shape = NULL;
};

