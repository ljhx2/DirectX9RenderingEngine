//#pragma once
//#include "DockHost.h"
//#include <strsafe.h>
//BOOL				MakeDockable(HWND hWnd, HWND hDockHost, int resizability);
//
//class cCustomWindow
//{
//protected:
//	LPCTSTR WC_CUSTOMWINDOW = NULL;
//	HWND m_hWnd = NULL;
//	HINSTANCE m_hInstance = NULL;
//	HDHNODE m_hNode = NULL;
//	HDHNODE m_hHostNode = NULL;
//
//	virtual BOOL Initialize(HWND hwnd, HINSTANCE hInstance) = 0;
//	virtual VOID Uninitialize(HINSTANCE hInstance) = 0;
//	virtual HWND CreateCustomWindow(LPCTSTR sName, HWND hHost, INT width, INT height, int resizability) = 0;
//	virtual HDHNODE DockHost_D(HWND hDockHost,
//		HWND hWnd,
//		DH_DockLocation dockLocation,
//		HDHNODE hBaseNode = NULL,
//		DWORD dwNodeStyle = 0,
//		INT requestedWidth = -1, INT requestedHeight = -1,
//		INT minWidth = -1, INT minHeight = -1,
//		INT resizability = DH_DEFAULT_RESIZABILITY,
//		VOID* userData = NULL) = 0;
//	struct TWindowInfo
//	{
//		COLORREF	color;
//		HBRUSH		hBrush;
//		TCHAR		text[MAX_PATH];
//
//		TWindowInfo()
//		{
//			color = RGB(255, 255, 255);
//			hBrush = CreateSolidBrush(color);
//			text[0] = 0;
//		}
//
//		~TWindowInfo()
//		{
//			DeleteObject(hBrush);
//		}
//	};
//
//};