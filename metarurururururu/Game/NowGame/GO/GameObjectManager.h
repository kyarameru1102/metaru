#pragma once
/// <summary>
/// GameObject�̃}�l�[�W���[�B
/// </summary>

#include <vector>		//�ϒ��z��B
#include"GameObject.h"
#include "util/Util.h"


class GameObjectManager
{
private:
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̖��O�L�[���쐬����֐��B
	/// </summary>
	static unsigned int MakeGameObjectNameKey(const char* objectName)
	{
		static const unsigned int defaultNameKey = Util::MakeHash("Undefined");
		unsigned int hash;
		if (objectName == nullptr) {
			hash = defaultNameKey;
		}
		else {
			hash = Util::MakeHash(objectName);
		}
		return hash;
	}
public:
	/// <summary>
	/// �������֐��B
	/// </summary>
	/// <param name="gameObjectPrioMax">
	/// �ő�D�揇�ʁi255�܂Łj
	/// </param>
	void Init(int gameObjectPrioMax);

	static GameObjectManager& Instance()
	{
		static GameObjectManager instance;
		return instance;
	}
	void Update();
	void Render();
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��new
	/// </summary>
	/// prio			�D�揇�ʁB
	/// objectName		�I�u�W�F�N�g�̖��O�B
	template<class T>
	T* NewGameObject(GOPrio prio, const char* objectName)
	{
		if (prio <= m_priorityMax)
		{
			T* NewObject = new T();
			m_gameObjectListArray.at(prio).push_back(NewObject);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			NewObject->SetPriority(prio);
			NewObject->SetNameKey(hash);
			return NewObject;
		}
		else {
			assert(1);
		}
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍폜�B
	/// </summary>
	/// <param name="gameObject">
	/// �Q�[���I�u�W�F�N�g�B
	/// </param>
	void DeleteGameObject(IGameObject*& gameObject)
	{
		if (gameObject != nullptr)
		{
			gameObject->OnDestroy();
			gameObject->SetIsDead();
			m_deleteObjectListArray.at(gameObject->GetPriority()).push_back(gameObject);
			gameObject = nullptr;
		}
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̌����B
	/// </summary>
	template<class T>
	T* FindGameObject(const char* objectName)
	{
		unsigned int nameKey = Util::MakeHash(objectName);
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					T* t = dynamic_cast<T*>(go);
					if (t != nullptr) {
						return t;
					}
				}
			}
		}
		return nullptr;
	}
private:
	std::vector<IGameObject*>			 m_gameObjectList;
	typedef std::list<IGameObject*>		GameObjectList;
	std::vector<GameObjectList>			m_gameObjectListArray;		//�Q�[���I�u�W�F�N�g�̗D��x�t�����X�g
	std::vector<GameObjectList>			m_deleteObjectListArray;	//�폜�p�̃��X�g�B
	GOPrio								m_priorityMax = 255;		//�D��x�̍ő吔�B
	CVector3							m_lightPos = { 100.0f,300.0f,70.0f, };
};
static inline GameObjectManager& gameObjectManager()
{
	return GameObjectManager::Instance();
}
/// <summary>
/// �Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
/// </summary>
template<class T>
static inline T* NewGO(int priority, const char* objectName = nullptr)
{
	return gameObjectManager().NewGameObject<T>((GOPrio)priority, objectName);
}
/// <summary>
/// �Q�[���I�u�W�F�N�g�폜�̃w���p�[�֐��B
/// </summary>
/// <param name="go">
/// �Q�[���I�u�W�F�N�g�B
/// </param>
static inline void DeleteGO(IGameObject* go)
{
	gameObjectManager().DeleteGameObject(go);
}
/// <summary>
/// g�Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
/// </summary>
template<class T>
static inline T* FindGO(const char* objectName)
{
	return gameObjectManager().FindGameObject<T>(objectName);
}
