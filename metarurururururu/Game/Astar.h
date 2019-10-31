#pragma once
#include "NaviMesh.h"
class Astar
{
public:
	Astar();
	~Astar();
	bool Execute(
		const CVector3& startPos,
		const CVector3& targetPos
	);
	CVector3 GetAStarAnswerPos()
	{
		if (AStarAnswerIt == AStarAnswer.end()) {
			return CVector3::Zero();
		}
		return (*AStarAnswerIt)->centerPos;
	}
	bool AdvanceIt() {
		if (AStarAnswerIt != AStarAnswer.end()) {
		AStarAnswerIt++;
			return true;
		}
		else {
			return false;
		}
	}
private:
	NaviMesh* m_naviMesh = nullptr;
	std::list<Cell*> AStarAnswer;
	std::list<Cell*>::iterator AStarAnswerIt;
};

