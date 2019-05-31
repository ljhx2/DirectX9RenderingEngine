#pragma once 

namespace ENGINE
{
	//�������� Frustum�� ī�޶� + �ͷ��� + ���� Ʈ�� ������ �����̴�. 
	//�� �κп��� ����� �Ұ� ���� . ī�޶��� ��İ��� ����ؼ� �޾ƿ��ŵ�.. 
	//�������� ī�޶� �̸��� ���� �ٲ��൵ �ɰ� ����. 

	class cFrustum
	{
		D3DXVECTOR3	m_vtx[8];	/// ���������� ������ ���� 8��
		D3DXVECTOR3	m_vPos;		/// ���� ī�޶��� ������ǥ
		D3DXPLANE	m_plane[6];	/// ���������� �����ϴ� 6���� ���

		/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
		const float m_fPlaneEpsilon = 5.0f;

		D3DXVECTOR3 m_center;

	public:
		cFrustum();
		~cFrustum();

		// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
		//BOOL Make(D3DXMATRIXA16* pmatViewProj, float fPlaneEpsilon);
		BOOL Make(D3DXMATRIXA16* pmatViewProj); // z���� 0~1�� ��ü �������� ����

		D3DXVECTOR3 GetCenter() { return m_center; }	//���������� ����

		D3DXVECTOR3* GetPoint(int i) { return &m_vtx[i]; }

		//-------------------------------
		//�����ϰ� ���� �ɸ��� ��� 
		// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
		BOOL	IsIn(D3DXVECTOR3* pv);

		/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
		*  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
		*/
		BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);

		// ���������� ȭ�鿡 �׷��ش�. -------------������ �ʿ� �Ұ� �������� 
		BOOL	Draw();

		// ���� ī�޶��� ������ǥ�� ����ش�.
		D3DXVECTOR3*	GetPos() { return &m_vPos; }

		// ����� �������� ���� �����ϴ� �ҽ� -- �� ������� ����???? 
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