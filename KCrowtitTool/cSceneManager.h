#pragma once

#define g_pSceneManager cSceneManager::GetInstance()

class cScene;

class cSceneManager
{
	SINGLETONE(cSceneManager);
public:
	const std::wstring& GetCurrentSceneID() const;
	bool ChangeScene(const std::wstring& id);
	cScene* GetCurrentScene() { return _currentScene; }
	void Update(DWORD elapsedTime);
	void Render();
	bool AddScene(std::wstring& id, cScene* scene);
	cScene* GetScene(std::wstring& id);
	void DeleteScene(std::wstring& id);

protected:
	typedef std::map<const std::wstring, cScene*> SceneMap;

	SceneMap _sceneMap;

	cScene* _currentScene;

};

