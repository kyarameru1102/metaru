#pragma once
#include "NaviMesh.h"
class Astar
{
public:
	Astar();
	~Astar();
	/// <summary>
	/// A*実行関数。Pathデータを作成。
	/// </summary>
	/// <param name="startPos">スタート。</param>
	/// <param name="targetPos">ゴール。</param>
	/// <returns>trueで成功。</returns>
	bool Execute(
		const CVector3& startPos,
		const CVector3& targetPos
	);
	/// <summary>
	/// Executeで作成したPathのポジションを返す関数。
	/// </summary>
	/// <returns></returns>
	CVector3 GetAStarAnswerPos() const
	{
		if (AStarAnswerIt == AStarAnswer.end()) {
			return CVector3::Zero();
		}
		if (AStarAnswerIt._Ptr->_Myval == nullptr) {
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
	auto GetAStarAnswerEnd() const
	{
		return AStarAnswer.end();
	}
	auto GetAStarAnswerIt() const
	{
		return AStarAnswerIt;
	}
private:
	bool isExecute = false;
	NaviMesh* m_naviMesh = nullptr;
	std::list<Cell*> AStarAnswer;
	std::list<Cell*>::iterator AStarAnswerIt;
};

