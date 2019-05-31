#include "Common.h"
#include "cTransform.h"


cTransform::cTransform()
{
	Reset();
}


cTransform::~cTransform()
{
}

void cTransform::Reset()
{
	m_LocalPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LocalScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_LocalRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pParnet = NULL;

	m_vecChild.clear();

	m_LocalRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_LocalUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LocalFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	UpdateMatrix();

}

void cTransform::UpdateMatrix()
{
	D3DXVECTOR3 xAxis = m_LocalRight * m_LocalScale.x;	//오른쪽
	D3DXVECTOR3 yAxis = m_LocalUp * m_LocalScale.y;		//위쪽
	D3DXVECTOR3 zAxis = m_LocalFront * m_LocalScale.z;	//정면

	//로컬행렬 초기화
	D3DXMatrixIdentity(&m_matLocal);
	memcpy(&m_matLocal._11, &xAxis, sizeof(D3DXVECTOR3));
	memcpy(&m_matLocal._21, &yAxis, sizeof(D3DXVECTOR3));
	memcpy(&m_matLocal._31, &zAxis, sizeof(D3DXVECTOR3));
	memcpy(&m_matLocal._41, &m_LocalPosition, sizeof(D3DXVECTOR3));

	if (m_pParnet == NULL)	//부모가 없으면
		m_matWorld = m_matLocal;
	else //부모가 있으면
	{
		D3DXMATRIXA16 matParent = m_pParnet->GetWorldMatrix();
		m_matWorld = m_matLocal * matParent;
	}

	//자식이 있으면
	int size = m_vecChild.size();
	for (int i = 0; i < size; i++)
	{
		m_vecChild[i]->UpdateMatrix();
	}
}

//S, R, T 셋팅
void cTransform::SetPosition(float x, float y, float z)
{
	m_LocalPosition = D3DXVECTOR3(x, y, z);

	UpdateMatrix();
}

void cTransform::SetSclae(float x, float y, float z)
{
	m_LocalScale = D3DXVECTOR3(x, y, z);

	UpdateMatrix();
}

void cTransform::SetRotation(float x, float y, float z)
{
	m_LocalRotation = D3DXVECTOR3(x, y, z);

	D3DXMATRIXA16 rot;
	D3DXMatrixRotationYawPitchRoll(&rot, y, x, z);

	D3DXVec3TransformNormal(&m_LocalRight, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &rot);
	D3DXVec3TransformNormal(&m_LocalUp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &rot);
	D3DXVec3TransformNormal(&m_LocalFront, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &rot);

	UpdateMatrix();
}

//이동, 회전, 스케일링
void cTransform::Translation_World(float dx, float dy, float dz)
{
	m_LocalPosition.x += dx;
	m_LocalPosition.y += dy;
	m_LocalPosition.z += dz;

	UpdateMatrix();
}

void cTransform::Translation_Self(float right_dx, float up_dy, float front_dz)
{
	m_LocalPosition += m_LocalRight * right_dx;
	m_LocalPosition += m_LocalUp * up_dy;
	m_LocalPosition += m_LocalFront * front_dz;

	UpdateMatrix();

}

void cTransform::Scaling(float dx, float dy, float dz)
{
	m_LocalScale.x += dx;
	m_LocalScale.y += dy;
	m_LocalScale.z += dz;

	UpdateMatrix();
}

void cTransform::Rotate_World(float dx, float dy, float dz)
{
	m_LocalRotation.x += dx;
	m_LocalRotation.y += dy;
	m_LocalRotation.z += dz;

	D3DXMATRIXA16 rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_LocalRotation.y, m_LocalRotation.x, m_LocalRotation.z);

	D3DXVec3TransformNormal(&m_LocalRight, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &rot);
	D3DXVec3TransformNormal(&m_LocalUp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &rot);
	D3DXVec3TransformNormal(&m_LocalFront, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &rot);

	UpdateMatrix();

}

//Get 함수
D3DXMATRIXA16 cTransform::GetWorldMatrix()
{
	return m_matWorld;
}

D3DXVECTOR3 cTransform::GetLocalScale()
{
	return m_LocalScale;
}

D3DXVECTOR3 cTransform::GetLocalRotation()
{
	return m_LocalRotation;
}

D3DXVECTOR3 cTransform::GetLocalPosition()
{
	return m_LocalPosition;
}

D3DXVECTOR3 cTransform::GetLocalRight()
{
	return m_LocalRight;
}

D3DXVECTOR3 cTransform::GetLocalUp()
{
	return m_LocalUp;
}

D3DXVECTOR3 cTransform::GetLocalFront()
{
	return m_LocalFront;
}

//부모가 없다면 local과 같다
D3DXVECTOR3 cTransform::GetWorldPosition()
{
	D3DXVECTOR3 worldPos;
	memcpy(&worldPos, &m_matWorld._41, sizeof(D3DXVECTOR3));
	return worldPos;
}

D3DXVECTOR3 cTransform::GetWorldRight()
{
	D3DXVECTOR3 right;
	memcpy(&right, &m_matWorld._11, sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&right, &right);
	return right;
}

D3DXVECTOR3 cTransform::GetWorldUp()
{
	D3DXVECTOR3 up;
	memcpy(&up, &m_matWorld._21, sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&up, &up);
	return up;
}

D3DXVECTOR3 cTransform::GetWorldFront()
{
	D3DXVECTOR3 front;
	memcpy(&front, &m_matWorld._31, sizeof(D3DXVECTOR3));
	D3DXVec3Normalize(&front, &front);
	return front;
}

void cTransform::Addchild(cTransform* pChild)
{
	//이미 자식이면 실행취소
	if (pChild->m_pParnet == this) return;

	m_vecChild.push_back(pChild);
	pChild->m_pParnet = this;
}

void cTransform::DetechParent()
{
	if (m_pParnet == NULL) return;

	int size = m_pParnet->m_vecChild.size();
	for (int i = 0; i < size; i++)
	{
		if (m_pParnet->m_vecChild[i] == this)
		{
			m_pParnet->m_vecChild.erase(m_pParnet->m_vecChild.begin() + i);
			break;
		}
	}

	m_pParnet = NULL;
}

cTransform* cTransform::GetParent()
{
	return m_pParnet;
}