#include "Common.h"
#include "include\D3DRenderer.h"
#include "cQuadTree.h"

const DWORD ST_PNTT_VERTEX::FVF = { D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 };
namespace ENGINE
{
	cQuadTree::cQuadTree(int cx, int cz)
	{
		m_pParent = NULL;
		m_nCenter = 0;
		for (int i = 0; i < 4; ++i)
		{
			m_pChild[i] = NULL;
			m_pNeighbor[i] = NULL;
		}

		//코너값 4개 설정 
		m_nCorner[CORNER_TL] = 0;
		m_nCorner[CORNER_TR] = cx - 1;
		m_nCorner[CORNER_BL] = cx * (cz - 1);
		m_nCorner[CORNER_BR] = cx * cz - 1;
		m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] +
			m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;


		m_fRadius = 0.0f;
		m_bCulled = FALSE;

	}

	//8개로 변경할것 
	cQuadTree::cQuadTree(cQuadTree * pParent)
	{
		m_pParent = pParent;
		m_nCenter = 0;
		for (int i = 0; i < 4; ++i)
		{
			m_pChild[i] = NULL;
			m_nCorner[i] = 0;
			m_pNeighbor[i] = NULL;
		}

		m_bCulled = FALSE;
		m_fRadius = 0.0f;
	}
	cQuadTree::~cQuadTree()
	{
		for (int i = 0; i < 4; i++)
			SAFE_DELETE(m_pChild[i]);
	}
	BOOL cQuadTree::BuildQuadTree(ST_PNTT_VERTEX * pHeightMap)
	{
		//반지름 구함 
		if (SubDivide())
		{
			// 좌측상단과, 우측 하단의 거리를 구한다.
			D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TL])) -
				*((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BR]));
			// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 
			// 2로 나누어 반지름을 구한다.
			m_fRadius = D3DXVec3Length(&v) / 2.0f;
			m_pChild[CORNER_TL]->BuildQuadTree(pHeightMap);
			m_pChild[CORNER_TR]->BuildQuadTree(pHeightMap);
			m_pChild[CORNER_BL]->BuildQuadTree(pHeightMap);
			m_pChild[CORNER_BR]->BuildQuadTree(pHeightMap);
		}
		return TRUE;
	}
	BOOL cQuadTree::Build(ST_PNTT_VERTEX * pHeightMap)
	{

		BuildQuadTree(pHeightMap);
		BuildNeighborNode(this, pHeightMap, m_nCorner[CORNER_TR] + 1);

		return TRUE;
	}

	// 이웃노드를 만든다.(삼각형 찢어짐 방지용)
	void cQuadTree::BuildNeighborNode(cQuadTree* pRoot, ST_PNTT_VERTEX* pHeightMap, int cx)
	{
		int				n;
		int				_0, _1, _2, _3;

		for (int i = 0; i < 4; i++)
		{
			_0 = m_nCorner[0];
			_1 = m_nCorner[1];
			_2 = m_nCorner[2];
			_3 = m_nCorner[3];
			// 이웃노드의 4개 코너값을 얻는다.
			n = GetNodeIndex(i, cx, _0, _1, _2, _3);
			// 코너값으로 이웃노드의 포인터를 얻어온다.
			if (n >= 0) m_pNeighbor[i] = pRoot->FindNode(pHeightMap, _0, _1, _2, _3);
		}

		// 자식노드로 재귀호출
		if (m_pChild[0])
		{
			m_pChild[0]->BuildNeighborNode(pRoot, pHeightMap, cx);
			m_pChild[1]->BuildNeighborNode(pRoot, pHeightMap, cx);
			m_pChild[2]->BuildNeighborNode(pRoot, pHeightMap, cx);
			m_pChild[3]->BuildNeighborNode(pRoot, pHeightMap, cx);
		}
	}

	// 4개 방향(상단,하단,좌측,우측)의 이웃노드 인덱스를 구한다.
	int	cQuadTree::GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3)
	{
		int		n, _a, _b, _c, _d, gap;
		_a = _0;
		_b = _1;
		_c = _2;
		_d = _3;
		gap = _b - _a;	// 현재 노드의 좌우폭값 -----------------------------------------------현재노드 

		switch (ed)
		{
		case EDGE_UP:	// 위쪽 방향 이웃노드의 인덱스
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// 아래 방향 이웃노드의 인덱스
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// 좌측 방향 이웃노드의 인덱스
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// 우측 방향 이웃노드의 인덱스
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if ((_0 / cx) != (_a / cx)) return -1;
			if ((_1 / cx) != (_b / cx)) return -1;
			break;
		}

		n = (_0 + _1 + _2 + _3) / 4;	// 가운데 인덱스
		if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

		return n;
	}
	// 쿼드트리를 검색해서 4개 코너값과 일치하는 노드를 찾는다.
	cQuadTree* cQuadTree::FindNode(ST_PNTT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3)
	{
		cQuadTree*	p = NULL;
		// 일치하는 노드라면 노드값을 리턴
		if ((m_nCorner[0] == _0) && (m_nCorner[1] == _1) && (m_nCorner[2] == _2) && (m_nCorner[3] == _3))
			return this;

		// 자식 노드가 있가?
		if (m_pChild[0])
		{
			RECT	rc;
			POINT	pt;
			int n = (_0 + _1 + _2 + _3) / 4;

			// 현재 맵상에서의 위치
			pt.x = (int)pHeightMap[n].p.x;
			pt.y = (int)pHeightMap[n].p.z;

			// 4개의 코너값을 기준으로 자식노드의 맵 점유범위를 얻는다.
			SetRect(&rc, (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.x,
				(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.z,
				(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.x,
				(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.z);
			// pt값이 점유범위안에 있다면 자식노드로 들어간다.
			if (IsInRect(&rc, pt))
				return m_pChild[0]->FindNode(pHeightMap, _0, _1, _2, _3);

			SetRect(&rc, (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.x,
				(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.z,
				(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.x,
				(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.z);
			if (IsInRect(&rc, pt))
				return m_pChild[1]->FindNode(pHeightMap, _0, _1, _2, _3);

			SetRect(&rc, (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.x,
				(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.z,
				(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.x,
				(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.z);
			if (IsInRect(&rc, pt))
				return m_pChild[2]->FindNode(pHeightMap, _0, _1, _2, _3);

			SetRect(&rc, (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.x,
				(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.z,
				(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.x,
				(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.z);
			if (IsInRect(&rc, pt))
				return m_pChild[3]->FindNode(pHeightMap, _0, _1, _2, _3);
		}

		return NULL;
	}

	//4개의 하위 트리로 부분분할 -> 8개로 변경 -----------여기가 중요 
	BOOL cQuadTree::SubDivide()
	{
		int		nTopEdgeCenter;
		int		nBottomEdgeCenter;
		int		nLeftEdgeCenter;
		int		nRightEdgeCenter;
		int		nCentralPoint;

		// 상단변 가운데
		nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
		// 하단변 가운데 
		nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
		// 좌측변 가운데
		nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
		// 우측변 가운데
		nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
		// 한가운데
		nCentralPoint = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] +
			m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

		// 더이상 분할이 불가능한가? 그렇다면 SubDivide() 종료
		if ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1)
		{
			return FALSE;
		}

		// 4개의 자식노드 추가		// Topleft  ,  상단변 가운데 , 좌측변 가운데, 한가운데 
		m_pChild[CORNER_TL] = AddChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
		m_pChild[CORNER_TR] = AddChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
		m_pChild[CORNER_BL] = AddChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
		m_pChild[CORNER_BR] = AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

		return TRUE;
	}
	// 자식 노드를 추가
	cQuadTree * cQuadTree::AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
	{
		cQuadTree*	pChild;

		//부모 노드를 받아서 차일드에 넣어줌 
		pChild = new cQuadTree(this);
		//차일드도 코너 셋팅을 하게됨 
		pChild->SetCorners(nCornerTL, nCornerTR, nCornerBL, nCornerBR);

		return pChild;
	}

	// 4개의 코너값을 셋팅
	BOOL cQuadTree::SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
	{

		// QuadTree에 보관할 두번째 값
		m_nCorner[CORNER_TL] = nCornerTL;
		m_nCorner[CORNER_TR] = nCornerTR;
		m_nCorner[CORNER_BL] = nCornerBL;
		m_nCorner[CORNER_BR] = nCornerBR;
		/// QuadTree에 보관할 첫번째 값
		m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;
		return TRUE;
	}

	// 출력할 폴리곤의 인덱스를 생성
	int cQuadTree::GenTriIndex(int nTriangles, LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio)
	{
		// 컬링된 노드라면 그냥 리턴
		if (m_bCulled)
		{
			m_bCulled = FALSE;
			return nTriangles;
		}

		LPDWORD p = ((LPDWORD)pIndex) + nTriangles * 3;

		// 현재 노드가 출력이 가능한 노드인가?
		if (IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio))
		{

			// 만약 최하위 노드라면 부분분할(subdivide)이 불가능하므로 그냥 출력하고 리턴한다.
			if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
			{
				// 좌측상단 삼각형
				*p++ = m_nCorner[0];
				*p++ = m_nCorner[1];
				*p++ = m_nCorner[2];
				nTriangles++;
				// 우측하단 삼각형
				*p++ = m_nCorner[2];
				*p++ = m_nCorner[1];
				*p++ = m_nCorner[3];
				nTriangles++;

				return nTriangles;
			}

			BOOL	b[4] = { 0, 0, 0, 0 };
			// 상단 이웃노드(neightbor node)가 출력가능한가?
			if (m_pNeighbor[EDGE_UP]) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
			// 하단 이웃노드(neightbor node)가 출력가능한가?
			if (m_pNeighbor[EDGE_DN]) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
			// 좌측 이웃노드(neightbor node)가 출력가능한가?
			if (m_pNeighbor[EDGE_LT]) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
			// 우측 이웃노드(neightbor node)가 출력가능한가?
			if (m_pNeighbor[EDGE_RT]) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);

			// 이웃노드들이 모두다 출력가능하다면 현재노드와 이웃노드들이 같은 LOD이므로 
			// 부분분할이 필요없다.
			if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
			{
				*p++ = m_nCorner[0];
				*p++ = m_nCorner[1];
				*p++ = m_nCorner[2];
				nTriangles++;
				*p++ = m_nCorner[2];
				*p++ = m_nCorner[1];
				*p++ = m_nCorner[3];
				nTriangles++;
				return nTriangles;
			}

			int		n;

			if (!b[EDGE_UP]) // 상단 부분분할이 필요한가?
			{
				n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_TL];
				*p++ = n; nTriangles++;
				*p++ = m_nCenter;
				*p++ = n;
				*p++ = m_nCorner[CORNER_TR];
				nTriangles++;
			}
			else	// 상단 부분분할이 필요없을 경우
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_TL];
				*p++ = m_nCorner[CORNER_TR];
				nTriangles++;
			}

			if (!b[EDGE_DN]) // 하단 부분분할이 필요한가?
			{
				n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_BR];
				*p++ = n; nTriangles++;
				*p++ = m_nCenter;
				*p++ = n;
				*p++ = m_nCorner[CORNER_BL];
				nTriangles++;
			}
			else	// 하단 부분분할이 필요없을 경우
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_BR];
				*p++ = m_nCorner[CORNER_BL];
				nTriangles++;
			}

			if (!b[EDGE_LT]) // 좌측 부분분할이 필요한가?
			{
				n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_BL];
				*p++ = n; nTriangles++;
				*p++ = m_nCenter;
				*p++ = n;
				*p++ = m_nCorner[CORNER_TL];
				nTriangles++;
			}
			else	// 좌측 부분분할이 필요없을 경우
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_BL];
				*p++ = m_nCorner[CORNER_TL];
				nTriangles++;
			}

			if (!b[EDGE_RT]) // 우측 부분분할이 필요한가?
			{
				n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_TR];
				*p++ = n; nTriangles++;
				*p++ = m_nCenter;
				*p++ = n;
				*p++ = m_nCorner[CORNER_BR];
				nTriangles++;
			}
			else	// 우측 부분분할이 필요없을 경우
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_TR];
				*p++ = m_nCorner[CORNER_BR];
				nTriangles++;
			}

			return nTriangles;	// 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!

		}

		if (m_pChild[CORNER_TL]) nTriangles = m_pChild[CORNER_TL]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);
		if (m_pChild[CORNER_TR]) nTriangles = m_pChild[CORNER_TR]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);
		if (m_pChild[CORNER_BL]) nTriangles = m_pChild[CORNER_BL]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);
		if (m_pChild[CORNER_BR]) nTriangles = m_pChild[CORNER_BR]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);

		return nTriangles;
	}

	int cQuadTree::IsInFrustum(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum)
	{
		BOOL	b[4];
		BOOL	bInSphere;

		//// 경계구안에 있는가?
		//if( m_fRadius == 0.0f ) 
		//	g_pDebug->Log( "Index:[%d], Radius:[%f]",m_nCenter, m_fRadius );

		bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);
		if (!bInSphere) return FRUSTUM_OUT;	// 경계구 안에 없으면 점단위의 프러스텀 테스트 생략

		// 쿼드트리의 4군데 경계 프러스텀 테스트
		b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
		b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
		b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
		b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

		// 4개모두 프러스텀 안에 있음
		if ((b[0] + b[1] + b[2] + b[3]) == 4) return FRUSTUM_COMPLETELY_IN;

		// 일부분이 프러스텀에 있는 경우
		return FRUSTUM_PARTIALLY_IN;
	}

	// _IsInFrustum()함수의 결과에 따라 프러스텀 컬링 수행
	void cQuadTree::FrustumCull(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum)
	{
		int ret;

		ret = IsInFrustum(pHeightMap, pFrustum);
		switch (ret)
		{
		case FRUSTUM_COMPLETELY_IN:	// 프러스텀에 완전포함, 하위노드 검색 필요없음
			m_bCulled = FALSE;
			return;
		case FRUSTUM_PARTIALLY_IN:		// 프러스텀에 일부포함, 하위노드 검색 필요함
			m_bCulled = FALSE;
			break;
		case FRUSTUM_OUT:				// 프러스텀에서 완전벗어남, 하위노드 검색 필요없음
			m_bCulled = TRUE;
			return;
		}
		if (m_pChild[0]) m_pChild[0]->FrustumCull(pHeightMap, pFrustum);
		if (m_pChild[1]) m_pChild[1]->FrustumCull(pHeightMap, pFrustum);
		if (m_pChild[2]) m_pChild[2]->FrustumCull(pHeightMap, pFrustum);
		if (m_pChild[3]) m_pChild[3]->FrustumCull(pHeightMap, pFrustum);
	}
	//	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환
	int	cQuadTree::GenerateIndex(LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio)
	{
		FrustumCull(pHeightMap, pFrustum);
		return GenTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
	}
}