#pragma once
#include "fbxsdk.h"

#define g_pFbxManager cFbxManager::GetInstance()

namespace ENGINE
{
	class cFbxData;

	class cFbxManager
	{
	private:
		cFbxManager();
		~cFbxManager();
	public:
		static cFbxManager* GetInstance() { static cFbxManager instance; return &instance; }


	private:

		FbxManager*				m_pFbxSdkManager;

		std::map<std::string, cFbxData*> m_mapFbx;

	public:

		cFbxData* GetFbxData(char* szFolder, const char* szFilename);
		bool AddFbxData(char* szFolder, const char* szFilename);

		void Destroy();
	};
}
