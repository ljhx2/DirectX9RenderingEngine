#include "Common.h"
#include "cCollisionBox.h"
#include "cCollisionSphere.h"

namespace ENGINE
{


	cCollisionBox::cCollisionBox(std::string name, cColliderComponent* owner)
	{
		m_name = name;

		m_pOwnerColliderComponent = owner;

		D3DXCreateBox(g_pD3DDevice, 2.0f, 2.0f, 2.0f, &m_mesh, 0);

		m_shapeTpye = eBOX_SHAPE;

		Init();

	}


	cCollisionBox::~cCollisionBox()
	{
		SAFE_RELEASE(m_mesh);
	}

	void cCollisionBox::ComputeWorld()
	{
		D3DXMATRIXA16 world = m_localWorld;
		if (m_pParentWorld != NULL) world = m_localWorld * *m_pParentWorld;

		m_xAxis = D3DXVECTOR3(world._11, world._12, world._13);
		m_yAxis = D3DXVECTOR3(world._21, world._22, world._23);
		m_zAxis = D3DXVECTOR3(world._31, world._32, world._33);

		m_worldScale.x = D3DXVec3Length(&m_xAxis);
		m_worldScale.y = D3DXVec3Length(&m_yAxis);
		m_worldScale.z = D3DXVec3Length(&m_zAxis);

		D3DXVec3Normalize(&m_xAxis, &m_xAxis);
		D3DXVec3Normalize(&m_yAxis, &m_yAxis);
		D3DXVec3Normalize(&m_zAxis, &m_zAxis);

		m_worldPosition = D3DXVECTOR3(world._41, world._42, world._43);

	}

	bool cCollisionBox::CheckCollision(cCollisionShape & other)
	{
		if (other.GetShapeType() == eBOX_SHAPE)
		{
			cCollisionBox* temp = (cCollisionBox*)&other;
			return CheckCollisionBox(temp);
		}
		else if (other.GetShapeType() == eSPHERE_SHAPE)
		{
			cCollisionSphere* temp = (cCollisionSphere*)&other;
			return CheckCollisionSphere(temp);
		}
	}

