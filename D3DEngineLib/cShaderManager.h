#pragma once

#define g_pShaderManager cShaderManager::GetInstance()


namespace ENGINE
{
	class cShaderManager
	{
	private:
		cShaderManager();
		~cShaderManager();

	public:
		static cShaderManager* GetInstance() { static cShaderManager instance; return &instance; }

	private:
		std::map<std::string, LPD3DXEFFECT> m_mapShader;

	public:
		LPD3DXEFFECT GetShader(char* szFolder, const char* szFileName);
		bool AddShader(char* szFolder, const char* szFileName);
		void Destroy();

		//void SetLight(D3DXVECTOR3 LightPos, D3DXVECTOR3 LightDir, D3DXVECTOR4 LightColor, D3DXMATRIXA16* matDirLightViewProj);

		void SetLight(D3DXVECTOR3 LightPos, D3DXVECTOR3 LightDir, D3DXVECTOR4 LightColor,
			D3DXMATRIXA16* matDirLightViewProj, D3DXMATRIXA16* matDirLightViewProj1, D3DXMATRIXA16* matDirLightViewProj2);
		void SetCamera(D3DXVECTOR3 CameraPos, float frustum_far1, float frustum_far2);
		void SetShadowMap(LPDIRECT3DTEXTURE9 shadowMap, LPDIRECT3DTEXTURE9 shadowMap1, LPDIRECT3DTEXTURE9 shadowMap2);
		void SetCreateShadowMatrix(D3DXMATRIXA16* matDirLightViewProj);
		
	};

}