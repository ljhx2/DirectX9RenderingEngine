#include "Common.h"
#include "cStaticMeshManager.h"
#include "cStaticMeshComponent.h"

namespace ENGINE
{
	cStaticMeshManager::cStaticMeshManager()
	{
	}


	cStaticMeshManager::~cStaticMeshManager()
	{
	}

	void cStaticMeshManager::Destroy()
	{
		for each(auto it in m_mapStaticMesh)
		{
			it.second->Destroy();
			SAFE_DELETE(it.second);
		}
	}

	cStaticMeshComponent* cStaticMeshManager::GetStaticMesh(char* szFolder, const char* szFilename)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFilename);

		if (m_mapStaticMesh.find(sFullPath) == m_mapStaticMesh.end())
		{
			cStaticMeshComponent* pStaticMesh = new cStaticMeshComponent;
			pStaticMesh->Load(szFolder, szFilename);
			m_mapStaticMesh[sFullPath] = pStaticMesh;
		}
		return m_mapStaticMesh[sFullPath];
	}

	bool cStaticMeshManager::AddStaticMesh(char* szFolder, const char* szFilename)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFilename);

		if (m_mapStaticMesh.find(sFullPath) == m_mapStaticMesh.end())
		{
			cStaticMeshComponent* pStaticMesh = new cStaticMeshComponent;
			pStaticMesh->Load(szFolder, szFilename);
			m_mapStaticMesh[sFullPath] = pStaticMesh;
			return true;
		}
		return false;
	}
}