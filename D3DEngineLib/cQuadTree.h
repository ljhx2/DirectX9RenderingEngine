#pragma once 
#include "cFrustum.h"
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)
//SY 추가 
struct ST_PNTT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t; //위치값 u v 
	D3DXVECTOR2	t2; //위치값 u v 
	static const DWORD FVF;
};
namespace ENGINE
{
	class cQuadTree
	{
		//8개로 변경할것 
		//TL - TOP LEFT / TR - TOP RIGHT /BL - BOTTOM LEFT / BR  -BOTTOM RIGHT
		enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };

		/// 쿼드트리와 프러스텀간의 관계
		enum QuadLocation {
			FRUSTUM_OUT = 0,			// 프러스텀에서 완전벗어남
			FRUSTUM_PARTIALLY_IN = 1,	// 프러스텀에 부분포함
			FRUSTUM_COMPLETELY_IN = 2,	// 프러스텀에 완전포함
			FRUSTUM_UNKNOWN = -1		// 모르겠음(^^;)
		};

		/// 이웃노드 처리용 상수값
		enum			{ EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT };
	private:
		int	m_nCenter;			// QuadTree에 보관할 첫번째 값					// TopLeft(TL)      TopRight(TR)
		//              0------1
		int	m_nCorner[4];		// QuadTree에 보관할 두번째 값					//              |      |
		//8개로 변경할것 														//              |      |
		cQuadTree*	m_pChild[4];		// QuadTree의 4개의 자식노드			//              2------3

		cQuadTree*	m_pParent;			/// Triangle Crack(Popping)을 막기위해서 사용한다.
		cQuadTree*	m_pNeighbor[4];		/// Triangle Crack(Popping)을 막기위해서 사용한다.

	private:
		BOOL m_bCulled;			// 프러스텀에서 컬링된 노드인가?
		float m_fRadius;		// 노드를 감싸는 경계구(bounding sphere)의 반지름
		// 현재노드가 프러스텀에 포함되는가?
		int	IsInFrustum(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum);

		// IsInFrustum()함수의 결과에 따라 프러스텀 컬링 수행
		void FrustumCull(ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum);
	private:

		// pt가 rc안에 포함되는지 검사한다.(PtInRect()라는 API함수는 음수(-)값 처리를 못했다.)
		inline BOOL IsInRect(RECT* rc, POINT pt)
		{
			if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
				(rc->bottom <= pt.y) && (pt.y <= rc->top))
				return TRUE;

			return FALSE;
		}
		//두점 사이의 거리 vector 3 
		inline BOOL GetDistacne(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
		{
			return D3DXVec3Length(&(*pv2 - *pv1));
		}

		//현재 노드가 LOD등급으로 볼때  출력이 가능한 노드인가 ?
		BOOL IsVisible(ST_PNTT_VERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
		{
			return ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= GetLODLevel(pHeightMap, pCamera, fLODRatio));
		}
		/// 카메라와 현재 노드와의 거리값을 기준으로 LOD값을 구한다.
		int GetLODLevel(ST_PNTT_VERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
		{
			float d = GetDistacne((D3DXVECTOR3*)(pHeightMap + m_nCenter), pCamera);
			return max((int)(d * fLODRatio), 1);
			/*D3DXVECTOR3 center(m_nCenter, m_nCenter, m_nCenter);
			float d = GetDistacne(&(pHeightMap->p + center), pCamera);
			return  max((int)(d * fLODRatio), 1);*/
		}
	private:
		// 현재 노드가 출력이 가능한 노드인가? - non LOD
		//BOOL IsVisible() { return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1); }
		BOOL SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);	// 4개의 코너값을 셋팅
		cQuadTree * AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);// 자식 노드를 추가
		BOOL SubDivide();//4개의 하위 트리로 부분분할 -> 8개로 변경 할것 

		//출력할 폴리곤의 인덱스를 생성 - LOD로 인해 변경 
		int GenTriIndex(int nTriangles, LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio);

		BOOL BuildQuadTree(ST_PNTT_VERTEX * pHeightMap);
		void BuildNeighborNode(cQuadTree* pRoot, ST_PNTT_VERTEX* pHeightMap, int cx);
		cQuadTree* cQuadTree::FindNode(ST_PNTT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3);
		int	GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
	public:
		cQuadTree(int cx, int cz);	// Root Node가 사용할것 
		cQuadTree(cQuadTree * pParent); //자식들만 사용할것 
		~cQuadTree();

		BOOL Build(ST_PNTT_VERTEX * pHeightMap);// QuadTree를 구축

		//	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환 - LOD로 인해 변경 
		int	GenerateIndex(LPVOID pIndex, ST_PNTT_VERTEX* pHeightMap, cFrustum* pFrustum, float fLODRatio);
	};
}