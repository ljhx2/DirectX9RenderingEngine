#include "Common.h"
#include "include\D3DRenderer.h"
#include "cTerrainData.h"
#include "cTerrainComponent.h"


namespace ENGINE
{

	cTerrainComponent::cTerrainComponent()
	{
		eComponent_Type = eTERRAIN_TYPE; 
		m_pTerrainData = NULL; 
	}
	cTerrainComponent::cTerrainComponent(GameObject* pOwnerObject)
	{
		eComponent_Type = eTERRAIN_TYPE;
		m_pOwnerObject = pOwnerObject;
		m_pTerrainData = NULL;
	}


	cTerrainComponent::~cTerrainComponent()
	{
		SAFE_DELETE(m_pTerrainData);
	}


	void cTerrainComponent::Setup(char * szHeightFileName, const char * szTextureFileName)
	{
		strcpy_s(m_szFullPath, szHeightFileName);
		m_pTerrainData = g_pTerrainManager->GetTerrain(szHeightFileName, szTextureFileName);

		m_fWidth = m_pTerrainData->GetWidth();
		m_fDepth = m_pTerrainData->GetDepth();
	}

	bool cTerrainComponent::GetHeightmapPos(OUT D3DXVECTOR3& pos)
	{
		return m_pTerrainData->GetHeightmapPos(pos);
	}

	//void cTerrainComponent::SetMouse(POINT mouse)
	//{
	//	m_pTerrainData->SetMouse(mouse);
	//	m_pTerrainData->UpdateBrush();
	//}
	void cTerrainComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(matWorld->_41, matWorld->_42, matWorld->_43);
		
		D3DXVECTOR3 vMin = D3DXVECTOR3(-m_fWidth / 2.0f, 0.0f, -m_fDepth / 2.0f);
		D3DXVECTOR3 vMax = D3DXVECTOR3(m_fWidth / 2.0f, 0.0f, m_fDepth / 2.0f);
		D3DXVec3TransformCoord(&vMin, &vMin, matWorld);
		D3DXVec3TransformCoord(&vMax, &vMax, matWorld);
		D3DXVECTOR3 center = (vMin + vMax) / 2.0f;
		float radius = D3DXVec3Length(&(vMax - center));

		g_pShadowManager->AllocGameObjectToShadowMap(pos, radius, m_pOwnerObject);


		m_World = matWorld;
		D3DXVECTOR3 vPos;
		m_pTerrainData->MousePicking(vPos);
	}
	void cTerrainComponent::SaveAllData(const char * szFileName)
	{
		std::string szPath("Terrain/");
		std::string szName(szFileName);
		szPath += szName;
		m_pTerrainData->SaveHeightMap(szPath.c_str());
		szPath = "Terrain/Texture/";
		szName.replace(szName.size() - 3, 3, "png");
		szPath += szName;
		m_pTerrainData->SaveAlphaMap(szPath.c_str());
	}
	void cTerrainComponent::PrevRender()
	{
		//m_pTerrainData->PrevRender(m_World);
	}

	void cTerrainComponent::Render()
	{
		m_pTerrainData->Render(m_World);
	}
}