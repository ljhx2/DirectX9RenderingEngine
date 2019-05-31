#include"stdafx.h"
#include"cDebugView.h"

extern HFONT g_hFont;
HWND g_hWndDebugList = NULL;

cDebugView::cDebugView()
{
}
cDebugView::~cDebugView()
{

}
BOOL cDebugView::Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hwnd;
	WC_CUSTOMWINDOW = name;
	// Register the color box window class.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)cDebugView::WindowProc;
	wcex.cbWndExtra = sizeof(TWindowInfo*);
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = WC_CUSTOMWINDOW;
	if (0 == RegisterClassEx(&wcex))
	{
		OutputDebugString(TEXT("Failed to register the custom window class!"));
		return FALSE;
	}

	return TRUE;
}
VOID cDebugView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
	DeleteObject(g_hFont);
	g_hFont = NULL;
}
LRESULT CALLBACK cDebugView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	cDebugView* pView = DebugView;

	// Retrieve the custom window info associated with the window.
	TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);

	// We allocate an instance of our custom window info structure (TWindowInfo) and assign
	// it to the window when the WM_NCCREATE message is received. This is not guaranteed
	// to be the first message received however, so we need to check the return value of GetInfo( HWND )
	// for NULL.
	if (Msg == WM_NCCREATE)
	{
		pInfo = new TWindowInfo;
		SetWindowLongPtr(hWnd, 0, (LONG)pInfo);
	}
	else if (Msg == WM_NCDESTROY)
	{
		//assert( pInfo != NULL );
		delete pInfo;
		pInfo = NULL;
	}

	if (!pInfo)
		return DefWindowProc(hWnd, Msg, wParam, lParam);


	switch (Msg)
	{ 
	case WM_CREATE:
	{
		pView->InitList(hWnd, wParam, lParam);
	}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if (GetFocus() != g_hwndDebug)
			SetFocus(g_hwndDebug);
		break;
	case WM_SIZE:
	{
					RECT rClient;
					GetClientRect(hWnd, &rClient);
					//StringCchPrintf(pInfo->text, MAX_PATH, TEXT("Debug size: %d x %d"), rClient.right, rClient.bottom);
					MoveWindow(GetDlgItem(g_hwndDebug, 0), 0, 0, rClient.right, rClient.bottom, TRUE);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);

	}
		break;
	case WM_PAINT:
	{
			PAINTSTRUCT ps;
			HDC hdcPaint = BeginPaint(hWnd, &ps);
			if (!hdcPaint)
				break;
			RECT rClient;
			GetClientRect(hWnd, &rClient);
			FillRect(hdcPaint, &rClient, pInfo->hBrush);
			// Draw text
			{
				HGDIOBJ hOldFont = NULL;
				if (g_hFont)
					hOldFont = SelectObject(hdcPaint, g_hFont);
				int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
				COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
				size_t len = 0;
				StringCchLength(pInfo->text, MAX_PATH, &len);
				DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				SetBkMode(hdcPaint, oldMode);
				SetTextColor(hdcPaint, oldColor);
				if (g_hFont)
					SelectObject(hdcPaint, hOldFont);
			}
			EndPaint(hWnd, &ps);
	}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void cDebugView::InitList(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	g_hWndDebugList = CreateWindow(L"LISTBOX", L"", WS_CHILD | WS_VSCROLL, 0, 0, 280, 1131, hWnd, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(g_hWndDebugList, SW_SHOW);
}
