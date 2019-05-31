#include "Common.h"
#include "include\D3DRenderer.h"
#include "cStaticMeshComponent.h"
#include "cObjParser.h"


const DWORD ST_PNT_VERTEX2::FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

namespace ENGINE
{
	cStaticMeshComponent::cStaticMeshComponent()
	{
		eComponent_Type = eSTATIC_MESH_TYPE;
		m_pOwnerObject = NULL;

		m_matWorld = NULL;

		m_vMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		m_vMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	cStaticMeshComponent::cStaticMeshComponent(GameObject* pOwnerObject)
	{
		eComponent_Type = eSTATIC_MESH_TYPE;
		m_pOwnerObject = pOwnerObject;

		m_matWorld = NULL;

		m_vMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		m_vMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}
	cStaticMeshComponent::cStaticMeshComponent(GameObject* pOwnerObject, char* szFolder, const char* szFilename)
	{
		eComponent_Type = eSTATIC_MESH_TYPE;
		m_pOwnerObject = pOwnerObject;

		cStaticMeshComponent* pStaticMesh = g_pStaticMeshManager->GetStaticMesh(szFolder, szFilename);

		m_Groups.assign(pStaticMesh->m_Groups.begin(), pStaticMesh->m_Groups.end());
		m_MtlTexMap = pStaticMesh->m_MtlTexMap;

		m_matWorld = NULL;

		m_vMin = pStaticMesh->m_vMin;
		m_vMax = pStaticMesh->m_vMax;
	}


	cStaticMeshComponent::~cStaticMeshComponent()
	{
	}

	void cStaticMeshComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		m_matWorld = matWorld;
	}

	void cStaticMeshComponent::Render()
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, m_matWorld);

		g_pD3DDevice->SetFVF(ST_PNT_VERTEX2::FVF);
		for (size_t i = 0; i < m_Groups.size(); ++i)
		{
			if (m_Groups[i]->pVB == NULL)
				continue;
			g_pD3DDevice->SetMaterial(&m_MtlTexMap[m_Groups[i]->sMtlName].Mtl);
			g_pD3DDevice->SetTexture(0, m_MtlTexMap[m_Groups[i]->sMtlName].pTex);
			g_pD3DDevice->SetStreamSource(0, m_Groups[i]->pVB, 0, sizeof(ST_PNT_VERTEX2));
			g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Groups[i]->nNumTri);
		}
	}

	void cStaticMeshComponent::Destroy()	//버텍스 버퍼 릴리즈(staticmeshManager에서만 호출)
	{
		for each(auto p in m_Groups)
		{
			SAFE_RELEASE(p->pVB);
			SAFE_DELETE(p);
		}
	}

	void cStaticMeshComponent::Load(char* szFolder, const char* szFilename)
	{
		//TODO 파싱해서
		//멤버 변수 채우기

		std::string filepath(szFolder);
		filepath += std::string(szFilename);

		cObjParser parser;
		parser.Load(filepath.c_str(), m_Groups, m_MtlTexMap);

		//Min, Max구하기
		for (size_t i = 0; i < m_Groups.size(); i++)
		{
			if (m_vMin.x > m_Groups[i]->vMin.x) m_vMin.x = m_Groups[i]->vMin.x;
			if (m_vMin.y > m_Groups[i]->vMin.y) m_vMin.y = m_Groups[i]->vMin.y;
			if (m_vMin.z > m_Groups[i]->vMin.z) m_vMin.z = m_Groups[i]->vMin.z;
			if (m_vMax.x < m_Groups[i]->vMax.x) m_vMax.x = m_Groups[i]->vMax.x;
			if (m_vMax.y < m_Groups[i]->vMax.y) m_vMax.y = m_Groups[i]->vMax.y;
			if (m_vMax.z < m_Groups[i]->vMax.z) m_vMax.z = m_Groups[i]->vMax.z;
		}
	}

	bool cStaticMeshComponent::IntersectRayMesh(D3DXVECTOR3 origin, D3DXVECTOR3 dir)
	{
		// orgin과 dir을 로컬좌표 기준으로 바꿔준다.
		D3DXMATRIXA16 worldInverse;
		D3DXMatrixInverse(&worldInverse, 0, m_matWorld);

		D3DXVECTOR3 localOrigin, localDir;
		D3DXVec3TransformCoord(&localOrigin, &origin, &worldInverse);
		D3DXVec3TransformNormal(&localDir, &dir, &worldInverse);
		D3DXVec3Normalize(&localDir, &localDir);

		FLOAT fDist = FLT_MAX;
		FLOAT fU, fV;
		ST_PNT_VERTEX2* pVertices;

		for (size_t i = 0; i < m_Groups.size(); i++)
		{
			m_Groups[i]->pVB->Lock(0, 0, (void**)&pVertices, 0);

			for (size_t j = 0; j < m_Groups[i]->nNumTri; j++)
			{
				D3DXVECTOR3 v0 = pVertices[3 * j + 0].p;
				D3DXVECTOR3 v1 = pVertices[3 * j + 1].p;
				D3DXVECTOR3 v2 = pVertices[3 * j + 2].p;

				if (IntersectTriangle(localOrigin, localDir, v0, v1, v2, &fDist, &fU, &fV))
				{
					m_Groups[i]->pVB->Unlock();
					return true;
				}
			}
			m_Groups[i]->pVB->Unlock();
		}
		return false;
	}

	bool cStaticMeshComponent::IntersectTriangle(IN const D3DXVECTOR3& orig, IN const D3DXVECTOR3& dir,
		IN D3DXVECTOR3& v0, IN D3DXVECTOR3& v1, IN D3DXVECTOR3& v2, OUT FLOAT* dist, OUT FLOAT* u, OUT FLOAT* v)
	{
		// Find vectors for two edges sharing vert0
		D3DXVECTOR3 edge1 = v1 - v0;
		D3DXVECTOR3 edge2 = v2 - v0;

		// Begin calculating determinant - also used to calculate U parameter
		D3DXVECTOR3 pvec;
		D3DXVec3Cross(&pvec, &dir, &edge2);

		// If determinant is near zero, ray lies in plane of triangle
		FLOAT det = D3DXVec3Dot(&edge1, &pvec);

		D3DXVECTOR3 tvec;
		if (det > 0)
		{
			tvec = orig - v0;
		}
		else
		{
			tvec = v0 - orig;
			det = -det;
		}

		if (det < 0.0001f)
			return FALSE;

		// Calculate U parameter and test bounds
		*u = D3DXVec3Dot(&tvec, &pvec);
		if (*u < 0.0f || *u > det)
			return FALSE;

		// Prepare to test V parameter
		D3DXVECTOR3 qvec;
		D3DXVec3Cross(&qvec, &tvec, &edge1);

		// Calculate V parameter and test bounds
		*v = D3DXVec3Dot(&dir, &qvec);
		if (*v < 0.0f || *u + *v > det)
			return FALSE;

		// Calculate t, scale parameters, ray intersects triangle
		*dist = D3DXVec3Dot(&edge2, &qvec);
		FLOAT fInvDet = 1.0f / det;
		*dist *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;

		return TRUE;
	}
}