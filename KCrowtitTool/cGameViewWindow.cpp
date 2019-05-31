//#include "stdafx.h"
//#include "cGameViewWindow.h"
//
//cGameViewWindow::cGameViewWindow()
//{
//	WC_CUSTOMWINDOW = TEXT("Game View");
//}
//cGameViewWindow::~cGameViewWindow()
//{
//
//}
//
//LRESULT CALLBACK cGameViewWindow::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//
//	TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);
//
//	if (Msg == WM_NCCREATE)
//	{
//		pInfo = new TWindowInfo;
//		SetWindowLongPtr(hWnd, 0, (LONG)pInfo);
//	}
//	switch (Msg)
//	{
//	case WM_CLOSE:
//		DestroyWindow(hWnd);
//		break;
//	case WM_LBUTTONDOWN:
//	case WM_SIZE:
//	{
//		RECT rClient;
//		GetClientRect(hWnd, &rClient);
//		StringCchPrintf(pInfo->text, MAX_PATH, TEXT("size: %d x %d"), rClient.right, rClient.bottom);
//		InvalidateRect(hWnd, NULL, FALSE);
//		UpdateWindow(hWnd);
//	}
//		break;
//	case WM_PAINT:
//		PAINTSTRUCT ps;
//		HDC hdcPaint = BeginPaint(hWnd, &ps);
//		if (!hdcPaint)
//			break;
//		RECT rClient;
//		GetClientRect(hWnd, &rClient);
//		FillRect(hdcPaint, &rClient, pInfo->hBrush);
//		{
//			HGDIOBJ hOldFont = NULL;
//			int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
//			COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
//			size_t len = 0;
//			StringCchLength(pInfo->text, MAX_PATH, &len);
//			DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
//			SetBkMode(hdcPaint, oldMode);
//			SetTextColor(hdcPaint, oldColor);
//
//		}
//		EndPaint(hWnd, &ps);
//
//		break;
//	}
//	return DefWindowProc(hWnd, Msg, wParam, lParam);
//}
//HWND cGameViewWindow::CreateCustomWindow(LPCTSTR sName, HWND hHost, INT width, INT height, int resizability)
//{
//	HWND hWnd = CreateWindowEx(NULL,WC_CUSTOMWINDOW,
//		sName, WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME,
//		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
//		m_hWnd, NULL, m_hInstance, NULL);
//	if (!hWnd)
//		return NULL;
//
//	MakeDockable(m_hWnd, hHost, resizability);
//}
//HDHNODE cGameViewWindow::DockHost_D(HWND hDockHost,
//	HWND hWnd,
//	DH_DockLocation dockLocation,
//	HDHNODE hBaseNode ,
//	DWORD dwNodeStyle,
//	INT requestedWidth , INT requestedHeight,
//	INT minWidth, INT minHeight ,
//	INT resizability,
//	VOID* userData)
//{
//	return DockHost_Dock(hDockHost, m_hWnd, dockLocation, hBaseNode);
//}
//BOOL cGameViewWindow::Initialize(HWND hwnd, HINSTANCE hInstance)
//{
//	m_hWnd = hwnd;
//	m_hInstance = hInstance;
//
//	WNDCLASSEX wcex;
//	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.lpfnWndProc = (WNDPROC)cGameViewWindow::WindowProc;
//	wcex.cbWndExtra =  0;		//À½..
//	wcex.hInstance = hInstance;
//	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wcex.lpszClassName = WC_CUSTOMWINDOW;
//	if (0 == RegisterClassEx(&wcex))
//	{
//		OutputDebugString(TEXT("Failed to register the custom window class!"));
//		return FALSE;
//	}
//	return TRUE;
//}
//VOID cGameViewWindow::Uninitialize(HINSTANCE hInstance)
//{
//	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
//}