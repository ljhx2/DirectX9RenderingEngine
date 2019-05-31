#include "Common.h"
#include "cCollisionManager.h"
#include "cColliderComponent.h"
#include "include\D3DRenderer.h"

namespace ENGINE
{

	cCollisionManager::cCollisionManager()
	{
		m_vecStaticCollider.clear();
		m_vecMovableCollider.clear();
	}


	cCollisionManager::~cCollisionManager()
	{
	}

	void cCollisionManager::CollisionUpdate()
	{
		int vecSize = m_vecMovableCollider.size();
		int staticVecSize = m_vecStaticCollider.size();

		//�� Collision�� �ִ� �浹����Ʈ �ʱ�ȭ
		for (int i = 0; i < vecSize; i++)
		{
			m_vecMovableCollider[i]->ClearCollisionList();
		}
		for (int i = 0; i < staticVecSize; i++)
		{
			m_vecStaticCollider[i]->ClearCollisionList();
		}

		//�浹�˻�
		for (int i = 0; i < vecSize; i++)
		{
			if (m_vecMovableCollider[i]->GetOwnerObject()->GetIsEnable() == false)
				continue;

			for (int j = i + 1; j < vecSize; j++)
			{
				m_vecMovableCollider[i]->ColliderCollision(m_vecMovableCollider[j]);
			}

			for (int k = 0; k < staticVecSize; k++)
			{
				m_vecMovableCollider[i]->ColliderCollision(m_vecStaticCollider[k]);
			}
		}
	}

	void cCollisionManager::AddMovableCollider(cColliderComponent* collider)
	{
		int vecSize = m_vecMovableCollider.size();
		for (int i = 0; i < vecSize; i++)
		{
			if (m_vecMovableCollider[i] == collider) //�̹� ���Ϳ� ������ ����
			{
				return;
			}
		}

		//������ ���Ϳ� �߰�
		m_vecMovableCollider.push_back(collider);
	}

	void cCollisionManager::AddStaticCollider(cColliderComponent* collider)
	{
		int vecSize = m_vecStaticCollider.size();
		for (int i = 0; i < vecSize; i++)
		{
			if (m_vecStaticCollider[i] == collider) //�̹� ���Ϳ� ������ ����
			{
				return;
			}
		}

		//������ ���Ϳ� �߰�
		m_vecStaticCollider.push_back(collider);
	}

	void cCollisionManager::EraseMovableCollider(cColliderComponent* collider)
	{
		int vecSize = m_vecMovableCollider.size();
		for (int i = 0; i < vecSize; i++)
		{
			if (m_vecMovableCollider[i] == collider)
			{
				m_vecMovableCollider.erase(m_vecMovableCollider.begin() + i);
				break;
			}
		}
	}

	void cCollisionManager::EraseStaticCollider(cColliderComponent* collider)
	{
		int vecSize = m_vecStaticCollider.size();
		for (int i = 0; i < vecSize; i++)
		{
			if (m_vecStaticCollider[i] == collider)
			{
				m_vecStaticCollider.erase(m_vecStaticCollider.begin() + i);
				break;
			}
		}
	}

}