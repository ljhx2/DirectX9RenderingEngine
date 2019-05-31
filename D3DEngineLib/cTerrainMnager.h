#pragma once

#define g_pTerrainManager cTerrainMnager::GetInstance()

namespace ENGINE
{
	class cTerrainData;

	class cTerrainMnager
	{
	private:
		cTerrainMnager();
		~cTerrainMnager();
	public :
		static cTerrainMnager* GetInstance()
		{
			static cTerrainMnager instance; return &instance;
		}
	public:
		cTerrainData* GetCurrentTerrain() { return m_pCurrentTerrain; }
		void SetCurrentTerrain(cTerrainData* data) { m_pCurrentTerrain = data; }

		bool AddTerrain(char* szHeightFileName, const char* szTextureFileName, float fcell = 10.0f, bool bIsFog = false);
		cTerrainData * SetTerrain(char* szHeightFileName);
		cTerrainData * GetTerrain(char* szHeightFileName, const char* szTextureFileName);
		void Destroy();
	private:
		std::map<std::string, cTerrainData*> m_mapTerrain;
		cTerrainData* m_pCurrentTerrain;
	};
}