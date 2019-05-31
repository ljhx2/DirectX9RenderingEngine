#pragma once

#define g_pCollisionManager cCollisionManager::GetInstance()

namespace ENGINE
{
	class cColliderComponent;

	class cCollisionManager
	{
	private:
		cCollisionManager();
		~cCollisionManager();

	public:
		static cCollisionManager* GetInstance() { static cCollisionManager instance; return &instance; }

	private:
		std::vector<cColliderComponent*> m_vecStaticCollider;
		std::vector<cColliderComponent*> m_vecMovableCollider;

	public:
		void CollisionUpdate();

		void AddMovableCollider(cColliderComponent* collider);
		void AddStaticCollider(cColliderComponent* collider);
		void EraseMovableCollider(cColliderComponent* collider);
		void EraseStaticCollider(cColliderComponent* collider);
	};

}