#pragma once
#include "physics/ICollider.h"

//球体のコライダー。
class SphereCollider : public ICollider
{
public:
	SphereCollider();
	~SphereCollider();
	/// <summary>
	/// 球体コライダー作成クラス。
	/// </summary>
	/// <param name="radius">
	/// 球体の半径。
	/// </param>
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
private:
	btSphereShape*		shape = NULL;
};

