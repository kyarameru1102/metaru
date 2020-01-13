#include "stdafx.h"
#include "Astar.h"

static inline Cell* PopMostLowCostCell(std::vector<Cell*>& cells)
{
	Cell* cell;
	float cost = FLT_MAX;

	std::vector<Cell*>::iterator itPopCell;
	std::vector<Cell*>::iterator itEnd = cells.end();
	for (
		std::vector<Cell*>::iterator itCell = cells.begin();
		itCell != itEnd;
		itCell++
		)
	{
		if ((*itCell)->Cost < cost) {
			//コストが低いを代入。
			itPopCell = itCell;
			cost = (*itCell)->Cost;
		}
	}

	cell = *itPopCell;
	cells.erase(itPopCell);
	return cell;

}

Astar::Astar()
{
	m_naviMesh = &GetNaviMesh();
}


Astar::~Astar()
{
}

bool Astar::Execute(const CVector3& startPos, const CVector3& targetPos)
{
	Cell* startCell;
	Cell* endCell;
	startCell = m_naviMesh->Findcell(startPos);
	endCell = m_naviMesh->Findcell(targetPos);
	Cell* crtCell = startCell;
	std::vector<Cell*> openCellList;
	std::vector<Cell*> closeCellList;

	std::vector<Cell*>::iterator openListIt, closeListIt;

	//始めのセルをオープンリストに登録。
	openCellList.push_back(startCell);

	//スタートからエンドまでの距離を計算。
	CVector3 toTarget;
	toTarget = endCell->centerPos - startCell->centerPos;
	toTarget.y = 0.0f;
	float startToTargetSq = toTarget.LengthSq();
	
	crtCell->SetCost(startToTargetSq);
	crtCell->SetFromStartCost(0.0);

	Cell* linkCell;

	//オープンセルが空じゃなかったら。
	while (!openCellList.empty()) {
		//オープンリストから最も安いセルを取得。
		crtCell = PopMostLowCostCell(openCellList);
		if (crtCell == endCell) {	
			//現在のセルをクローズリストに追加。
			closeCellList.push_back(crtCell);
			//A*アルゴリズム成功。
			break;
		}
		else {
			//現在のセルをクローズリストに追加。
			closeCellList.push_back(crtCell);
			//リンクしているセルの数を取得。
			auto linkMax = crtCell->GetLinkCellMax();
			for (short i = 0; i < 3; ++i) {
				linkCell = crtCell->GetLinkCell(i);
				if (!linkCell) {
					//隣接セルがなかったら。
					continue;
				}
				openListIt = std::find(
					openCellList.begin(),
					openCellList.end(),
					linkCell
				);

				closeListIt = std::find(
					closeCellList.begin(),
					closeCellList.end(),
					linkCell
				);

				//コスト計算。
				float fromStartCost = (linkCell->centerPos - crtCell->centerPos).Length() + crtCell->GetFromStartCost();
				linkCell->SetFromStartCost(fromStartCost);
				float toEndCost = (endCell->centerPos - linkCell->centerPos).Length();
				toEndCost = toEndCost * linkCell->GetPassingCellCost();
				float cost = fromStartCost + toEndCost;
				if ((openListIt != openCellList.end() && cost > (*openListIt)->GetCost())
					|| (closeListIt != closeCellList.end() && cost > (*closeListIt)->GetCost()))
				{
					continue;
				}
				else {
					linkCell->SetCost(cost);
					linkCell->parent = crtCell;
					if (closeListIt != closeCellList.end()) {
						closeCellList.erase(closeListIt);
						openCellList.push_back(linkCell);
					}
					else if (openListIt == openCellList.end()) {
						openCellList.push_back(linkCell);
					}
				}
			}
		}
	}
	
	AStarAnswer.clear();
	
	Cell* pCell = endCell;
	while (startCell != pCell)
	{
		if (pCell->parent == nullptr) {
			int hoge = 0;
			return true;
		}
		/*std::list<Cell*>::iterator kari = std::find(
			AStarAnswer.begin(),
			AStarAnswer.end(),
			pCell
		);
		if (kari == AStarAnswerIt) {

		}*/
		AStarAnswer.push_back(pCell);
		//親のセルに移動する
		pCell = pCell->parent;
	}

		AStarAnswer.push_back(startCell);
		std::reverse(AStarAnswer.begin(), AStarAnswer.end());
		AStarAnswerIt = AStarAnswer.begin();
		return true;
	
}
