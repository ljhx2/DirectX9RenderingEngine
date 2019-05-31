#include"stdafx.h"
#include"cBaseCustomWindow.h"

cBaseCustomWindow::cBaseCustomWindow()
{
}
cBaseCustomWindow::~cBaseCustomWindow()
{

}

VOID cBaseCustomWindow::SetColor(HWND hWnd, COLORREF color, BOOL bRepaint)
{
	TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);
	pInfo->color = color;
	DeleteObject(pInfo->hBrush);
	pInfo->hBrush = CreateSolidBrush(color);
	if (bRepaint)
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
}
HWND cBaseCustomWindow::CreateCustomWindow(LPCTSTR sName, HWND hWndHost, INT width, INT height, int resizability)
{
	m_hWnd = CreateWindowEx(NULL, WC_CUSTOMWINDOW,
		sName, WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		hWndHost, NULL, m_hInstance, NULL);
	if (!m_hWnd)
		return NULL;

	MakeDockable(m_hWnd, hWndHost, resizability);
	return m_hWnd;
}
//BOOL cBaseCustomWindow::Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance)
//{
//	m_hInstance = hInstance;
//	m_hWnd = hwnd;
//	WC_CUSTOMWINDOW = name;
//	// Register the color box window class.
//	WNDCLASSEX wcex;
//	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.lpfnWndProc = WindowProc;
//	wcex.cbWndExtra = sizeof(TWindowInfo*);
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
//VOID cBaseCustomWindow::Uninitialize(HINSTANCE hInstance)
//{
//	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
//	DeleteObject(g_hFont);
//	g_hFont = NULL;
//}
//LRESULT CALLBACK cBaseCustomWindow::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//	// Retrieve the custom window info associated with the window.
//	TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);
//
//	// We allocate an instance of our custom window info structure (TWindowInfo) and assign
//	// it to the window when the WM_NCCREATE message is received. This is not guaranteed
//	// to be the first message received however, so we need to check the return value of GetInfo( HWND )
//	// for NULL.
//	if (Msg == WM_NCCREATE)
//	{
//		pInfo = new TWindowInfo;
//		SetWindowLongPtr(hWnd, 0, (LONG)pInfo);
//	}
//	else if (Msg == WM_NCDESTROY)
//	{
//		//assert( pInfo != NULL );
//		delete pInfo;
//		pInfo = NULL;
//	}
//
//	if (!pInfo)
//		return DefWindowProc(hWnd, Msg, wParam, lParam);
//
//
//	switch (Msg)
//	{
//	case WM_CLOSE:
//		DestroyWindow(hWnd);
//		break;
//	case WM_LBUTTONDOWN:
//	case WM_SIZE:
//	{
//					RECT rClient;
//					GetClientRect(hWnd, &rClient);
//					StringCchPrintf(pInfo->text, MAX_PATH, TEXT("size: %d x %d"), rClient.right, rClient.bottom);
//					InvalidateRect(hWnd, NULL, FALSE);
//					UpdateWindow(hWnd);
//	}
//		break;
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdcPaint = BeginPaint(hWnd, &ps);
//		if (!hdcPaint)
//			break;
//		RECT rClient;
//		GetClientRect(hWnd, &rClient);
//		FillRect(hdcPaint, &rClient, pInfo->hBrush);
//		// Draw text
//		{
//			HGDIOBJ hOldFont = NULL;
//			if (g_hFont)
//				hOldFont = SelectObject(hdcPaint, g_hFont);
//			int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
//			COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
//			size_t len = 0;
//			StringCchLength(pInfo->text, MAX_PATH, &len);
//			DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
//			SetBkMode(hdcPaint, oldMode);
//			SetTextColor(hdcPaint, oldColor);
//			if (g_hFont)
//				SelectObject(hdcPaint, hOldFont);
//		}
//		EndPaint(hWnd, &ps);
//	}
//		break;
//	}
//
//	return DefWindowProc(hWnd, Msg, wParam, lParam);
//}