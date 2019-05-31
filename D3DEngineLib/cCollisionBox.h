#pragma once
#include "cCollisionShape.h"


namespace ENGINE
{
	class cCollisionSphere;

	class cCollisionBox : public cCollisionShape
	{
	public:
		virtual bool CheckCollision(cCollisionShape & other);
		virtual bool CheckCollision(cCollisionShape* other) { return CheckCollision(*other); }


		cCollisionBox(std::string name, cColliderComponent* owner);
		virtual ~cCollisionBox();


		bool CheckCollisionBox(cCollisionBox& other);
		bool CheckCollisionBox(cCollisionBox* other) { return CheckCollisionBox(*other); }
		bool CheckCollisionSphere(cCollisionSphere& other);
		bool CheckCollisionSphere(cCollisionSphere* other) { return CheckCollisionSphere(*other); }

	private:
		inline float DotProduct(const float v0[3], const float v1[3])
		{
			return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
		}

		void ComputeWorld();

		

	private:

		//월드상에서의 축
		D3DXVECTOR3		m_xAxis;
		D3DXVECTOR3		m_yAxis;
		D3DXVECTOR3		m_zAxis;
		//월드상에서의 스케일
		D3DXVECTOR3		m_worldScale;
		//월드상에서의 위치
		D3DXVECTOR3		m_worldPosition;
	};

}