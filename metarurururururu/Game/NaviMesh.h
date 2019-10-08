#pragma once
#include "physics/CapsuleCollider.h"
#include "SkinModelRender.h"
//�i�r�Q�[�V�������b�V���B
struct Cell {
	CVector3			vertexPos[4];
	Cell*				linkCells[4];		//�אڂ��Ă���Z���B
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

