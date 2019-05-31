#include "Common.h"
#include "include\D3DRenderer.h"
#include "cColliderComponent.h"
#include "cSkinnedMeshComponent.h"

namespace ENGINE
{
	cColliderComponent::cColliderComponent()
	{
		eComponent_Type = eCOLLIDER_TYPE;
		m_pOwnerObject = NULL;
	}

	cColliderComponent::cColliderComponent(GameObject* pOwnerObject)
	{
		eComponent_Type = eCOLLIDER_TYPE;
		m_pOwnerObject = pOwnerObject;

		m_isRenderBoundingSphere = false;

		cSkinnedMeshComponent* skinnedComp = m_pOwnerObject->GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
		cStaticMeshComponent* staticComp = m_pOwnerObject->GetComponent<cStaticMeshComponent*>(eSTATIC_MESH_TYPE);
		cFbxComponent* fbxComp = m_pOwnerObject->GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);
		if (skinnedComp != NULL)
		{
			m_vMax = m_vOriginMax = skinnedComp->GetMaxPoint();
			m_vMin = m_vOriginMin = skinnedComp->GetMinPoint();
		}
		else if (staticComp != NULL)
		{	
			m_vMax = m_vOriginMax = staticComp->GetMaxPoint();
			m_vMin = m_vOriginMin = staticComp->GetMinPoint();
		}
		else if (fbxComp != NULL)
		{
			m_vMax = m_vOriginMax = fbxComp->GetMaxPoint();
			m_vMin = m_vOriginMin = fbxComp->GetMinPoint();
		}
		

		m_vecBoundingCenter = m_vOriginBoundingCenter = (m_vMax + m_vMin) / 2.0f;
		m_fBoundingRadius = D3DXVec3Length(&(m_vMax - m_vMin)) / 2.0f;

		m_pBoundingSphereMesh = NULL;

		D3DXCreateSphere(g_pD3DDevice, m_fBoundingRadius, 15, 15, &m_pBoundingSphereMesh, 0);


