#include "Common.h"
#include "cCollisionSphere.h"
#include "cCollisionBox.h"

namespace ENGINE
{
	cCollisionSphere::cCollisionSphere(std::string name, cColliderComponent* owner)
	{
		m_name = name;

		m_pOwnerColliderComponent = owner;

		D3DXCreateSphere(g_pD3DDevice, 1.0f, 6U, 6U, &m_mesh, 0);

		m_shapeTpye = eSPHERE_SHAPE;

		Init();
	}


	cCollisionSphere::~cCollisionSphere()
	{
		SAFE_RELEASE(m_mesh);
	}

	D3DXVECTOR3 cCollisionSphere::GetWorldCenter()
	{
		D3DXVECTOR3 center;

		D3DXMATRIXA16 world = m_localWorld;
		if (m_pParentWorld) world *= *m_pParentWorld;

		D3DXVec3TransformCoord(&center, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &world);

		return center;
	}

	float cCollisionSphere::GetWorldRadius()
	{
		//최초 반지름은 1.0f
		D3DXVECTOR3 radius = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

		D3DXMATRIXA16 world = m_localWorld;
		if (m_pParentWorld) world *= *m_pParentWorld;

		D3DXVec3TransformNormal(&radius, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &world);

		return D3DXVec3Length(&radius);

	}

	bool cCollisionSphere::CheckCollisionSphere(cCollisionSphere& other)
	{
		D3DXVECTOR3 center = GetWorldCenter();
		float radius = GetWorldRadius();

		D3DXVECTOR3 otherCenter = other.GetWorldCenter();
		float otherRadius = other.GetWorldRadius();

		D3DXVECTOR3 dist = center - otherCenter;

		if (D3DXVec3Length(&dist) <= (radius + otherRadius))
			return true;
		else
			return false;
	}

	bool cCollisionSphere::CheckCollision(cCollisionShape & other)
	{
		if (other.GetShapeType() == eBOX_SHAPE)
		{
			cCollisionBox* temp = (cCollisionBox*)&other;
			return temp->CheckCollisionSphere(this);
		}
		else if (other.GetShapeType() == eSPHERE_SHAPE)
		{
			cCollisionSphere* temp = (cCollisionSphere*)&other;
			return CheckCollisionSphere(temp);
		}
	}


}