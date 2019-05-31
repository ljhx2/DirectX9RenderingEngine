#include "Common.h"
#include "cCollisionShape.h"
#include "cColliderComponent.h"
#include "include\D3DRenderer.h"

namespace ENGINE
{

	cCollisionShape::cCollisionShape()
	{
	}


	cCollisionShape::~cCollisionShape()
	{
	}

	void cCollisionShape::Init()
	{
		m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_localRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMatrixIdentity(&m_localWorld);

		m_pParentWorld = NULL;
		m_parentName = "NULL";

		m_isRender = true;


		vecPoint = 0;
		Max_vecPoint = 10;

		m_vecClashed.assign(Max_vecPoint, NULL);
		m_vecPrevClashed.assign(Max_vecPoint, NULL);
	}

	void cCollisionShape::UpdateLocalWorld()
	{
		D3DXMATRIXA16 scale, rotation, position;
		D3DXMatrixScaling(&scale, m_localScale.x, m_localScale.y, m_localScale.z);
		D3DXMatrixRotationYawPitchRoll(&rotation, m_localRotation.y, m_localRotation.x, m_localRotation.z);
		D3DXMatrixTranslation(&position, m_localPosition.x, m_localPosition.y, m_localPosition.z);

		m_localWorld = scale * rotation * position;
	}

	void cCollisionShape::Render()
	{
		if (m_isRender)
		{
			D3DXMATRIXA16 world = m_localWorld;
			if (m_pParentWorld) world *= *m_pParentWorld;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);
			m_mesh->DrawSubset(0);
		}
	}

	GameObject* cCollisionShape::GetOwnerGameObject()
	{
		return m_pOwnerColliderComponent->GetOwnerObject();
	}

	void cCollisionShape::Clashed(cCollisionShape* col)
	{
		if (vecPoint >= Max_vecPoint) return;
		m_vecClashed[vecPoint] = col;
		vecPoint++;

		for (int i = 0; i < Max_vecPoint; i++)
		{
			if (col == m_vecPrevClashed[i])
			{
				GetOwnerGameObject()->ActionCallbackOnCollisionStay(this, col);
				return;
			}
			else if (m_vecPrevClashed[i] == NULL)
			{
				GetOwnerGameObject()->ActionCallbackOnCollisionEnter(this, col);
				return;
			}
		}
	}

	void cCollisionShape::ClearClashedVec()
	{
		for (int i = 0; i < Max_vecPoint; i++)
		{
			m_vecPrevClashed[i] = m_vecClashed[i];
			m_vecClashed[i] = NULL;
		}
		vecPoint = 0;
	}


}