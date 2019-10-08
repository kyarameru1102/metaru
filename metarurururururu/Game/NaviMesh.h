#pragma once
#include "physics/CapsuleCollider.h"
#include "SkinModelRender.h"
//ナビゲーションメッシュ。
struct Cell {
	CVector3			vertexPos[4];
	Cell*				linkCells[4];		//隣接しているセル。
};
class NaviMesh
{
public:
	NaviMesh();
	~NaviMesh();
	void Create(SkinModel& model);
private:
	std::vector<Cell*>	m_cells;
	SkinModelRender*	m_skin = nullptr;
	CapsuleCollider		m_collider;
};

