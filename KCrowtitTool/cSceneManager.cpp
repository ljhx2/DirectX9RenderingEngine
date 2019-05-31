#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"

cSceneManager::cSceneManager()
	: _currentScene(NULL)
{
}


cSceneManager::~cSceneManager()
{
	if (_currentScene)
		_currentScene->OnLeave();

	SceneMap::iterator iter = _sceneMap.begin();
	SceneMap::iterator end = _sceneMap.end();

	while (iter != end)
	{
		SAFE_DELETE(iter->second);
		iter++;
	}
	_sceneMap.clear();
}

const std::wstring& cSceneManager::GetCurrentSceneID() const
{
	assert(_currentScene);
	return _currentScene->GetID();
}

/* 
	@param id ��ü �ϰ��� �ϴ� �� ���̵�
	@return ����ȯ ���� ����
*/
bool cSceneManager::ChangeScene(const std::wstring& id)
{
	SceneMap::iterator iter;
	iter = _sceneMap.find(id);
	if (iter != _sceneMap.end())
	{
		if (_currentScene)
			_currentScene->OnLeave(); //�ٲ�� �� ���� ������ �޸� ����

		_currentScene = iter->second;
		_currentScene->OnEnter(); //�ٲ� ��鿡�� OnEnter

		return true;
	}

	return false;
}

void cSceneManager::Update(DWORD elapsedTime)
{
	assert(_currentScene);

	_currentScene->Update(elapsedTime);
}

void cSceneManager::Render()
{
	assert(_currentScene);

	_currentScene->Render();
}

/*
	@param id �߰��� ���� id(key)
	@param scene �߰��� ��
	@return ���߰� ����
*/
bool cSceneManager::AddScene(std::wstring& id, cScene* scene)
{
	if (scene == NULL) return false;

	SceneMap::iterator iter;
	iter = _sceneMap.find(id);

	if (iter != _sceneMap.end()) return false;	//id�� �̹� ����

	_sceneMap.insert(std::make_pair(id, scene));
	return true;
}

/*
	@param id ������ ���� id
	@return id�� �´� scene����, ���� id�̸� NULL���� 
*/
cScene* cSceneManager::GetScene(std::wstring& id)
{
	SceneMap::iterator iter;
	iter = _sceneMap.find(id);
	
	if (iter == _sceneMap.end()) return NULL;

	return iter->second;

}

/*
	@param id ���� ���� id
*/
void cSceneManager::DeleteScene(std::wstring& id)
{
	SceneMap::iterator iter;
	iter = _sceneMap.find(id);

	if (iter == _sceneMap.end()) return;

	_sceneMap.erase(iter);
}