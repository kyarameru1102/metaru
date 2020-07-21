#pragma once
/// <summary>
/// GameObject�̃}�l�[�W���[�B
/// </summary>

#include <vector>		//�ϒ��z��B
#include"GameObject.h"
#include "util/Util.h"
#include "Sprite.h"
#include "RenderTarget.h"
#include "Bloom.h"

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
	/// <summary>
	/// ���O�`��B
	/// </summary>
	void PreRender();
	/// <summar
	/// 3D�`��B
	/// </summary>
	void Render3D();
	/// <summary>
	/// �x���`��B
	/// </summary>
	void PostRender();
	void Render2D();
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̐؂�ւ��B
	/// </summary>
	/// <param name="d3dDeviceContext">D3D�f�o�C�X�R���e�L�X�g</param>
	/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
	/// <param name="viewport">�r���[�|�[�g</param>
	/*void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);*/
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
	/// NewGO����ۂɂ������O�Ō�����������B
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
	template<class T>
	void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = Util::MakeHash(objectName);
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					T* t = dynamic_cast<T*>(go);
					if (func(t) == false)
						return;
				}
			}
		}
	}
private:
	std::vector<IGameObject*>			 m_gameObjectList;
	typedef std::list<IGameObject*>		GameObjectList;
	std::vector<GameObjectList>			m_gameObjectListArray;		//�Q�[���I�u�W�F�N�g�̗D��x�t�����X�g
	std::vector<GameObjectList>			m_deleteObjectListArray;	//�폜�p�̃��X�g�B
	GOPrio								m_priorityMax = 255;		//�D��x�̍ő吔�B
	CVector3							m_lightPos = { 0.0f,0.0f,0.0f, };

	RenderTarget						m_mainRenderTarget;				//���C�������_�����O�^�[�Q�b�g�B

	Bloom								m_bloom;						//�u���[���N���X�B
	Sprite								m_copyMainRtToFrameBufferSprite;			//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�B
	D3D11_VIEWPORT						m_frameBufferViewports;			//�t���[���o�b�t�@�̃r���[�|�[�g�B
	ID3D11RenderTargetView*				m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�B
	ID3D11DepthStencilView*				m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[�B
};
static inline GameObjectManager& gameObjectManager()
{
	return GameObjectManager::Instance();
}
/// <summary>
/// �Q�[���I�u�W�F�N�g�𐶐�����֐��B
/// </summary>
template<class T>
static inline T* NewGO(int priority, const char* objectName = nullptr)
{
	return gameObjectManager().NewGameObject<T>((GOPrio)priority, objectName);
}
/// <summary>
/// �Q�[���I�u�W�F�N�g���폜����֐��B
/// </summary>
/// <param name="go">
/// �Q�[���I�u�W�F�N�g�B
/// </param>
static inline void DeleteGO(IGameObject* go)
{
	gameObjectManager().DeleteGameObject(go);
}
/// <summary>
///�Q�[���I�u�W�F�N�g�����p�̊֐��B
/// </summary>
/// <param name="objectName">NewGO�̂Ƃ��ɃI�u�W�F�N�g�ɂ������O�B</param>
/// <returns>�C���X�^���X�B</returns>
template<class T>
static inline T* FindGO(const char* objectName)
{
	return gameObjectManager().FindGameObject<T>(objectName);
}
/// <summary>
/// �Q�[���I�u�W�F�N�g�����p�̊֐��B
/// </summary>
/// <param name="objectName">NewGO�̂Ƃ��ɃI�u�W�F�N�g�ɂ������O�B</param>
/// <param name="func">�����������ɍs���R�[���o�b�N�֐��B</param>

template<class T>
static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
{
	return gameObjectManager().FindGameObjects<T>(objectName, func);
}

/// <summary>
/// �����ɓn�������O�����Ă���Q�[���I�u�W�F�����Ƃ��ׂĂ��폜����֐��B
/// </summary>
/// <param name="objectName">NewGO�̂Ƃ��ɃI�u�W�F�N�g�ɂ������O�B</param>
static inline void DeleteGOs(const char* objectName)
{
	QueryGOs<IGameObject>(objectName, [](auto go) {
		DeleteGO(go);
		return true;
	});
}