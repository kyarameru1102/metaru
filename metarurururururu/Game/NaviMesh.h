#pragma once
#include "physics/CapsuleCollider.h"
#include "SkinModelRender.h"
#include "graphics/SkinModel.h"
//ナビゲーションメッシュ。
struct Cell {
	CVector3			vertexPos[3];
	Cell*				linkCells[3];		//隣接しているセル。
	Cell*				parent;				//ひらかれた時の親のセルを記録する。
	CVector3			centerPos;
	//コストを設定する関数。
	void SetCost(float cost)
	{
		Cost = cost;
	}
	//リンクしているセルの数を返してくる関数。
	auto GetLinkCellMax()
	{
		return linkMax;
	}
	//リンクセルを取得。
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
	short				linkMax = 0;			//リンクしているセルの数。
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

