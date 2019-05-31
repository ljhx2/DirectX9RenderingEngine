#include "stdafx.h"
#include "cWorkingCamera.h"
#include "cInput.h"

cWorkingCamera::cWorkingCamera(HWND hWnd)
{
	Reset(hWnd);

	m_aspect = m_width / (float)m_height;

	viewThisCamera = false;

	m_isLButtonDown = false;

	m_fDist = 15.0f;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	UpdateProj();

	
	ENGINE::g_pCameraManager->AddCamera(this);
}


cWorkingCamera::~cWorkingCamera()
{
	ENGINE::g_pCameraManager->EraseCamera(this);
}

void cWorkingCamera::Update(float deltaTime, D3DXMATRIXA16* matWorld)
{
	if (viewThisCamera)
	{
		
		float dist = g_WheelDeltaWPARAM / 50.0f;
		
		
		m_Position += m_Front * dist;

		if (GetFocus() == g_hwndGame)
		{
			if (cInput::GetKey(VK_LEFT))
			{
				m_Position -= m_Right;
			}
			if (cInput::GetKey(VK_RIGHT))
			{
				m_Position += m_Right;
			}
			if (cInput::GetKey(VK_UP))
			{
				m_Position += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			if (cInput::GetKey(VK_DOWN))
			{
				m_Position -= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}


			RECT rc;
			GetClientRect(g_hwndGame, &rc);

			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hwndGame, &ptMouse);

			if (PtInRect(&rc, ptMouse))
			{
				if (cInput::GetKeyDown(VK_RBUTTON))
				{
					m_isLButtonDown = true;
					GetCursorPos(&m_ptPrevMouse);
				}
				if (cInput::GetKeyUp(VK_RBUTTON))
				{
					m_isLButtonDown = false;
				}
				if (m_isLButtonDown)
				{
					POINT ptCurrMouse;
					GetCursorPos(&ptCurrMouse);

					POINT ptDelta;
					ptDelta.x = ptCurrMouse.x - m_ptPrevMouse.x;
					ptDelta.y = ptCurrMouse.y - m_ptPrevMouse.y;

					float angleX = ptDelta.x / 500.f;
					float angleY = ptDelta.y / 500.f;

					D3DXMATRIXA16 RotX, RotY;
					D3DXMatrixRotationY(&RotX, angleX);
					D3DXMatrixRotationX(&RotY, angleY);

					D3DXMATRIXA16 Rot = RotX * RotY;
					
					D3DXVec3TransformNormal(&m_Front, &m_Front, &Rot);
					D3DXVec3TransformNormal(&m_Right, &m_Right, &Rot);
					D3DXVec3TransformNormal(&m_Up, &m_Up, &Rot);
		
					m_ptPrevMouse = ptCurrMouse;
				}
			}
		}


		float dotX = -D3DXVec3Dot(&m_Right, &m_Position);
		float dotY = -D3DXVec3Dot(&m_Up, &m_Position);
		float dotZ = -D3DXVec3Dot(&m_Front, &m_Position);

		m_matView.m[0][0] = m_Right.x;		m_matView.m[0][1] = m_Up.x;		m_matView.m[0][2] = m_Front.x;		m_matView.m[0][3] = 0.0f;
		m_matView.m[1][0] = m_Right.y;		m_matView.m[1][1] = m_Up.y;		m_matView.m[1][2] = m_Front.y;		m_matView.m[1][3] = 0.0f;
		m_matView.m[2][0] = m_Right.z;		m_matView.m[2][1] = m_Up.z;		m_matView.m[2][2] = m_Front.z;		m_matView.m[2][3] = 0.0f;
		m_matView.m[3][0] = dotX;			m_matView.m[3][1] = dotY;		m_matView.m[3][2] = dotZ;			m_matView.m[3][3] = 1.0f;

		g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	}
}

void cWorkingCamera::Render()
{

}

void cWorkingCamera::UpdateProj()
{
	if (viewThisCamera)
	{
		D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovy, m_width / (float)m_height, m_z_near, m_z_far);
		g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	}
}