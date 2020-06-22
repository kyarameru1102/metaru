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

	if (m_specMapSRV != nullptr)
	{
		m_specMapSRV->Release();
	}
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/modelData/WallSpec.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_specMapSRV);
	m_skinModelRender->GetSkinModel().SetSpecularMap(m_specMapSRV);
	
	//静的物理オブジェクトをメッシュコライダーから作成する。
	m_physicsStaticObject.CreateMeshObject(m_skinModelRender->GetSkinModel(), objData.position, objData.rotation,objData.scale);
}

