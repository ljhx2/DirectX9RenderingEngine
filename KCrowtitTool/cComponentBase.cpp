#include "stdafx.h"
#include "cComponentBase.h"



cComponentBase::cComponentBase()
{
	m_hWnd = NULL;
}


cComponentBase::~cComponentBase()
{
}

HWND cComponentBase::CreateCustomWindow(LPCTSTR sName, HWND hWndHost, INT startX, INT startY)
{
	m_hWnd = CreateWindowEx(NULL, WC_COMPONENTWINDOW,
		sName, WS_CHILD | WS_BORDER | WS_VISIBLE ,
		startX, startY, m_winWidth, m_winHeight,
		hWndHost, NULL, m_hInstance, NULL);
	
	if (!m_hWnd)	return NULL;

	return m_hWnd;
}

void cComponentBase::ChangeWindowSize(int x, int y, int width, int height)
{
	SetWindowPos(m_hWnd, HWND_TOP, x, y, width, height, SWP_NOZORDER);
}