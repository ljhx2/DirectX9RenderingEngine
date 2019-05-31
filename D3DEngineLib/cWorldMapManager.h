#pragma once

#define g_pWorldMapanager cWorldMapManger::GetInstance()

namespace ENGINE
{
	enum ST_TILE_STATE
	{
		TILE_NONE_SOMETHING = 0,
		TILE_OBJECT = 100,
	};
	class cWorldMapManger
	{
	private:
		cWorldMapManger();
		~cWorldMapManger();

	public:
		static cWorldMapManger* GetInstance() { static cWorldMapManger instance; return &instance; }
		
		//터레인 생기고 나서 셋팅 꼭 해야함 
		void SettingWorldMap();
		//d으음 
		int  **GetArrWorldMapInfo() { return arrWorldMap; }

		int GetStartRow() { return m_nStartRow; }
		int GetStartCol() { return m_nStartCol; }
		float GetCellSpacing() { return m_fCellSpacing; }
		int GetCellNum() { return m_nCell; }

	private:
		int **arrWorldMap;
		int m_nCell; //셀 개수 
		float m_fCellSpacing; //각 셀당 거리 
		int m_nStartRow; //셀 개수 
		int m_nStartCol; //셀 개수 


	};

}