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
		
		//�ͷ��� ����� ���� ���� �� �ؾ��� 
		void SettingWorldMap();
		//d���� 
		int  **GetArrWorldMapInfo() { return arrWorldMap; }

		int GetStartRow() { return m_nStartRow; }
		int GetStartCol() { return m_nStartCol; }
		float GetCellSpacing() { return m_fCellSpacing; }
		int GetCellNum() { return m_nCell; }

	private:
		int **arrWorldMap;
		int m_nCell; //�� ���� 
		float m_fCellSpacing; //�� ���� �Ÿ� 
		int m_nStartRow; //�� ���� 
		int m_nStartCol; //�� ���� 


	};

}