	bool cCollisionBox::CheckCollisionBox(cCollisionBox & other)
	{
		ComputeWorld();
		other.ComputeWorld();


		//구를 이용해서 미리 검사한다.
		float radius = m_worldScale.x;
		if (radius < m_worldScale.y) radius = m_worldScale.y;
		if (radius < m_worldScale.z) radius = m_worldScale.z;

		float otherRadius = other.m_worldScale.x;
		if (otherRadius < other.m_worldScale.y) otherRadius = other.m_worldScale.y;
		if (otherRadius < other.m_worldScale.z) otherRadius = other.m_worldScale.z;

		D3DXVECTOR3 distance = m_worldPosition - other.m_worldPosition;

		if (D3DXVec3Length(&distance) > radius + otherRadius)
		{
			return false;
		}



		//OBB시작

		D3DXVECTOR3 D = other.m_worldPosition - m_worldPosition;

		float C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
		float absC[3][3]; //|c_{ij}|
		float AD[3];      //Dot(A_i,D)
		float R0, R1, R;  //interval radii and distance between centers
		float R01;        //=R0+R1

		//A0
		C[0][0] = D3DXVec3Dot(&m_xAxis, &other.m_xAxis);
		C[0][1] = D3DXVec3Dot(&m_xAxis, &other.m_yAxis);
		C[0][2] = D3DXVec3Dot(&m_xAxis, &other.m_zAxis);
		AD[0] = D3DXVec3Dot(&m_xAxis, &D);
		absC[0][0] = (float)fabsf(C[0][0]);
		absC[0][1] = (float)fabsf(C[0][1]);
		absC[0][2] = (float)fabsf(C[0][2]);
		R = (float)fabsf(AD[0]);
		R1 = other.m_worldScale.x * absC[0][0] + other.m_worldScale.y * absC[0][1] + other.m_worldScale.z * absC[0][2];
		R01 = m_worldScale.x + R1;
		if (R > R01) return false;

		//A1
		C[1][0] = D3DXVec3Dot(&m_yAxis, &other.m_xAxis);
		C[1][1] = D3DXVec3Dot(&m_yAxis, &other.m_yAxis);
		C[1][2] = D3DXVec3Dot(&m_yAxis, &other.m_zAxis);
		AD[1] = D3DXVec3Dot(&m_yAxis, &D);
		absC[1][0] = (float)fabsf(C[1][0]);
		absC[1][1] = (float)fabsf(C[1][1]);
		absC[1][2] = (float)fabsf(C[1][2]);
		R = (float)fabsf(AD[1]);
		R1 = other.m_worldScale.x * absC[1][0] + other.m_worldScale.y * absC[1][1] + other.m_worldScale.z * absC[1][2];
		R01 = m_worldScale.y + R1;
		if (R > R01)return false;

		//A2
		C[2][0] = D3DXVec3Dot(&m_zAxis, &other.m_xAxis);
		C[2][1] = D3DXVec3Dot(&m_zAxis, &other.m_yAxis);
		C[2][2] = D3DXVec3Dot(&m_zAxis, &other.m_zAxis);
		AD[2] = D3DXVec3Dot(&m_zAxis, &D);
		absC[2][0] = (float)fabsf(C[2][0]);
		absC[2][1] = (float)fabsf(C[2][1]);
		absC[2][2] = (float)fabsf(C[2][2]);
		R = (float)fabsf(AD[2]);
		R1 = other.m_worldScale.x * absC[2][0] + other.m_worldScale.y * absC[2][1] + other.m_worldScale.z * absC[2][2];
		R01 = m_worldScale.z + R1;
		if (R > R01)return false;

		//B0
		R = (float)fabsf(D3DXVec3Dot(&other.m_xAxis, &D));
		R0 = m_worldScale.x * absC[0][0] + m_worldScale.y * absC[1][0] + m_worldScale.z * absC[2][0];
		R01 = R0 + other.m_worldScale.x;
		if (R > R01)return false;

		//B1
		R = (float)fabsf(D3DXVec3Dot(&other.m_yAxis, &D));
		R0 = m_worldScale.x * absC[0][1] + m_worldScale.y * absC[1][1] + m_worldScale.z * absC[2][1];
		R01 = R0 + other.m_worldScale.y;
		if (R > R01)return false;

		//B2
		R = (float)fabsf(D3DXVec3Dot(&other.m_zAxis, &D));
		R0 = m_worldScale.x * absC[0][2] + m_worldScale.y * absC[1][2] + m_worldScale.z * absC[2][2];
		R01 = R0 + other.m_worldScale.z;
		if (R > R01)return false;

		//A0xB0
		R = (float)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
		R0 = m_worldScale.y * absC[2][0] + m_worldScale.z * absC[1][0];
		R1 = other.m_worldScale.y * absC[0][2] + other.m_worldScale.z * absC[0][1];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A0xB1
		R = (float)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
		R0 = m_worldScale.y * absC[2][1] + m_worldScale.z * absC[1][1];
		R1 = other.m_worldScale.x * absC[0][2] + other.m_worldScale.z * absC[0][0];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A0xB2
		R = (float)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
		R0 = m_worldScale.y * absC[2][2] + m_worldScale.z * absC[1][2];
		R1 = other.m_worldScale.x * absC[0][1] + other.m_worldScale.y * absC[0][0];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A1xB0
		R = (float)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
		R0 = m_worldScale.x * absC[2][0] + m_worldScale.z * absC[0][0];
		R1 = other.m_worldScale.y * absC[1][2] + other.m_worldScale.z * absC[1][1];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A1xB1
		R = (float)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
		R0 = m_worldScale.x * absC[2][1] + m_worldScale.z * absC[0][1];
		R1 = other.m_worldScale.x * absC[1][2] + other.m_worldScale.z * absC[1][0];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A1xB2
		R = (float)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
		R0 = m_worldScale.x * absC[2][2] + m_worldScale.z * absC[0][2];
		R1 = other.m_worldScale.x * absC[1][1] + other.m_worldScale.y * absC[1][0];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A2xB0
		R = (float)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
		R0 = m_worldScale.x * absC[1][0] + m_worldScale.y * absC[0][0];
		R1 = other.m_worldScale.y * absC[2][2] + other.m_worldScale.z * absC[2][1];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A2xB1
		R = (float)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
		R0 = m_worldScale.x * absC[1][1] + m_worldScale.y * absC[0][1];
		R1 = other.m_worldScale.x * absC[2][2] + other.m_worldScale.z * absC[2][0];
		R01 = R0 + R1;
		if (R > R01)return false;

		//A2xB2
		R = (float)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
		R0 = m_worldScale.x * absC[1][2] + m_worldScale.y * absC[0][2];
		R1 = other.m_worldScale.x * absC[2][1] + other.m_worldScale.y * absC[2][0];
		R01 = R0 + R1;
		if (R > R01)return false;

		return true;
	}

