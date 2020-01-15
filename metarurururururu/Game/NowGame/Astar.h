#pragma once
#include "NaviMesh.h"
class Astar
{
public:
	Astar();
	~Astar();
	/// <summary>
	/// A*���s�֐��BPath�f�[�^���쐬�B
	/// </summary>
	/// <param name="startPos">�X�^�[�g�B</param>
	/// <param name="targetPos">�S�[���B</param>
	/// <returns>true�Ő����B</returns>
	bool Execute(
		const CVector3& startPos,
		const CVector3& targetPos
	);
	/// <summary>
	/// Execute�ō쐬����Path�̃|�W�V������Ԃ��֐��B
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

