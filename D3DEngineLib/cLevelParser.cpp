#include "Common.h"
#include "cLevelParser.h"
//#include "include\D3DRenderer.h"
#include "cStaticMeshComponent.h"
#include "cSkinnedMeshComponent.h"

namespace ENGINE
{
	cLevelParser::cLevelParser()
	{
	}


	cLevelParser::~cLevelParser()
	{
	}

	void cLevelParser::Load(IN const char* szFilePath, OUT multiMapGameObjects& outputMultiMap)
	{
		outputMultiMap.clear();

		fopen_s(&m_fp, szFilePath, "r");
		char* szToken = NULL;
		while (szToken = GetToken())
		{
			if (IsEqual(szToken, "*GameObject"))
			{
				ProcessGameObject(outputMultiMap);
			}
		}
	}

	char* cLevelParser::GetToken()
	{
		int nCnt = 0;
		bool isQuote = false;

		while (!feof(m_fp))
		{
			char c = fgetc(m_fp);

			if (c < 33 && !isQuote)	//공백
			{
				if (nCnt > 0) break;
				else continue;
			}

			if (c == '\"')
			{
				if (isQuote) break;
				else
				{
					isQuote = true;
					continue;
				}
			}

			m_szToken[nCnt++] = c;
		}
		if (nCnt == 0)
			return NULL;

		m_szToken[nCnt] = '\0';
		return m_szToken;
	}

	int	 cLevelParser::GetInteger()
	{
		return atoi(GetToken());
	}

	float cLevelParser::GetFloat()
	{
		return (float)atof(GetToken());
	}

	bool cLevelParser::IsEqual(char* str1, char* str2)
	{
		return strcmp(str1, str2) == 0;
	}

	void cLevelParser::ProcessGameObject(OUT multiMapGameObjects& outputMultiMap)
	{
		int resourceType = 0;
		std::string resourcePath;
		std::string name;
		D3DXVECTOR3 scale(1, 1, 1);
		D3DXVECTOR3 rotation(0, 0, 0);
		D3DXVECTOR3 translation(0, 0, 0);

		int nLevel = 0;
		do
		{
			char* szToken = GetToken();
			if (IsEqual(szToken, "{"))
				++nLevel;
			else if (IsEqual(szToken, "}"))
				--nLevel;
			else if (IsEqual(szToken, "*Name"))
			{
				name = GetToken();
			}
			else if (IsEqual(szToken, "*ResourcePath"))
			{
				resourcePath = GetToken();
			}
			else if (IsEqual(szToken, "*ResourceType"))
			{
				resourceType = GetInteger();
			}
			else if (IsEqual(szToken, "*ScaleValue"))
			{
				scale.x = GetFloat();
				scale.y = GetFloat();
				scale.z = GetFloat();
			}
			else if (IsEqual(szToken, "*RotationValue"))
			{
				rotation.x = GetFloat();
				rotation.y = GetFloat();
				rotation.z = GetFloat();
			}
			else if (IsEqual(szToken, "*TranslationValue"))
			{
				translation.x = GetFloat();
				translation.y = GetFloat();
				translation.z = GetFloat();
			}

		} while (nLevel > 0);



		//GameObject생성
		if ((ECOMPONENT_TYPE)resourceType == eSTATIC_MESH_TYPE)
		{
			GameObject* obj = InstantiateStaticMesh(resourcePath);
			obj->SetName(name.c_str());
			obj->SetScale(scale.x, scale.y, scale.z);
			obj->SetRotation(rotation.x, rotation.y, rotation.z);
			obj->SetTranslate(translation.x, translation.y, translation.z);
			outputMultiMap.insert(std::make_pair(name, obj));
		}
		else if ((ECOMPONENT_TYPE)resourceType == eSKINNED_MESH_TYPE)
		{
			GameObject* obj = InstantiateSkinnedMesh(resourcePath);
			obj->SetName(name.c_str());
			obj->SetScale(scale.x, scale.y, scale.z);
			obj->SetRotation(rotation.x, rotation.y, rotation.z);
			obj->SetTranslate(translation.x, translation.y, translation.z);
			outputMultiMap.insert(std::make_pair(name, obj));
		}
	}
}