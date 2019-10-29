#pragma once
#include "physics/CapsuleCollider.h"
#include "SkinModelRender.h"
#include "graphics/SkinModel.h"
//�i�r�Q�[�V�������b�V���B
struct Cell {
	CVector3			vertexPos[3];
	Cell*				linkCells[3];		//�אڂ��Ă���Z���B
	Cell*				parent;				//�Ђ炩�ꂽ���̐e�̃Z�����L�^����B
	CVector3			centerPos;
	//�R�X�g��ݒ肷��֐��B
	void SetCost(float cost)
	{
		Cost = cost;
	}
	//�����N���Ă���Z���̐���Ԃ��Ă���֐��B
	auto GetLinkCellMax()
	{
		return linkMax;
	}
	//�����N�Z�����擾�B
	auto GetLinkCell(short i)
	{
		return linkCells[i];
	}
	//
	auto GetCost()
	{
		return Cost;
	}
	float				Cost;
	short				linkMax = 0;			//�����N���Ă���Z���̐��B
};
class NaviMesh
{
public:
	NaviMesh();
	~NaviMesh();
	static inline NaviMesh& GetNaviMesh()
	{ 
		static NaviMesh r; 
		return r;
	};
	void Create(SkinModel& model);
	Cell* Findcell(CVector3 pos)
	{
		CVector3 closepos;
		closepos.x = FLT_MAX;
		closepos.y = FLT_MAX;
		closepos.z = FLT_MAX;
		Cell* cell = nullptr;
		for (auto Node : m_cells) {
			auto Pos = cell->centerPos - pos;
			if (closepos.Length() >= Pos.Length()) {
				cell = Node;
			}
		}
		return cell;
	}
private:
	std::vector<Cell*>	m_cells;
	SkinModelRender*	m_skin = nullptr;
	CapsuleCollider		m_collider;
	SkinModel			m_model;
	
};
static NaviMesh& GetNaviMesh()
{
	return NaviMesh::GetNaviMesh();
}

