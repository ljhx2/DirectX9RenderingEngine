#include "Common.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"

namespace ENGINE
{
	cSkinnedMeshManager::cSkinnedMeshManager(void)
	{
	}


	cSkinnedMeshManager::~cSkinnedMeshManager(void)
	{
	}

	void cSkinnedMeshManager::Destroy()
	{
		for each(auto it in m_mapSkinnedMesh)
		{
			it.second->Destroy();
			SAFE_DELETE(it.second);
		}
	}

	cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh(char* szFolder, const char* szFilename)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFilename);

		if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
		{
			cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
			pSkinnedMesh->Load(szFolder, szFilename);
			m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
		}
		return m_mapSkinnedMesh[sFullPath];
	}

	bool cSkinnedMeshManager::AddSkinnedMesh(char* szFolder, const char* szFilename)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFilename);

		if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
		{
			cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
			pSkinnedMesh->Load(szFolder, szFilename);
			m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;

			return true;
		}
		return false;
	}
}