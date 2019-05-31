#include "Common.h"
#include "cFbxManager.h"
#include "cFbxData.h"


/*
// Char ¡æ TCHAR
inline TCHAR* CharToTCHAR(const char* pstrSrc)
{
	size_t nLen = strlen(pstrSrc) + 1;
	TCHAR * pwstr = (LPWSTR)malloc(sizeof(TCHAR)*nLen);
	size_t con = 0;
	memset(pwstr, 0, sizeof(pwstr));

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pstrSrc, nLen, pwstr, nLen);
	return pwstr;
}

// TCHAR ¡æ Char
inline char * TCHARToChar(const TCHAR* pstrSrc)
{
	size_t nLen = lstrlen(pstrSrc) + 1;
	char * pstr = new char[sizeof(char)* nLen];
	WideCharToMultiByte(CP_ACP, 0, pstrSrc, nLen, pstr, nLen, NULL, NULL);

	return pstr;
}
*/

namespace ENGINE
{
	cFbxManager::cFbxManager()
	{
		m_pFbxSdkManager = FbxManager::Create();
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(m_pFbxSdkManager, IOSROOT);
		m_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	cFbxManager::~cFbxManager()
	{
	}

	void cFbxManager::Destroy()
	{
		for each(auto it in m_mapFbx)
		{
			it.second->Destroy();
			SAFE_DELETE(it.second);
		}

		if (m_pFbxSdkManager)
			m_pFbxSdkManager->Destroy();
	}

	cFbxData* cFbxManager::GetFbxData(char* szFolder, const char* szFilename)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFilename);

		if (m_mapFbx.find(sFullPath) == m_mapFbx.end())
		{
			cFbxData* pFbxData = new cFbxData;
			pFbxData->Load(m_pFbxSdkManager, szFolder, szFilename);
			m_mapFbx[sFullPath] = pFbxData;
		}

		return m_mapFbx[sFullPath];
	}

	bool cFbxManager::AddFbxData(char* szFolder, const char* szFilename)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFilename);

		if (m_mapFbx.find(sFullPath) == m_mapFbx.end())
		{
			cFbxData* pFbxData = new cFbxData;
			pFbxData->Load(m_pFbxSdkManager, szFolder, szFilename);
			m_mapFbx[sFullPath] = pFbxData;

			return true;
		}

		return false;
	}
}
