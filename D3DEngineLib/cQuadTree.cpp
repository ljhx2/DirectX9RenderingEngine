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

		//�ڳʰ� 4�� ���� 
		m_nCorner[CORNER_TL] = 0;
		m_nCorner[CORNER_TR] = cx - 1;
		m_nCorner[CORNER_BL] = cx * (cz - 1);
		m_nCorner[CORNER_BR] = cx * cz - 1;
		m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] +
			m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;


		m_fRadius = 0.0f;
		m_bCulled = FALSE;

	}

	//8���� �����Ұ� 
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
		//������ ���� 
		if (SubDivide())
		{
			// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
			D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TL])) -
				*((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BR]));
			// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
			// 2�� ������ �������� ���Ѵ�.
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

	// �̿���带 �����.(�ﰢ�� ������ ������)
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
			// �̿������ 4�� �ڳʰ��� ��´�.
			n = GetNodeIndex(i, cx, _0, _1, _2, _3);
			// �ڳʰ����� �̿������ �����͸� ���´�.
			if (n >= 0) m_pNeighbor[i] = pRoot->FindNode(pHeightMap, _0, _1, _2, _3);
		}

		// �ڽĳ��� ���ȣ��
		if (m_pChild[0])
		{
			m_pChild[0]->BuildNeighborNode(pRoot, pHeightMap, cx);
			m_pChild[1]->BuildNeighborNode(pRoot, pHeightMap, cx);
			m_pChild[2]->BuildNeighborNode(pRoot, pHeightMap, cx);
			m_pChild[3]->BuildNeighborNode(pRoot, pHeightMap, cx);
		}
	}

	// 4�� ����(���,�ϴ�,����,����)�� �̿���� �ε����� ���Ѵ�.
	int	cQuadTree::GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3)
	{
		int		n, _a, _b, _c, _d, gap;
		_a = _0;
		_b = _1;
		_c = _2;
		_d = _3;
		gap = _b - _a;	// ���� ����� �¿����� -----------------------------------------------������ 

		switch (ed)
		{
		case EDGE_UP:	// ���� ���� �̿������ �ε���
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// �Ʒ� ���� �̿������ �ε���
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// ���� ���� �̿������ �ε���
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// ���� ���� �̿������ �ε���
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if ((_0 / cx) != (_a / cx)) return -1;
			if ((_1 / cx) != (_b / cx)) return -1;
			break;
		}

		n = (_0 + _1 + _2 + _3) / 4;	// ��� �ε���
		if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

		return n;
	}
	// ����Ʈ���� �˻��ؼ� 4�� �ڳʰ��� ��ġ�ϴ� ��带 ã�´�.
	cQuadTree* cQuadTree::FindNode(ST_PNTT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3)
	{
		cQuadTree*	p = NULL;
		// ��ġ�ϴ� ����� ��尪�� ����
		if ((m_nCorner[0] == _0) && (m_nCorner[1] == _1) && (m_nCorner[2] == _2) && (m_nCorner[3] == _3))
			return this;

		// �ڽ� ��尡 �ְ�?
		if (m_pChild[0])
		{
			RECT	rc;
			POINT	pt;
			int n = (_0 + _1 + _2 + _3) / 4;

			// ���� �ʻ󿡼��� ��ġ
			pt.x = (int)pHeightMap[n].p.x;
			pt.y = (int)pHeightMap[n].p.z;

			// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
			SetRect(&rc, (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.x,
				(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.z,
				(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.x,
				(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.z);
			// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
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

	//4���� ���� Ʈ���� �κк��� -> 8���� ���� -----------���Ⱑ �߿� 
	BOOL cQuadTree::SubDivide()
	{
		int		nTopEdgeCenter;
		int		nBottomEdgeCenter;
		int		nLeftEdgeCenter;
		int		nRightEdgeCenter;
		int		nCentralPoint;

		// ��ܺ� ���
		nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
		// �ϴܺ� ��� 
		nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
		// ������ ���
		nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
		// ������ ���
		nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
		// �Ѱ��
		nCentralPoint = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] +
			m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

		// ���̻� ������ �Ұ����Ѱ�? �׷��ٸ� SubDivide() ����
		if ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1)
		{
			return FALSE;
		}

		// 4���� �ڽĳ�� �߰�		// Topleft  ,  ��ܺ� ��� , ������ ���, �Ѱ�� 
		m_pChild[CORNER_TL] = AddChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
		m_pChild[CORNER_TR] = AddChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
		m_pChild[CORNER_BL] = AddChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
		m_pChild[CORNER_BR] = AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

		return TRUE;
	}
	// �ڽ� ��带 �߰�
	cQuadTree * cQuadTree::AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
	{
		cQuadTree*	pChild;

		//�θ� ��带 �޾Ƽ� ���ϵ忡 �־��� 
		pChild = new cQuadTree(this);
		//���ϵ嵵 �ڳ� ������ �ϰԵ� 
		pChild->SetCorners(nCornerTL, nCornerTR, nCornerBL, nCornerBR);

		return pChild;
	}

	// 4���� �ڳʰ��� ����
	BOOL cQuadTree::SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
	{

		// QuadTree�� ������ �ι�° ��
		m_nCorner[CORNER_TL] = nCornerTL;
		m_nCorner[CORNER_TR] = nCornerTR;
		m_nCorner[CORNER_BL] = nCornerBL;
		m_nCorner[CORNER_BR] = nCornerBR;
		/// QuadTree�� ������ ù��° ��
		m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;
		return TRUE;
	}

	// ����� �������� �ε����� ����
	int cQuadTree::GenTriIndex(int nTriangles, LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio)
	{
		// �ø��� ����� �׳� ����
		if (m_bCulled)
		{
			m_bCulled = FALSE;
			return nTriangles;
		}

		LPDWORD p = ((LPDWORD)pIndex) + nTriangles * 3;

		// ���� ��尡 ����� ������ ����ΰ�?
		if (IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio))
		{

			// ���� ������ ����� �κк���(subdivide)�� �Ұ����ϹǷ� �׳� ����ϰ� �����Ѵ�.
			if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
			{
				// ������� �ﰢ��
				*p++ = m_nCorner[0];
				*p++ = m_nCorner[1];
				*p++ = m_nCorner[2];
				nTriangles++;
				// �����ϴ� �ﰢ��
				*p++ = m_nCorner[2];
				*p++ = m_nCorner[1];
				*p++ = m_nCorner[3];
				nTriangles++;

				return nTriangles;
			}

			BOOL	b[4] = { 0, 0, 0, 0 };
			// ��� �̿����(neightbor node)�� ��°����Ѱ�?
			if (m_pNeighbor[EDGE_UP]) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
			// �ϴ� �̿����(neightbor node)�� ��°����Ѱ�?
			if (m_pNeighbor[EDGE_DN]) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
			// ���� �̿����(neightbor node)�� ��°����Ѱ�?
			if (m_pNeighbor[EDGE_LT]) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
			// ���� �̿����(neightbor node)�� ��°����Ѱ�?
			if (m_pNeighbor[EDGE_RT]) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);

			// �̿������� ��δ� ��°����ϴٸ� ������� �̿������� ���� LOD�̹Ƿ� 
			// �κк����� �ʿ����.
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

			if (!b[EDGE_UP]) // ��� �κк����� �ʿ��Ѱ�?
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
			else	// ��� �κк����� �ʿ���� ���
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_TL];
				*p++ = m_nCorner[CORNER_TR];
				nTriangles++;
			}

			if (!b[EDGE_DN]) // �ϴ� �κк����� �ʿ��Ѱ�?
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
			else	// �ϴ� �κк����� �ʿ���� ���
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_BR];
				*p++ = m_nCorner[CORNER_BL];
				nTriangles++;
			}

			if (!b[EDGE_LT]) // ���� �κк����� �ʿ��Ѱ�?
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
			else	// ���� �κк����� �ʿ���� ���
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_BL];
				*p++ = m_nCorner[CORNER_TL];
				nTriangles++;
			}

			if (!b[EDGE_RT]) // ���� �κк����� �ʿ��Ѱ�?
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
			else	// ���� �κк����� �ʿ���� ���
			{
				*p++ = m_nCenter;
				*p++ = m_nCorner[CORNER_TR];
				*p++ = m_nCorner[CORNER_BR];
				nTriangles++;
			}

			return nTriangles;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!

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

		//// ��豸�ȿ� �ִ°�?
		//if( m_fRadius == 0.0f ) 
		//	g_pDebug->Log( "Index:[%d], Radius:[%f]",m_nCenter, m_fRadius );

		bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);
		if (!bInSphere) return FRUSTUM_OUT;	// ��豸 �ȿ� ������ �������� �������� �׽�Ʈ ����

		// ����Ʈ���� 4���� ��� �������� �׽�Ʈ
		b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
		b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
		b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
		b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

		// 4����� �������� �ȿ� ����
		if ((b[0] + b[1] + b[2] + b[3]) == 4) return FRUSTUM_COMPLETELY_IN;

		// �Ϻκ��� �������ҿ� �ִ� ���
		return FRUSTUM_PARTIALLY_IN;
	}

	// _IsInFrustum()�Լ��� ����� ���� �������� �ø� ����
	void cQuadTree::FrustumCull(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum)
	{
		int ret;

		ret = IsInFrustum(pHeightMap, pFrustum);
		switch (ret)
		{
		case FRUSTUM_COMPLETELY_IN:	// �������ҿ� ��������, ������� �˻� �ʿ����
			m_bCulled = FALSE;
			return;
		case FRUSTUM_PARTIALLY_IN:		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
			m_bCulled = FALSE;
			break;
		case FRUSTUM_OUT:				// �������ҿ��� �������, ������� �˻� �ʿ����
			m_bCulled = TRUE;
			return;
		}
		if (m_pChild[0]) m_pChild[0]->FrustumCull(pHeightMap, pFrustum);
		if (m_pChild[1]) m_pChild[1]->FrustumCull(pHeightMap, pFrustum);
		if (m_pChild[2]) m_pChild[2]->FrustumCull(pHeightMap, pFrustum);
		if (m_pChild[3]) m_pChild[3]->FrustumCull(pHeightMap, pFrustum);
	}
	//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ
	int	cQuadTree::GenerateIndex(LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio)
	{
		FrustumCull(pHeightMap, pFrustum);
		return GenTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
	}
}