		m_colliderType = eSTATIC_COLLISION;
		g_pCollisionManager->AddStaticCollider(this);
	}

	cColliderComponent::~cColliderComponent()
	{
		SAFE_RELEASE(m_pBoundingSphereMesh);

		for each(auto a in m_mapCollision)
		{
			SAFE_DELETE(a.second);
		}

		if (m_colliderType == eMOVABLE_COLLISION)
		{
			g_pCollisionManager->EraseMovableCollider(this);
		}
		else if (m_colliderType == eSTATIC_COLLISION)
		{
			g_pCollisionManager->EraseStaticCollider(this);
		}
	}

	void cColliderComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		m_matWorld = (*matWorld);

		D3DXVec3TransformCoord(&m_vMax, &m_vOriginMax, matWorld);
		D3DXVec3TransformCoord(&m_vMin, &m_vOriginMin, matWorld);
		
		m_vecBoundingCenter = (m_vMax + m_vMin) / 2.0f;
		m_fBoundingRadius = D3DXVec3Length(&(m_vMax - m_vMin)) / 2.0f;
	}

	void cColliderComponent::Render()
	{
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->LightEnable(0, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (m_isRenderBoundingSphere)
		{

			D3DXMATRIXA16 offset, boundingWorld;
			D3DXMatrixTranslation(&offset, m_vOriginBoundingCenter.x, m_vOriginBoundingCenter.y, m_vOriginBoundingCenter.z);
			boundingWorld = offset * m_matWorld;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &boundingWorld);
			m_pBoundingSphereMesh->DrawSubset(0);
		}

		for (m_mapIter = m_mapCollision.begin(); m_mapIter != m_mapCollision.end(); ++m_mapIter)
		{
			m_mapIter->second->Render();
		}
		
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		g_pD3DDevice->LightEnable(0, TRUE);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	
	bool cColliderComponent::AddCollisionSphere(std::string name)
	{
		cCollisionSphere* sphere = new cCollisionSphere(name, this);
		
		D3DXMATRIXA16* pParent = m_pOwnerObject->GetPWorldMatrix();
		sphere->SetParentMatrix(pParent);

		std::pair<std::map<std::string, cCollisionShape*>::iterator, bool> pr;

		pr = m_mapCollision.insert(std::make_pair(name, sphere));

		if (pr.second == false)
		{
			SAFE_DELETE(sphere);
			assert(!"구추가실패");
		}

		return pr.second;
	}

	cCollisionSphere* cColliderComponent::GetClassCollisionSphere(std::string name)
	{
		cCollisionShape* sphere = m_mapCollision[name];
		
		if (sphere == NULL) assert(!"CollisionSphere가없음");

		if (sphere->GetShapeType() == eSPHERE_SHAPE)
			return (cCollisionSphere*)sphere;
		else
			assert(!"타입을 잘못선택했습니다");
	}

	COLLISION_SPHERE cColliderComponent::GetCollisionSphere(std::string name)
	{
		cCollisionSphere* sphere = GetClassCollisionSphere(name);
		
		D3DXVECTOR3 center = sphere->GetWorldCenter();
		float radius = sphere->GetWorldRadius();

		return COLLISION_SPHERE(center, radius);
	}

	bool cColliderComponent::AddCollisionBox(std::string name)
	{
		cCollisionBox* box = new cCollisionBox(name, this);

		D3DXMATRIXA16* pParent = m_pOwnerObject->GetPWorldMatrix();
		box->SetParentMatrix(pParent);

		std::pair<std::map<std::string, cCollisionShape*>::iterator, bool> pr;

		pr = m_mapCollision.insert(std::make_pair(name, box));

		if (pr.second == false)
		{
			SAFE_DELETE(box);
			assert(!"박스추가실패");
		}

		return pr.second;
	}

	void cColliderComponent::DelCollision(std::string name)
	{
		m_mapIter = m_mapCollision.find(name);

		if (m_mapIter != m_mapCollision.end())
		{
			SAFE_DELETE(m_mapIter->second);
			m_mapCollision.erase(m_mapIter);
		}
	}

	cCollisionBox* cColliderComponent::GetCollisionBox(std::string name)
	{
		cCollisionShape* box = m_mapCollision[name];
		if (box == NULL) assert(!"CollisionBox가없음");

		if (box->GetShapeType() == eBOX_SHAPE)
			return (cCollisionBox*)box;
		else
			assert(!"타입을 잘못선택했습니다");
	}

	void cColliderComponent::ColliderCollision(cColliderComponent& collider)
	{
		for (m_mapIter = m_mapCollision.begin(); m_mapIter != m_mapCollision.end(); ++m_mapIter)
		{
			for (collider.m_mapIter = collider.m_mapCollision.begin(); collider.m_mapIter != collider.m_mapCollision.end(); ++collider.m_mapIter)
			{
				if (m_mapIter->second->CheckCollision(collider.m_mapIter->second))
				{
					m_mapIter->second->Clashed(collider.m_mapIter->second);
					collider.m_mapIter->second->Clashed(m_mapIter->second);
				}
			}
		}
	}

	void cColliderComponent::ClearCollisionList()
	{
		for (m_mapIter = m_mapCollision.begin(); m_mapIter != m_mapCollision.end(); ++m_mapIter)
		{
			m_mapIter->second->ClearClashedVec();
		}
	}

	void cColliderComponent::SetCollisionTYPE(ECOLLISION_TYPE type)
	{
		if (m_colliderType == type) return;

		m_colliderType = type;
		if (m_colliderType == eMOVABLE_COLLISION)
		{
			g_pCollisionManager->EraseStaticCollider(this);
			g_pCollisionManager->AddMovableCollider(this);
		}
		else if (m_colliderType == eSTATIC_COLLISION)
		{
			g_pCollisionManager->EraseMovableCollider(this);
			g_pCollisionManager->AddStaticCollider(this);
		}
	}

	cCollisionShape* cColliderComponent::GetCollisionShape(std::string name)
	{
		cCollisionShape* shape = m_mapCollision[name];
		if (shape == NULL) assert(!"CollisionShape가없음");

		return shape;
		
	}
}