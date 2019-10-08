#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;

/*!
* @brief	�}�b�v�`�b�v�B
*/
class MapChip : public IGameObject
{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*@param[in] objData			�I�u�W�F�N�g���B
	*/
	MapChip(const LevelObjectData& objData);
	
private:
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[�B
	PhysicsStaticObject m_physicsStaticObject;	//!<�ÓI�����I�u�W�F�N�g�B
	
};