	bool cCollisionBox::CheckCollisionSphere(cCollisionSphere& other)
	{
		ComputeWorld();


		//구를 이용해서 미리 검사한다.
		float radius = m_worldScale.x;
		if (radius < m_worldScale.y) radius = m_worldScale.y;
		if (radius < m_worldScale.z) radius = m_worldScale.z;

		float otherRadius = other.GetWorldRadius();

		D3DXVECTOR3 distance = m_worldPosition - other.GetWorldCenter();

		if (D3DXVec3Length(&distance) > radius + otherRadius)
		{
			return false;
		}


		//박스와 구 충돌 시작

		float dotX = -D3DXVec3Dot(&m_xAxis, &m_worldPosition);
		float dotY = -D3DXVec3Dot(&m_yAxis, &m_worldPosition);
		float dotZ = -D3DXVec3Dot(&m_zAxis, &m_worldPosition);

		D3DXMATRIXA16 matView;
		matView.m[0][0] = m_xAxis.x;		matView.m[0][1] = m_yAxis.x;		matView.m[0][2] = m_zAxis.x;		matView.m[0][3] = 0.0f;
		matView.m[1][0] = m_xAxis.y;		matView.m[1][1] = m_yAxis.y;		matView.m[1][2] = m_zAxis.y;		matView.m[1][3] = 0.0f;
		matView.m[2][0] = m_xAxis.z;		matView.m[2][1] = m_yAxis.z;		matView.m[2][2] = m_zAxis.z;		matView.m[2][3] = 0.0f;
		matView.m[3][0] = dotX;				matView.m[3][1] = dotY;				matView.m[3][2] = dotZ;				matView.m[3][3] = 1.0f;


		D3DXVECTOR3 sphereCenter = other.GetWorldCenter();
		float sphereRadius = other.GetWorldRadius();

		D3DXVec3TransformCoord(&sphereCenter, &sphereCenter, &matView);

		D3DXVECTOR3 boxPoint;	//Box에서 Sphere와 가장 가까운 점

		if (sphereCenter.x < -m_worldScale.x) boxPoint.x = -m_worldScale.x;
		else if (sphereCenter.x > m_worldScale.x) boxPoint.x = m_worldScale.x;
		else boxPoint.x = sphereCenter.x;

		if (sphereCenter.y < -m_worldScale.y) boxPoint.y = -m_worldScale.y;
		else if (sphereCenter.y > m_worldScale.y) boxPoint.y = m_worldScale.y;
		else boxPoint.y = sphereCenter.y;

		if (sphereCenter.z < -m_worldScale.z) boxPoint.z = -m_worldScale.z;
		else if (sphereCenter.z > m_worldScale.z) boxPoint.z = m_worldScale.z;
		else boxPoint.z = sphereCenter.z;

		D3DXVECTOR3 dist = sphereCenter - boxPoint;

		if (D3DXVec3Length(&dist) <= sphereRadius)
			return true;
		else
			return false;

	}
}