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
	@param id 교체 하고자 하는 씬 아이디
	@return 씬전환 성공 여부
*/
bool cSceneManager::ChangeScene(const std::wstring& id)
{
	SceneMap::iterator iter;
	iter = _sceneMap.find(id);
	if (iter != _sceneMap.end())
	{
		if (_currentScene)
			_currentScene->OnLeave(); //바뀌기 전 현재 씬에서 메모리 정리

		_currentScene = iter->second;
		_currentScene->OnEnter(); //바뀐 장면에서 OnEnter

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
	@param id 추가할 씬의 id(key)
	@param scene 추가할 씬
	@return 씬추가 여부
*/
bool cSceneManager::AddScene(std::wstring& id, cScene* scene)
{
	if (scene == NULL) return false;

	SceneMap::iterator iter;
	iter = _sceneMap.find(id);

	if (iter != _sceneMap.end()) return false;	//id가 이미 존재

	_sceneMap.insert(std::make_pair(id, scene));
	return true;
}

/*
	@param id 가져올 씬의 id
	@return id에 맞는 scene리턴, 없는 id이면 NULL리턴 
*/
cScene* cSceneManager::GetScene(std::wstring& id)
{
	SceneMap::iterator iter;
	iter = _sceneMap.find(id);
	
	if (iter == _sceneMap.end()) return NULL;

	return iter->second;

}

/*
	@param id 지울 씬의 id
*/
void cSceneManager::DeleteScene(std::wstring& id)
{
	SceneMap::iterator iter;
	iter = _sceneMap.find(id);

	if (iter == _sceneMap.end()) return;

	_sceneMap.erase(iter);
}