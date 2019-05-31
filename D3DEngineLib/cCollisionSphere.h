#pragma once
#include "cCollisionShape.h"

namespace ENGINE
{
	class cCollisionSphere : public cCollisionShape
	{
	public:
		virtual bool CheckCollision(cCollisionShape& other);
		virtual bool CheckCollision(cCollisionShape* other) { return CheckCollision(*other); }
		
		D3DXVECTOR3 GetWorldCenter();
		float		GetWorldRadius();

		cCollisionSphere(std::string name, cColliderComponent* owner);
		virtual ~cCollisionSphere();

		bool CheckCollisionSphere(cCollisionSphere& other);
		bool CheckCollisionSphere(cCollisionSphere* other) { return CheckCollisionSphere(*other); }

	private:
		

	
	};

}