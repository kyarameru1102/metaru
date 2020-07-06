#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;

/*!
* @brief	マップチップ。
*/
class MapChip : public IGameObject
{
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~MapChip()
	{
		DeleteGO(m_skinModelRender);
	}
	/*
	*@param[in] objData			オブジェクト情報。
	*/
	MapChip(const LevelObjectData& objData);
	
private:
	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー。
	PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	ID3D11ShaderResourceView* m_specMapSRV = nullptr;		//スペキュラ用シェーダーリソースビュー。
};