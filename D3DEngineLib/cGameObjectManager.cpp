#include "Common.h"
#include "cGameObjectManager.h"
#include "include\D3DRenderer.h"

namespace ENGINE
{

	cGameObjectManager::cGameObjectManager()
	{
	}


	cGameObjectManager::~cGameObjectManager()
	{
	}


	GameObject* cGameObjectManager::FindGameObject(unsigned int uid)
	{
		stdext::hash_map<unsigned int, GameObject*>::iterator iter;

		iter = m_mapGameObjects.find(uid);

		if (iter != m_mapGameObjects.end())
		{
			return iter->second;
		}
		else
		{
			return NULL;
		}
	}
	void cGameObjectManager::FindTagGameObjectsID(GAMEOBJECT_TAG tag, IN unsigned int* arr, OUT int& size)
	{
		//리턴하는 오브젝트 id갯수
		unsigned int sizeCnt = 0;

		stdext::hash_map<unsigned int, GameObject*>::iterator iter;

		for (iter = m_mapGameObjects.begin(); iter != m_mapGameObjects.end(); iter++)
		{
			//최대 100개까지만 리턴
			if (size == 100) break;

			if (iter->second->GetGameObjectTag() == tag){
				arr[sizeCnt++] = iter->second->GetID();
			}
		}

		//리턴하는 값 카운트를 알려준다
		size = sizeCnt;
	}
	void cGameObjectManager::AddGameObject(GameObject* gameObject)
	{
		m_mapGameObjects.insert(stdext::hash_map<unsigned int, GameObject*>::value_type(gameObject->GetID(), gameObject));
	}

	void cGameObjectManager::EraseGameObject(GameObject* gameObject)
	{
		m_mapGameObjects.erase(gameObject->GetID());
	}

	void cGameObjectManager::EraseGameObject(unsigned int uid)
	{
		m_mapGameObjects.erase(uid);
	}
}