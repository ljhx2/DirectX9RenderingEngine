#pragma once
#include "IComponent.h"

#include <map>


struct ST_PNT_VERTEX2
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;
	static const DWORD FVF;
};
struct ST_GROUP
{
	int						nNumTri;
	std::string				sMtlName;
	LPDIRECT3DVERTEXBUFFER9 pVB;
	D3DXVECTOR3				vMin;
	D3DXVECTOR3				vMax;
	ST_GROUP() : nNumTri(0), pVB(NULL) {}
};

struct ST_MTL_TEX
{
	D3DMATERIAL9			Mtl;
	LPDIRECT3DTEXTURE9		pTex;
	ST_MTL_TEX() : pTex(NULL)
	{
		memset(&Mtl, 0, sizeof(D3DMATERIAL9));
	}
};


namespace ENGINE
{
	class GameObject;

	class cStaticMeshComponent : public IComponent
	{
	public:
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();

		D3DXVECTOR3 GetMaxPoint() { return m_vMax; }
		D3DXVECTOR3	GetMinPoint() { return m_vMin; }

		void Destroy();	//버텍스 버퍼 릴리즈(staticmeshManager에서만 호출)
		void Load(char* szFolder, const char* szFilename);


		bool IntersectRayMesh(D3DXVECTOR3 origin, D3DXVECTOR3 dir); //픽킹에서사용

		cStaticMeshComponent();
		cStaticMeshComponent(GameObject* pOwnerObject);
		cStaticMeshComponent(GameObject* pOwnerObject, char* szFolder, const char* szFilename);
		virtual ~cStaticMeshComponent();

	private:

		bool IntersectTriangle(IN const D3DXVECTOR3& orig, IN const D3DXVECTOR3& dir,
			IN D3DXVECTOR3& v0, IN D3DXVECTOR3& v1, IN D3DXVECTOR3& v2, OUT FLOAT* dist, OUT FLOAT* u, OUT FLOAT* v);

	private:
		D3DXVECTOR3							m_vMin;
		D3DXVECTOR3							m_vMax;
		std::vector<ST_GROUP*>				m_Groups;
		std::map<std::string, ST_MTL_TEX>	m_MtlTexMap;
		D3DXMATRIXA16*						m_matWorld;

	};

}