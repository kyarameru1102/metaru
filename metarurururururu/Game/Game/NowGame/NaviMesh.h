#pragma once
#include "physics/CapsuleCollider.h"
#include "SkinModelRender.h"
#include "graphics/SkinModel.h"
//�i�r�Q�[�V�������b�V���B
struct Cell {
	CVector3			vertexPos[3];					//���_���W�B
	Cell*				linkCells[3] = {nullptr};		//�אڂ��Ă���Z���B
	Cell*				parent = nullptr;				//�Ђ炩�ꂽ���̐e�̃Z�����L�^����B
	CVector3			centerPos = CVector3::Zero();	//�Z���̒��S���W
	//�R�X�g��ݒ肷��֐��B
	void SetCost(float cost)
	{
		Cost = cost;
	}
	void SetFromStartCost(float cost)
	{
		FromStartCost = cost;
	}
	//�����N���Ă���Z���̐���Ԃ��Ă���֐��B
	short GetLinkCellMax() const
	{
		return linkMax;
	}
	//�����N�Z�����擾�B
	Cell* GetLinkCell(short i) const
	{
		return linkCells[i];
	}
	//
	float GetCost() const
	{
		return Cost;
	}
	float GetFromStartCost() const
	{
		return FromStartCost;
	}
	float GetPassingCellCost() const
	{
		return passingCellCost;
	}
	float				Cost = FLT_MAX;
	float				FromStartCost = FLT_MAX;
	short				linkMax = 0;					//�����N���Ă���Z���̐��B
	float				passingCellCost = 0;			//���̃Z����ʂ邽�߂̃R�X�g
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
	void Update();
	Cell* Findcell(CVector3 pos)
	{
		CVector3 closepos;
		closepos.x = FLT_MAX;
		closepos.y = FLT_MAX;
		closepos.z = FLT_MAX;
		Cell* cell = nullptr;
		for (auto Node : m_cells) {
			auto Pos = Node->centerPos - pos;
			if (closepos.Length() >= Pos.Length()) {
				cell = Node;
				closepos = Pos;
			}
		}
		return cell;
	}
	std::vector<Cell*> GetCells() const
	{
		return m_cells;
	}
private:
	std::vector<Cell*>	m_cells;
	SkinModelRender*	m_skin = nullptr;
	SkinModelRender*	m_skin2 = nullptr;
	CapsuleCollider		m_collider;
	SkinModel			m_model;
	bool				m_debugCount = false;
	int					m_debugCount2 = 0;
};
static NaviMesh& GetNaviMesh()
{
	return NaviMesh::GetNaviMesh();
}
