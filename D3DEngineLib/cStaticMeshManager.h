#pragma once

#define g_pStaticMeshManager cStaticMeshManager::GetInstance()

namespace ENGINE
{
	class cStaticMeshComponent;

	class cStaticMeshManager
	{
	private:
		cStaticMeshManager();
		~cStaticMeshManager();
	public:
		static cStaticMeshManager* GetInstance() { static cStaticMeshManager instance; return &instance; }
	private:
		std::map<std::string, cStaticMeshComponent*> m_mapStaticMesh;

	public:
		cStaticMeshComponent* GetStaticMesh(char* szFolder, const char* szFilename);
		bool AddStaticMesh(char* szFolder, const char* szFilename);
		void Destroy();
	};

}