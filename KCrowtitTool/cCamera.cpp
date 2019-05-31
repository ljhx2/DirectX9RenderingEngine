#include "stdafx.h"
#include "cCamera.h"
#include "cInput.h"

cCamera::cCamera()
	: m_pTarget(NULL)
	, m_isLButtonDown(false)
	, m_fDist(15.0f)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_vOffset(0, 0, 0)
	, viewThisCamera(true)
{
}


cCamera::~cCamera()
{
}

void cCamera::Setup(D3DXVECTOR3* pTarget)
{
	m_pTarget = pTarget;

	//뷰 매트릭스 적용
	m_vEye = D3DXVECTOR3(50, 50, -m_fDist);
	m_vLook = D3DXVECTOR3(0, 0, 0);
	m_vUp = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vLook, &m_vUp);	//왼손좌표계 기준으로 뷰행렬 만듬
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mView);

	RECT rc;
	GetClientRect(g_hwndGame, &rc);

	m_fovy = D3DX_PI / 4.0f;
	m_aspect = rc.right / (float)rc.bottom;
	m_z_near = 1.0f;
	m_z_far = 1000.0f;
	
	UpdateProj();

	//투영 매트릭스 적용
	//D3DXMatrixPerspectiveFovLH(&m_mProj, D3DX_PI / 4.0f/*시야각*/, rc.right / (float)rc.bottom/*종횡비*/, 1.0f/*near*/, 10000.0f/*far*/);
	//g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);

}

void cCamera::Update()
{
	if (viewThisCamera == false) return;

	m_fDist -= g_WheelDeltaWPARAM / 50.f;
	if (m_fDist < 1.0f)
	{
		m_fDist = 1.0f;
	}

	D3DXVECTOR3 vecRight;
	D3DXVECTOR3 vecFromCameraToTarget = m_vEye - m_vLook;
	D3DXVec3Cross(&vecRight, &m_vUp, &vecFromCameraToTarget);
	D3DXVec3Normalize(&vecRight, &vecRight);
	if (cInput::GetKey(VK_LEFT))
	{
		if (GetFocus() == g_hwndGame)
			m_vOffset -= vecRight;
	}
	if (cInput::GetKey(VK_RIGHT))
	{
		if (GetFocus() == g_hwndGame)
			m_vOffset += vecRight;
	}
	if (cInput::GetKey(VK_UP))
	{

	}
	if (cInput::GetKey(VK_DOWN))
	{

	}


	m_vLook = D3DXVECTOR3(0, 0, 0);

	m_vEye = D3DXVECTOR3(0, 0, -m_fDist);
	m_vEye += m_vOffset;
	m_vLook += m_vOffset;

	RECT rc;
	GetClientRect(g_hwndGame, &rc);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hwndGame, &ptMouse);
	if (PtInRect(&rc, ptMouse))
	{
		if (cInput::GetKeyDown(VK_LBUTTON))
		{
			m_isLButtonDown = true;
			GetCursorPos(&m_ptPrevMouse);
		}
	}
	if (cInput::GetKeyUp(VK_LBUTTON))
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

		m_fAngleX += ptDelta.y / 100.f;
		m_fAngleY += ptDelta.x / 100.f;

		if (m_fAngleX >= D3DX_PI / 2.0f)
		{
			m_fAngleX = D3DX_PI / 2.0f - 0.0001f;
		}

		if (m_fAngleX <= -D3DX_PI / 2.0f)
		{
			m_fAngleX = -D3DX_PI / 2.0f + 0.0001f;
		}
		m_ptPrevMouse = ptCurrMouse;
	}

	D3DXMATRIXA16 mR, mRX, mRY;
	D3DXMatrixRotationX(&mRX, m_fAngleX);
	D3DXMatrixRotationY(&mRY, m_fAngleY);
	mR = mRX * mRY;

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &mR);

	
	

	if (m_pTarget)
	{
		m_vEye += (*m_pTarget);
		m_vLook = (*m_pTarget);
	}
	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vLook, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mView);
}

void cCamera::UpdateProj()
{
	if (viewThisCamera)
	{
		D3DXMatrixPerspectiveFovLH(&m_mProj, m_fovy, m_aspect, m_z_near, m_z_far);
		g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);
	}
}