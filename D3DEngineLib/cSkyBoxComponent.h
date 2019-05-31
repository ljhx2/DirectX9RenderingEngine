#pragma once
#include "IComponent.h"
namespace ENGINE
{
	class cSkyBoxComponent : public IComponent
	{
		struct ST_PNT_VERTEX
		{
			D3DXVECTOR3 p;
			D3DXVECTOR3 n;
			D3DXVECTOR2	t;
			static const DWORD FVF;
		};
	public:
		cSkyBoxComponent();
		cSkyBoxComponent(GameObject* pOwnerObject, char* fileName);
		cSkyBoxComponent(GameObject* pOwnerObject, char* top, char* bottom, char* left, char* right, char* front, char* back);
		~cSkyBoxComponent();

		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();


		void ChangeTexture(char * fileName);
		void SaveSkyBox(const char * fileName);
	public :
		BOOL GetIsTexOnce() { return bIsTexOnce; }
		std::string *  GetIsTexName() { return m_szFileName; }

	private:
		std::vector<ST_PNT_VERTEX>	m_Vertex;
		LPDIRECT3DTEXTURE9 mMultiTex[6];	//고치자 

		std::string m_szFileName[6];
		 
		D3DXMATRIXA16 m_mScale;
		D3DXMATRIXA16 m_mWorld;
		D3DXMATRIXA16 m_mCamera;

		//환경맵핑
		LPD3DXEFFECT	m_pEnvEffect;		//sky sphere 환경맵핑
		LPD3DXMESH				m_pMesh;
		LPDIRECT3DCUBETEXTURE9 m_pEnvTex;

		BOOL bIsTexOnce;
	};

}