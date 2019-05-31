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
		//�����ϴ� ������Ʈ id����
		unsigned int sizeCnt = 0;

		stdext::hash_map<unsigned int, GameObject*>::iterator iter;

		for (iter = m_mapGameObjects.begin(); iter != m_mapGameObjects.end(); iter++)
		{
			//�ִ� 100�������� ����
			if (size == 100) break;

			if (iter->second->GetGameObjectTag() == tag){
				arr[sizeCnt++] = iter->second->GetID();
			}
		}

		//�����ϴ� �� ī��Ʈ�� �˷��ش�
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