#pragma once 

namespace ENGINE
{
	//내생각엔 Frustum은 카메라 + 터레인 + 쿼드 트리 세개의 조합이다. 
	//씬 부분에서 해줘야 할거 같다 . 카메라의 행렬값은 계속해서 받아오거든.. 
	//내생각엔 카메라 이름에 따라 바꿔줘도 될걱 같다. 

	class cFrustum
	{
		D3DXVECTOR3	m_vtx[8];	/// 프러스텀을 구성할 정점 8개
		D3DXVECTOR3	m_vPos;		/// 현재 카메라의 월드좌표
		D3DXPLANE	m_plane[6];	/// 프러스텀을 구성하는 6개의 평면

		/// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
		const float m_fPlaneEpsilon = 5.0f;

		D3DXVECTOR3 m_center;

	public:
		cFrustum();
		~cFrustum();

		// 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
		//BOOL Make(D3DXMATRIXA16* pmatViewProj, float fPlaneEpsilon);
		BOOL Make(D3DXMATRIXA16* pmatViewProj); // z값이 0~1인 전체 프러스텀 생성

		D3DXVECTOR3 GetCenter() { return m_center; }	//프러스텀의 중점

		D3DXVECTOR3* GetPoint(int i) { return &m_vtx[i]; }

		//-------------------------------
		//과부하가 많이 걸리는 방법 
		// 한점 v가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
		BOOL	IsIn(D3DXVECTOR3* pv);

		/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
		*  TRUE를 반환, 아니면 FALSE를 반환한다.
		*/
		BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);

		// 프러스텀을 화면에 그려준다. -------------엔진에 필요 할거 같음ㅋㅋ 
		BOOL	Draw();

		// 현재 카메라의 월드좌표를 얻어준다.
		D3DXVECTOR3*	GetPos() { return &m_vPos; }

		// 평면의 방정식을 직접 유도하는 소스 -- 왜 사용하지 않지???? 
		void MakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2);

		float GetFarPlanWidth()
		{
			return D3DXVec3Length(&(m_vtx[7] - m_vtx[6]));
		}
		float GetFarPlanHeight()
		{
			return D3DXVec3Length(&(m_vtx[2] - m_vtx[6]));
		}

	};

}