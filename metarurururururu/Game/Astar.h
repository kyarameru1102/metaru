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
private:
	NaviMesh* m_naviMesh = nullptr;
};

