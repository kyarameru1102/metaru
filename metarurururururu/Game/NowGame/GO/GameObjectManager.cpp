#include "stdafx.h"
#include "GameObjectManager.h"

void GameObjectManager::Init(int gameObjectPrioMax)
{
	if (gameObjectPrioMax <= 255)
	{
		m_priorityMax = static_cast<GOPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectListArray.resize(gameObjectPrioMax);
	}
}

void GameObjectManager::Update()
{
	
	for (int i = 0; i < m_gameObjectListArray.size(); i++) {
		for (auto go : m_gameObjectListArray[i]) {
			if (go->GetStart() == false) {
				if (go->Start())
					go->StartCallEnd();
			}
			else {
				if (go->GetIsDead() == false) {
					go->Update();
				}
			}	
		}
	}
	
	CVector3 lightDer = { 1.0f, -2.0f, 1.0f, };
	ShadowMap::GetShadowMap().UpdateFromLightDir(m_lightPos, lightDer);
	ShadowMap::GetShadowMap().RenderToShadowMap();

	for (int i = 0; i < m_gameObjectListArray.size(); i++) {
		for (auto go : m_gameObjectListArray[i]) {			
			if (go->GetIsDead() == false) {
				go->Render();
			}
		}
	}
	for(auto& deleteList : m_deleteObjectListArray){
		for (IGameObject* go : deleteList) {
			if (go->GetIsDead() == true)
			{
				GOPrio prio = go->GetPriority();
				GameObjectList& goList = m_gameObjectListArray.at(prio);
				auto it = std::find(goList.begin(), goList.end(), go);
				if (it != goList.end())
				{
					delete(*it);
				}
				goList.erase(it);
			}
		}
		deleteList.clear();
	}
}

void GameObjectManager::Render()
{
}
