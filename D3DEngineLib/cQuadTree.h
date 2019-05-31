#pragma once 
#include "cFrustum.h"
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)
//SY �߰� 
struct ST_PNTT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t; //��ġ�� u v 
	D3DXVECTOR2	t2; //��ġ�� u v 
	static const DWORD FVF;
};
namespace ENGINE
{
	class cQuadTree
	{
		//8���� �����Ұ� 
		//TL - TOP LEFT / TR - TOP RIGHT /BL - BOTTOM LEFT / BR  -BOTTOM RIGHT
		enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };

		/// ����Ʈ���� �������Ұ��� ����
		enum QuadLocation {
			FRUSTUM_OUT = 0,			// �������ҿ��� �������
			FRUSTUM_PARTIALLY_IN = 1,	// �������ҿ� �κ�����
			FRUSTUM_COMPLETELY_IN = 2,	// �������ҿ� ��������
			FRUSTUM_UNKNOWN = -1		// �𸣰���(^^;)
		};

		/// �̿���� ó���� �����
		enum			{ EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT };
	private:
		int	m_nCenter;			// QuadTree�� ������ ù��° ��					// TopLeft(TL)      TopRight(TR)
		//              0------1
		int	m_nCorner[4];		// QuadTree�� ������ �ι�° ��					//              |      |
		//8���� �����Ұ� 														//              |      |
		cQuadTree*	m_pChild[4];		// QuadTree�� 4���� �ڽĳ��			//              2------3

		cQuadTree*	m_pParent;			/// Triangle Crack(Popping)�� �������ؼ� ����Ѵ�.
		cQuadTree*	m_pNeighbor[4];		/// Triangle Crack(Popping)�� �������ؼ� ����Ѵ�.

	private:
		BOOL m_bCulled;			// �������ҿ��� �ø��� ����ΰ�?
		float m_fRadius;		// ��带 ���δ� ��豸(bounding sphere)�� ������
		// �����尡 �������ҿ� ���ԵǴ°�?
		int	IsInFrustum(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum);

		// IsInFrustum()�Լ��� ����� ���� �������� �ø� ����
		void FrustumCull(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum);
	private:

		// pt�� rc�ȿ� ���ԵǴ��� �˻��Ѵ�.(PtInRect()��� API�Լ��� ����(-)�� ó���� ���ߴ�.)
		inline BOOL IsInRect(RECT* rc, POINT pt)
		{
			if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
				(rc->bottom <= pt.y) && (pt.y <= rc->top))
				return TRUE;

			return FALSE;
		}
		//���� ������ �Ÿ� vector 3 
		inline BOOL GetDistacne(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
		{
			return D3DXVec3Length(&(*pv2 - *pv1));
		}

		//���� ��尡 LOD������� ����  ����� ������ ����ΰ� ?
		BOOL IsVisible(ST_PNTT_VERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
		{
			return ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= GetLODLevel(pHeightMap, pCamera, fLODRatio));
		}
		/// ī�޶�� ���� ������ �Ÿ����� �������� LOD���� ���Ѵ�.
		int GetLODLevel(ST_PNTT_VERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
		{
			float d = GetDistacne((D3DXVECTOR3*)(pHeightMap + m_nCenter), pCamera);
			return max((int)(d * fLODRatio), 1);
			/*D3DXVECTOR3 center(m_nCenter, m_nCenter, m_nCenter);
			float d = GetDistacne(&(pHeightMap->p + center), pCamera);
			return  max((int)(d * fLODRatio), 1);*/
		}
	private:
		// ���� ��尡 ����� ������ ����ΰ�? - non LOD
		//BOOL IsVisible() { return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1); }
		BOOL SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);	// 4���� �ڳʰ��� ����
		cQuadTree * AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);// �ڽ� ��带 �߰�
		BOOL SubDivide();//4���� ���� Ʈ���� �κк��� -> 8���� ���� �Ұ� 

		//����� �������� �ε����� ���� - LOD�� ���� ���� 
		int GenTriIndex(int nTriangles, LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio);

		BOOL BuildQuadTree(ST_PNTT_VERTEX * pHeightMap);
		void BuildNeighborNode(cQuadTree* pRoot, ST_PNTT_VERTEX* pHeightMap, int cx);
		cQuadTree* cQuadTree::FindNode(ST_PNTT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3);
		int	GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
	public:
		cQuadTree(int cx, int cz);	// Root Node�� ����Ұ� 
		cQuadTree(cQuadTree * pParent); //�ڽĵ鸸 ����Ұ� 
		~cQuadTree();

		BOOL Build(ST_PNTT_VERTEX * pHeightMap);// QuadTree�� ����

		//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ - LOD�� ���� ���� 
		int	GenerateIndex(LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio);
	};
}