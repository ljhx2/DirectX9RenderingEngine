#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()


namespace ENGINE
{
	class cSkinnedMesh;
	class cSkinnedMeshManager
	{
	private:
		cSkinnedMeshManager();
		~cSkinnedMeshManager();
	public:
		static cSkinnedMeshManager* GetInstance() { static cSkinnedMeshManager instance; return &instance; }
	private:
		std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;

	public:
	
		cSkinnedMesh* GetSkinnedMesh(char* szFolder, const char* szFilename);
		bool AddSkinnedMesh(char* szFolder, const char* szFilename);
		void Destroy();
	};

}