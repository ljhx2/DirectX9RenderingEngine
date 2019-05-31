#include "Common.h"
#include "cTerrainData.h"
#include "cTerrainMnager.h"

namespace ENGINE
{
	cTerrainMnager::cTerrainMnager() : m_pCurrentTerrain(NULL)
	{

	}
	cTerrainMnager::~cTerrainMnager()
	{
		SAFE_DELETE(m_pCurrentTerrain);
		for each(auto iter in m_mapTerrain)
		{
			SAFE_DELETE(iter.second);
		}
	}
	void cTerrainMnager::Destroy()
	{
		for each(auto it in m_mapTerrain)
		{
			//it.second->r();
			SAFE_DELETE(it.second);
		}
	}

	bool cTerrainMnager::AddTerrain(char* szHeightFileName, const char* szTextureFileName, float fcell, bool bIsFog)
	{
		if (m_mapTerrain.find(szHeightFileName) == m_mapTerrain.end())
		{
			cTerrainData* pTerrainDate = new cTerrainData;
			pTerrainDate->SetCellSpacing(fcell);
			pTerrainDate->SetFog(bIsFog);
			pTerrainDate->LoadHeightMap(szHeightFileName);
			pTerrainDate->LoadTexture(szTextureFileName);
			m_mapTerrain[szHeightFileName] = pTerrainDate;
			m_pCurrentTerrain = m_mapTerrain[szHeightFileName];
			return true;
		}
		return false;
	}

	cTerrainData * cTerrainMnager::SetTerrain(char* szHeightFileName)
	{
		cTerrainData* pTerrainDate = new cTerrainData;
		pTerrainDate->SettingSize(255,255,1);
		m_mapTerrain.insert(std::make_pair(szHeightFileName, pTerrainDate));
		m_pCurrentTerrain = m_mapTerrain[szHeightFileName];
		return m_mapTerrain[szHeightFileName];
	}

	//높이 맵과 textrue file 은 같은 이름이다. 확장자만 다름 raw , png
	cTerrainData * cTerrainMnager::GetTerrain(char* szHeightFileName, const char* szTextureFileName)
	{ 
		if (m_mapTerrain.find(szHeightFileName) == m_mapTerrain.end())
		{
			SetTerrain(szHeightFileName);
		}
		return m_mapTerrain[szHeightFileName];
	}
}