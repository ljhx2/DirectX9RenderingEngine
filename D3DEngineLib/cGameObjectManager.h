#pragma once


#define g_pGameObjectManager cGameObjectManager::GetInstance()

namespace ENGINE
{
	class GameObject;
	enum GAMEOBJECT_TAG;

	class cGameObjectManager
	{
	private:
		cGameObjectManager();
		~cGameObjectManager();

	public:
		static cGameObjectManager* GetInstance() { static cGameObjectManager instance; return &instance; }

	private:
		stdext::hash_map<unsigned int, GameObject*> m_mapGameObjects;

	public:
		GameObject* FindGameObject(unsigned int uid);
		void FindTagGameObjectsID(GAMEOBJECT_TAG tag, IN unsigned int* arr, OUT int& size);
		void AddGameObject(GameObject* gameObject);
		void EraseGameObject(GameObject* gameObject);
		void EraseGameObject(unsigned int uid);
	};

}