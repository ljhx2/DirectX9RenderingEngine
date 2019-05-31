#pragma once
#include "IComponent.h"
#include "cCollisionSphere.h"
#include "cCollisionBox.h"

namespace ENGINE
{
	enum ECOLLISION_TYPE
	{
		eMOVABLE_COLLISION = 0,
		eSTATIC_COLLISION
	};

	struct COLLISION_SPHERE
	{
		D3DXVECTOR3 center;
		float		radius;
		COLLISION_SPHERE(){ center = D3DXVECTOR3(0, 0, 0); radius = 0.0f; };
		COLLISION_SPHERE(D3DXVECTOR3 _center, float _radius) { center = _center; radius = _radius; }
	};

	class GameObject;

	class cColliderComponent : public IComponent
	{
	public:
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();

		COLLISION_SPHERE GetBoundingSphere() { return COLLISION_SPHERE(m_vecBoundingCenter, m_fBoundingRadius); }

		void SetRenderCollisionSphere(bool b) { m_isRenderBoundingSphere = b; }
		bool GetRenderCOllisionSphere() { return m_isRenderBoundingSphere; }
		

		COLLISION_SPHERE GetCollisionSphere(std::string name);


		//collisionSphere
		bool AddCollisionSphere(std::string name);
		cCollisionSphere* GetClassCollisionSphere(std::string name);

		//collisionBox
		bool AddCollisionBox(std::string name);
		cCollisionBox* GetCollisionBox(std::string name);

		void DelCollision(std::string name);

		cCollisionShape* GetCollisionShape(std::string name);

		cColliderComponent();
		cColliderComponent(GameObject* pOwnerObject);
		virtual ~cColliderComponent();

		///------------------------------------------
		void ColliderCollision(cColliderComponent& collider);
		void ColliderCollision(cColliderComponent* collider) { ColliderCollision(*collider); }

		void ClearCollisionList();

		ECOLLISION_TYPE GetCollisionTYPE() { return m_colliderType; }
		void SetCollisionTYPE(ECOLLISION_TYPE type);
	

		//----------------------------------------------

	public:
		//Tool을 위해 제공되는 함수
		std::map<std::string, cCollisionShape*>* GetMapCollision() { return &m_mapCollision; }

	private:
		bool				m_isRenderBoundingSphere;

		//Origin변수들은 콜리젼스피어를 그리는 Mesh를 만들때만 사용하고
		//실제 중점과 반지름은 m_vecBoundingCenter와 m_fBoundingRadius이다
		D3DXVECTOR3			m_vMax, m_vMin;
		D3DXVECTOR3			m_vOriginMax, m_vOriginMin;
		
		D3DXVECTOR3			m_vecBoundingCenter, m_vOriginBoundingCenter;
		float				m_fBoundingRadius;
		ID3DXMesh*			m_pBoundingSphereMesh;
		
		
		D3DXMATRIXA16		m_matWorld;


		std::map<std::string, cCollisionShape*> m_mapCollision;
		std::map<std::string, cCollisionShape*>::iterator m_mapIter;


		ECOLLISION_TYPE m_colliderType;
	};

}
