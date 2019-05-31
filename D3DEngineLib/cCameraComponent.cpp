#include "Common.h"
#include "cCameraComponent.h"
#include "include\D3DRenderer.h"

namespace ENGINE
{

	cCameraComponent::cCameraComponent()
	{
		eComponent_Type = eCAMERA_TYPE;
		m_pOwnerObject = NULL;

		m_WheelDeltaWPARAM = NULL;
	}

	cCameraComponent::cCameraComponent(GameObject* pOwner, HWND hWnd)
	{
		eComponent_Type = eCAMERA_TYPE;
		m_pOwnerObject = pOwner;

		D3DXMatrixIdentity(&m_matView);
		D3DXMatrixIdentity(&m_matProj);

		Reset(hWnd);
	
		viewThisCamera = false;

		g_pCameraManager->AddCamera(this);

		m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		m_WheelDeltaWPARAM = NULL;
	}


	cCameraComponent::~cCameraComponent()
	{
		g_pCameraManager->EraseCamera(this);
	}

	void cCameraComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		//카메라 뷰행렬 만들기
		// XAxis.x				YAxis.x				ZAxis.x				0.0f
		// XAxis.y				YAxis.y				ZAxis.y				0.0f
		// XAxis.z				YAxis.z				ZAxis.z				0.0f
		// -( XAxis Dot pos )	-( YAxis Dot Pos )	-( ZAxis Dot Pos )	1.0f

		if (viewThisCamera)
		{

			D3DXMATRIXA16 world = *matWorld;
			m_Front = D3DXVECTOR3(world._31, world._32, world._33);
			m_Up = D3DXVECTOR3(world._21, world._22, world._23);
			m_Right = D3DXVECTOR3(world._11, world._12, world._13);
			D3DXVec3Normalize(&m_Front, &m_Front);
			D3DXVec3Normalize(&m_Up, &m_Up);
			D3DXVec3Normalize(&m_Right, &m_Right);
			m_Position = D3DXVECTOR3(world._41, world._42, world._43);

			if (m_WheelDeltaWPARAM != NULL)
			{
				float dist = (*m_WheelDeltaWPARAM )/ 50.0f;
				m_Position += m_Front * dist;
			}


			float dotX = -D3DXVec3Dot(&m_Right, &m_Position);
			float dotY = -D3DXVec3Dot(&m_Up, &m_Position);
			float dotZ = -D3DXVec3Dot(&m_Front, &m_Position);

			GameObject* parent = m_pOwnerObject->GetParent();

			if (parent == GetSceneRoot() || parent == NULL)
			{

				m_matView.m[0][0] = m_Right.x;		m_matView.m[0][1] = m_Up.x;		m_matView.m[0][2] = m_Front.x;		m_matView.m[0][3] = 0.0f;
				m_matView.m[1][0] = m_Right.y;		m_matView.m[1][1] = m_Up.y;		m_matView.m[1][2] = m_Front.y;		m_matView.m[1][3] = 0.0f;
				m_matView.m[2][0] = m_Right.z;		m_matView.m[2][1] = m_Up.z;		m_matView.m[2][2] = m_Front.z;		m_matView.m[2][3] = 0.0f;
				m_matView.m[3][0] = dotX;			m_matView.m[3][1] = dotY;		m_matView.m[3][2] = dotZ;			m_matView.m[3][3] = 1.0f;

				g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);

			}
			else
			{

				D3DXMATRIXA16 parentWorld = parent->GetWorldMatrix();
				D3DXVECTOR3 vLook = D3DXVECTOR3(parentWorld._41, parentWorld._42, parentWorld._43);	//바라보는점
				D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);	//월드좌표의 업(위쪽)벡터
				D3DXMatrixLookAtLH(&m_matView, &m_Position, &vLook, &vUp);	//왼손좌표계 기준으로 뷰 행렬을 만듬

				g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);	//뷰행렬 적용

			}

		}
	}

	void cCameraComponent::Render()
	{

	}

	void cCameraComponent::Reset(HWND hWnd)
	{
		m_fovy = D3DX_PI / 4.0f;
		
		RECT rc;
		GetClientRect(hWnd, &rc);
		m_width = rc.right;
		m_height = rc.bottom;

		m_z_near = 1.0f;

		m_z_far = 10000.0f;

		UpdateProj();

	}

	void cCameraComponent::UpdateProj()
	{
		if (viewThisCamera)
		{
			D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovy, m_width / (float)m_height, m_z_near, m_z_far);
			g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

		}
	}

	POINT cCameraComponent::WorldToScreen(float x, float y, float z)
	{
		D3DXMATRIXA16 mWorld;
		D3DXMatrixTranslation(&mWorld, x, y, z);

		D3DXMATRIXA16 mWVP = mWorld * m_matView * m_matProj;

		D3DXVECTOR4 worldToScreen(0.0f, 0.0f, 0.0f, 1.0f);
		D3DXVec4Transform(&worldToScreen, &worldToScreen, &mWVP);

		float fWorldToScreenX = worldToScreen.x / worldToScreen.w;
		float fWorldToScreenY = worldToScreen.y / worldToScreen.w;
		float fWorldToScreenZ = worldToScreen.z / worldToScreen.w;

		float screenNormalX = (fWorldToScreenX + 1.0f) * 0.5f;
		float screenNormalY = (fWorldToScreenY + 1.0f)  * 0.5f;
		screenNormalY = 1.0f - screenNormalY;

		
		POINT screen;

		screen.x = static_cast<int>(screenNormalX * m_width);
		screen.y = static_cast<int>(screenNormalY * m_height);

		return screen;
	}
}