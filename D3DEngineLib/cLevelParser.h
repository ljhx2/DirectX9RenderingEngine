#pragma once

#include "include\D3DRenderer.h"

namespace ENGINE
{
	class cLevelParser
	{
	public:
		void	Load(IN const char* szFilePath, OUT multiMapGameObjects& outputMultiMap);

		char*	GetToken();
		int		GetInteger();
		float	GetFloat();
		bool	IsEqual(char* str1, char* str2);
		void	ProcessGameObject(OUT multiMapGameObjects& outputMultiMap);

		cLevelParser();
		~cLevelParser();

	private:
		FILE*	m_fp;
		char	m_szToken[1024];
	};

}