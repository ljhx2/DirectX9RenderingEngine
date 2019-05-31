#include "Common.h"
#include "include\D3DRenderer.h"
#include "cWorldMapManager.h"

namespace ENGINE
{

	cWorldMapManger::cWorldMapManger()
	{
		arrWorldMap = NULL;
	}

	cWorldMapManger::~cWorldMapManger()
	{
		if (arrWorldMap != NULL){
			delete[] arrWorldMap;
		}
	}

	void cWorldMapManger::SettingWorldMap()
	{
		 cTerrainData * terrain = g_pTerrainManager->GetCurrentTerrain();
		 if (terrain != NULL)
		{
			m_nStartCol = terrain->GetStartCol();
			m_nStartRow = terrain->GetStartRow();
			m_nCell = terrain->GetRow() + 1;
			m_fCellSpacing = terrain->GetCellSpacing();

			arrWorldMap = new int*[m_nCell];
			for (int i = 0; i < m_nCell; ++i)
			{
				arrWorldMap[i] = new int[m_nCell];
			}

			//초기화 
			for (int col = 0; col < m_nCell; ++col)
			{
				for (int row = 0; row < m_nCell; ++row)
				{
					arrWorldMap[col][row] = TILE_NONE_SOMETHING;
				}
			}
			
			//이거 세련되게 바꾸자~
			FILE *pfile;
			fopen_s(&pfile, "Terrain/WorldMap_2D_Obstacle.txt", "r");
			//파일 없으면 에러 
			if (!pfile) {
				::MessageBox(0,L"OBSTACLE_ERROR",0,0);
			}

			//오브젝트 셋팅 
			char szObj[40];
			std::string szTok ,  szCol ,  szRow;
			while (fgets(szObj, sizeof(char) * 40, pfile) != NULL)
			{
				//분해 작업
				szTok = szObj;
				int ntok = szTok.find('_');
				int nEnd = szTok.find('\n');
				szCol = szTok.substr(0, ntok);
				szRow = szTok.substr(ntok + 1, nEnd - 2);

				int nRowTemp = 0, nColTemp = 0;
				nColTemp = atoi(szCol.c_str());
				nRowTemp = atoi(szRow.c_str());
				arrWorldMap[nColTemp][nRowTemp] = TILE_OBJECT;
			}
			fclose(pfile);
			
		}
	}
}