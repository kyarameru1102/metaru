#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"

MapChip::MapChip(const LevelObjectData& objData)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	wchar_t filePath[256];
	swprintf_s(filePath, L"Assets/modelData/%s.cmo", objData.name);
	m_skinModelRender->Init(filePath);
	m_skinModelRender->SetPosition(objData.position);
	m_skinModelRender->SetRotation(objData.rotation);
	m_skinModelRender->SetScale(objData.scale);
	//�ÓI�����I�u�W�F�N�g�����b�V���R���C�_�[����쐬����B
	m_physicsStaticObject.CreateMeshObject(m_skinModelRender->GetSkinModel(), objData.position, objData.rotation,objData.scale);
}

