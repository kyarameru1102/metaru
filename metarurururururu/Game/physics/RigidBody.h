#pragma once



class ICollider;

//剛体情報。
struct RigidBodyInfo {

	CVector3 pos = CVector3::Zero();			//座標。
	CQuaternion rot = CQuaternion::Identity();	//回転。
	CVector3 scl = CVector3::One();

	ICollider* collider = nullptr;				//コライダー。
	float mass = 0.0f;							//質量。

};
//剛体クラス。
class RigidBody
{
	btRigidBody*			rigidBody = nullptr;		//剛体。
	btDefaultMotionState*	myMotionState = nullptr;	//モーションステート。
public:
	~RigidBody();
	void Release();
	void Create(RigidBodyInfo& rbInfo);
	btRigidBody* GetBody()
	{
		return rigidBody;
	}
	void SetPosition(const CVector3& pos)
	{
		btTransform trans;
		btVector3 btPos;
		pos.CopyTo(btPos);
		trans.setOrigin(btPos);
		rigidBody->setWorldTransform(trans);
	}
};

