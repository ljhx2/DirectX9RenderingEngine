//#pragma once
//#include "cCustomWindow.h"
//
//class cGameViewWindow : public cCustomWindow
//{
//public:
//
//public :
//	cGameViewWindow();
//	~cGameViewWindow();
//	virtual BOOL Initialize(HWND hwnd, HINSTANCE hInstance);
//	virtual VOID Uninitialize(HINSTANCE hInstance);
//	HWND CreateCustomWindow(LPCTSTR sName, HWND hHost, INT width, INT height, int resizability);
//private:
//	static LRESULT CALLBACK cGameViewWindow::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//
//
//public :
//	 VOID SetColor(HWND hWnd, COLORREF color, BOOL bRepaint)
//	{
//		TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);
//		pInfo->color = color;
//		DeleteObject(pInfo->hBrush);
//		pInfo->hBrush = CreateSolidBrush(color);
//		if (bRepaint)
//		{
//			InvalidateRect(hWnd, NULL, TRUE);
//			UpdateWindow(hWnd);
//		}
//	}
//
//	 virtual HDHNODE DockHost_D(HWND hDockHost,
//		 HWND hWnd,
//		 DH_DockLocation dockLocation,
//		 HDHNODE hBaseNode = NULL,
//		 DWORD dwNodeStyle = 0,
//		 INT requestedWidth = -1, INT requestedHeight = -1,
//		 INT minWidth = -1, INT minHeight = -1,
//		 INT resizability = DH_DEFAULT_RESIZABILITY,
//		 VOID* userData = NULL);
//
//};