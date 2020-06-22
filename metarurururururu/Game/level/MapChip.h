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
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~MapChip()
	{
		DeleteGO(m_skinModelRender);
	}
	/*
	*@param[in] objData			�I�u�W�F�N�g���B
	*/
	MapChip(const LevelObjectData& objData);
	
private:
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[�B
	PhysicsStaticObject m_physicsStaticObject;	//!<�ÓI�����I�u�W�F�N�g�B
	ID3D11ShaderResourceView* m_specMapSRV = nullptr;		//�X�y�L�����p�V�F�[�_�[���\�[�X�r���[�B
};