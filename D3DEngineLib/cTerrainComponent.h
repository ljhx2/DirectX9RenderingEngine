#pragma once
#include "IComponent.h"
#include "cTerrainData.h"
 

namespace ENGINE
{  
	class GameObject;
	class cTerrainData;

	class cTerrainComponent : public IComponent
	{
	public:
	//	void SetMouse(POINT mouse);
		bool GetHeightmapPos(OUT D3DXVECTOR3& pos);
		void SaveAllData(const char * szFileName);
		cTerrainData *  GetTerrainData() { return m_pTerrainData; }
	//	cTerrainComponent * m_pCurrentComponent;
	public:
		void Setup(char * szHeightFileName,const char * szTextureFileName);//
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void PrevRender();
		virtual void Render();
	
		char* GetFullPath(){ return m_szFullPath; }
	public:
		cTerrainComponent();
		cTerrainComponent(GameObject* pOwnerObject);
		virtual ~cTerrainComponent();
	private:
		//높이값 받아오기 위해.
		float GetHeightmapCell(int row, int col);
	private: 
		char						m_szFullPath[1024];
		D3DXMATRIXA16*	m_World;
		cTerrainData * m_pTerrainData;

		float m_fWidth, m_fDepth;
	};		 

}