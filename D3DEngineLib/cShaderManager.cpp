#include "Common.h"
#include "cShaderManager.h"
#include "include\D3DRenderer.h"

namespace ENGINE
{

	cShaderManager::cShaderManager()
	{
	}


	cShaderManager::~cShaderManager()
	{
	}

	void cShaderManager::Destroy()
	{
		for each(auto it in m_mapShader)
		{
			it.second->Release();
		}
	}

	LPD3DXEFFECT cShaderManager::GetShader(char* szFolder, const char* szFileName)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFileName);

		if (m_mapShader.find(sFullPath) == m_mapShader.end())
		{ 
			if (AddShader(szFolder, szFileName) == false)
			{
				return NULL;
			}
		}

		return m_mapShader[sFullPath];
	}

	bool cShaderManager::AddShader(char* szFolder, const char* szFileName)
	{
		std::string sFullPath(szFolder);
		sFullPath += std::string(szFileName);

		std::wstring temp;

		UTIL::StringToWString(sFullPath, temp);

		LPD3DXEFFECT ret = NULL;

		LPD3DXBUFFER pError = NULL;
		DWORD dwShaderFlags = 0;

#if _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG;
#endif


		D3DXCreateEffectFromFile(g_pD3DDevice, temp.c_str(),
			NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

		// 쉐이더 로딩에 실패한 경우 output창에 쉐이더
		// 컴파일 에러를 출력한다.
		if (!ret && pError)
		{

			std::string error = (char*)pError->GetBufferPointer();
			std::wstring wError = L"";
			wError.assign(error.begin(), error.end());
			::MessageBox(0, wError.c_str(), 0, 0);
			SAFE_RELEASE(pError);

			return false;
		}

		//맵에 추가
		m_mapShader[sFullPath] = ret;

		return true;
	}

	void cShaderManager::SetLight(D3DXVECTOR3 LightPos, D3DXVECTOR3 LightDir, D3DXVECTOR4 LightColor, D3DXMATRIXA16* matDirLightViewProj, D3DXMATRIXA16* matDirLightViewProj1, D3DXMATRIXA16* matDirLightViewProj2)
	{
		std::map<std::string, LPD3DXEFFECT>::iterator iter;


		iter = m_mapShader.find("Shader/tooneffect.txt");
		if (iter != m_mapShader.end())
		{
			iter->second->SetMatrix("Mat_LightViewProj", matDirLightViewProj);
			iter->second->SetMatrix("Mat_LightViewProj1", matDirLightViewProj1);
			iter->second->SetMatrix("Mat_LightViewProj2", matDirLightViewProj2);
			D3DXVECTOR4 lightPos = D3DXVECTOR4(LightPos.x, LightPos.y, LightPos.z, 1.0f);
			iter->second->SetVector("gWorldLightPosition", &lightPos); //안쓰는듯
			iter->second->SetVector("gLightColor", &LightColor);
			D3DXVECTOR4 lightDir = D3DXVECTOR4(LightDir.x, LightDir.y, LightDir.z, 0.0f);
			iter->second->SetVector("LgtDir", &lightDir);
		}

		iter = m_mapShader.find("Terrain/Shader/Splatting.fx");
		if (iter != m_mapShader.end())
		{
			iter->second->SetMatrix("Mat_LightViewProj", matDirLightViewProj);
			iter->second->SetMatrix("Mat_LightViewProj1", matDirLightViewProj1);
			iter->second->SetMatrix("Mat_LightViewProj2", matDirLightViewProj2);
			D3DXVECTOR4 lightDir = D3DXVECTOR4(LightDir.x, LightDir.y, LightDir.z, 0.0f);
			iter->second->SetVector("m_LgtDir", &lightDir);
			iter->second->SetVector("m_LgtDif", &LightColor);
		}
	}

	void cShaderManager::SetCreateShadowMatrix(D3DXMATRIXA16* matDirLightViewProj)
	{
		std::map<std::string, LPD3DXEFFECT>::iterator iter;


		iter = m_mapShader.find("Shader/CreateShadow.fx");
		if (iter != m_mapShader.end())
		{
			iter->second->SetMatrix("Mat_LightViewProj", matDirLightViewProj);
		}
	}
	
	void cShaderManager::SetCamera(D3DXVECTOR3 CameraPos, float frustum_far1, float frustum_far2)
	{
		std::map<std::string, LPD3DXEFFECT>::iterator iter;

		cTerrainData *td = g_pTerrainManager->GetCurrentTerrain();
		bool bIsFog = false;
		if (td != NULL)
		{
			bIsFog = td->GetFog();
		}

		iter = m_mapShader.find("Shader/tooneffect.txt");
		if (iter != m_mapShader.end())
		{
			iter->second->SetFloat("Frustum_far1", frustum_far1);
			iter->second->SetFloat("Frustum_far2", frustum_far2);
			iter->second->SetVector("gWorldCameraPosition", &D3DXVECTOR4(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f));
			D3DXVECTOR4 fFog = D3DXVECTOR4(0.66f, 1800.0f, bIsFog, 0.0f);
			iter->second->SetVector("fFog", &fFog);

			D3DXMATRIXA16 mView = g_pCameraManager->GetCurrentCamera()->GetMatView();
			//g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
			iter->second->SetMatrix("Mat_View", &mView);

		}

		iter = m_mapShader.find("Terrain/Shader/Splatting.fx");
		if (iter != m_mapShader.end())
		{
			iter->second->SetFloat("Frustum_far1", frustum_far1);
			iter->second->SetFloat("Frustum_far2", frustum_far2);

			D3DXVECTOR4 fFog = D3DXVECTOR4(0.66f, 1800.0f, bIsFog, 0.0f);
			iter->second->SetVector("fFog", &fFog);

			D3DXMATRIXA16 mView;
			g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
			iter->second->SetMatrix("Mat_View", &mView);
		}
	}

	void cShaderManager::SetShadowMap(LPDIRECT3DTEXTURE9 shadowMap, LPDIRECT3DTEXTURE9 shadowMap1, LPDIRECT3DTEXTURE9 shadowMap2)
	{
		std::map<std::string, LPD3DXEFFECT>::iterator iter;
		iter = m_mapShader.find("Shader/tooneffect.txt");
		if (iter != m_mapShader.end())
		{
			iter->second->SetTexture("ShadowMap_Tex", shadowMap);
			iter->second->SetTexture("ShadowMap_Tex1", shadowMap1);
			iter->second->SetTexture("ShadowMap_Tex2", shadowMap2);
		}

		iter = m_mapShader.find("Terrain/Shader/Splatting.fx");
		if (iter != m_mapShader.end())
		{
			iter->second->SetTexture("ShadowMap_Tex", shadowMap);
			iter->second->SetTexture("ShadowMap_Tex1", shadowMap1);
			iter->second->SetTexture("ShadowMap_Tex2", shadowMap2);
		}

	}
}