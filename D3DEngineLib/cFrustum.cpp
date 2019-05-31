#include "Common.h"
#include "cFrustum.h"

namespace ENGINE
{
	cFrustum::cFrustum()
	{
		ZeroMemory(m_vtx, sizeof(m_vtx[0]) * 8);
		ZeroMemory(m_plane, sizeof(m_plane[0]) * 6);
	}
	cFrustum::~cFrustum()
	{

	}

	//이부분을 좀 더 이해 해야 할듯.. 
	// 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
	BOOL cFrustum::Make(D3DXMATRIXA16* pmatViewProj)
	{
		int				i;
		D3DXMATRIXA16	matInv;

		// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
		// m_vtx에 이 동차공간의 경계값을 넣어둔다.
		m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
		m_vtx[1].x = 1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
		m_vtx[2].x = 1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
		m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
		m_vtx[4].x = -1.0f;	m_vtx[4].y = 1.0f;	m_vtx[4].z = 0.0f;
		m_vtx[5].x = 1.0f;	m_vtx[5].y = 1.0f;	m_vtx[5].z = 0.0f;
		m_vtx[6].x = 1.0f;	m_vtx[6].y = 1.0f;	m_vtx[6].z = 1.0f;
		m_vtx[7].x = -1.0f;	m_vtx[7].y = 1.0f;	m_vtx[7].z = 1.0f;

		// view * proj의 역행렬을 구한다.
		D3DXMatrixInverse(&matInv, NULL, pmatViewProj);

		//-----------------------------
		//정확하게 이해 할것  공부 할 것 
		// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,
		// Vertex_world = Vertex_local * Matrix_world이므로,
		// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.
		// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서
		// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면
		// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.
		// 그러므로, m_vtx * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
		for (i = 0; i < 8; i++)
			D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

		m_center.x = 0.0f; m_center.y = 0.0f; m_center.z = 0.0f;
		for (i = 0; i < 8; i++)
		{
			m_center += m_vtx[i];
		}

		m_center = m_center / 8.0f;

		// 0번과 5번은 프러스텀중 near평면의 좌측상단과 우측하단이므로, 둘의 좌표를 더해서 2로 나누면
		// 카메라의 좌표를 얻을 수 있다.(정확히 일치하는 것은 아니다.)
		m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

		// 얻어진 월드좌표로 프러스텀 평면을 만든다
		// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
		D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// 상 평면(top)
		D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// 하 평면(bottom)
		D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// 근 평면(near)
		D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);	// 원 평면(far)
		D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7);	// 좌 평면(left)
		D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);	// 우 평면(right)

		return TRUE;
	}

	//-------------------------------
	//과부하가 많이 걸리는 방법 
	// 한점 v가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
	BOOL cFrustum::IsIn(D3DXVECTOR3* pv)
	{
		float		fDist;

		fDist = D3DXPlaneDotCoord(&m_plane[0], pv);
		if (fDist > m_fPlaneEpsilon) return FALSE;	// 점이 상평면보다 위에 있다
		fDist = D3DXPlaneDotCoord(&m_plane[1], pv);
		if (fDist > m_fPlaneEpsilon) return FALSE;	// 점이 하평면보다 아래에 있다
		fDist = D3DXPlaneDotCoord(&m_plane[2], pv);
		if (fDist > m_fPlaneEpsilon) return FALSE;	// 점이 근평면보다 안에 있다
		fDist = D3DXPlaneDotCoord(&m_plane[3], pv);
		if (fDist > m_fPlaneEpsilon) return FALSE;	// plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
		fDist = D3DXPlaneDotCoord(&m_plane[4], pv);
		if (fDist > m_fPlaneEpsilon) return FALSE;	// plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
		fDist = D3DXPlaneDotCoord(&m_plane[5], pv);
		if (fDist > m_fPlaneEpsilon) return FALSE;	// plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽

		return TRUE;
	}

	/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
	*  TRUE를 반환, 아니면 FALSE를 반환한다.
	*/
	BOOL cFrustum::IsInSphere(D3DXVECTOR3* pv, float radius)
	{
		float		fDist;

		fDist = D3DXPlaneDotCoord(&m_plane[0], pv);
		if (fDist > (radius + m_fPlaneEpsilon)) return FALSE;
		fDist = D3DXPlaneDotCoord(&m_plane[1], pv);
		if (fDist > (radius + m_fPlaneEpsilon)) return FALSE;
		fDist = D3DXPlaneDotCoord(&m_plane[2], pv);
		if (fDist > (radius + m_fPlaneEpsilon)) return FALSE;

		fDist = D3DXPlaneDotCoord(&m_plane[3], pv);
		if (fDist > (radius + m_fPlaneEpsilon)) return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
		fDist = D3DXPlaneDotCoord(&m_plane[4], pv);
		if (fDist > (radius + m_fPlaneEpsilon)) return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
		fDist = D3DXPlaneDotCoord(&m_plane[5], pv);
		if (fDist > (radius + m_fPlaneEpsilon)) return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

		return TRUE;
	}

	void cFrustum::MakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
	{
		D3DXPlaneFromPoints(pPlane, pv0, pv1, pv2);
		//  평면의 방정식을 직접 유도하는 소스
		//	for OpenGL
		//	D3DXVECTOR3	v0, v1, v2;
		//	v1 = *pv1 - *pv0;
		//	v2 = *pv2 - *pv0;
		//	D3DXVec3Cross( &v0, &v1, &v2 );
		//	D3DXVec3Normalize( &v0, &v0 );

		//	pPlane->a = v0.x;
		//	pPlane->b = v0.y;
		//	pPlane->c = v0.z;
		//	pPlane->d = -( v0.x * pv0->x + v0.y * pv0->y + v0.z * pv0->z );

	}
	BOOL cFrustum::Draw()
	{
		WORD		index[] = { 0, 1, 2,
			0, 2, 3,
			4, 7, 6,
			4, 6, 5,
			1, 5, 6,
			1, 6, 2,
			0, 3, 7,
			0, 7, 4,
			0, 4, 5,
			0, 5, 1,
			3, 7, 6,
			3, 6, 2 };

		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

		typedef struct tagVTX
		{
			D3DXVECTOR3	p;
		} VTX;

		VTX		vtx[8];

		for (int i = 0; i < 8; i++)
			vtx[i].p = m_vtx[i];

		g_pD3DDevice->SetFVF(D3DFVF_XYZ);
		g_pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(VTX));
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetIndices(0);
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// 파란색으로 상,하 평면을 그린다.
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		g_pD3DDevice->SetMaterial(&mtrl);
		g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		// 녹색으로 좌,우 평면을 그린다.
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		g_pD3DDevice->SetMaterial(&mtrl);
		g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		// 붉은색으로 원,근 평면을 그린다.
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		g_pD3DDevice->SetMaterial(&mtrl);
		g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

		return TRUE;
	}
}