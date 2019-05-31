#include "stdafx.h"
//#include "CustomWindow.h"
//#include <strsafe.h>
//
//namespace CustomWindow
//{
//	LPCTSTR WC_CUSTOMWINDOW = TEXT("SY's Game Tool");
//	HFONT g_hFont = NULL;
//
//	//각 윈도우들의 베이스 역활을 할 커스텀 스트럭쳐 
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
//	// Disable "conversion from LONG to ptr of different size" warnings.
//# pragma warning( push )
//# pragma warning( disable: 4311 )
//# pragma warning( disable: 4312 )
//
//	TWindowInfo*	GetInfo(HWND hWnd) { return (TWindowInfo*)GetWindowLongPtr(hWnd, 0); }
//	VOID			SetInfo(HWND hWnd, TWindowInfo* pWI) { SetWindowLongPtr(hWnd, 0, (LONG)pWI); }
//
//# pragma warning( pop )
//
//	/// The message processor function of the custom window.
//	LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//	{
//		// Retrieve the custom window info associated with the window.
//		TWindowInfo* pInfo = GetInfo(hWnd);
//
//		// We allocate an instance of our custom window info structure (TWindowInfo) and assign
//		// it to the window when the WM_NCCREATE message is received. This is not guaranteed
//		// to be the first message received however, so we need to check the return value of GetInfo( HWND )
//		// for NULL.
//		if (Msg == WM_NCCREATE)
//		{
//			pInfo = new TWindowInfo;
//			SetInfo(hWnd, pInfo);
//		}
//		else if (Msg == WM_NCDESTROY)
//		{
//			//assert( pInfo != NULL );
//			delete pInfo;
//			pInfo = NULL;
//		}
//
//		if (!pInfo)
//			return DefWindowProc(hWnd, Msg, wParam, lParam);
//
//
//		switch (Msg)
//		{
//		case WM_CLOSE:
//			DestroyWindow(hWnd);
//			break;
//		case WM_LBUTTONDOWN:
//		case WM_SIZE:
//		{
//			RECT rClient;
//			GetClientRect(hWnd, &rClient);
//			StringCchPrintf(pInfo->text, MAX_PATH, TEXT("size: %d x %d"), rClient.right, rClient.bottom);
//			InvalidateRect(hWnd, NULL, FALSE);
//			UpdateWindow(hWnd);
//		}
//			break;
//		case WM_PAINT:
//		{
//			PAINTSTRUCT ps;
//			HDC hdcPaint = BeginPaint(hWnd, &ps);
//			if (!hdcPaint)
//				break;
//			RECT rClient;
//			GetClientRect(hWnd, &rClient);
//			FillRect(hdcPaint, &rClient, pInfo->hBrush);
//			// Draw text
//			{
//				HGDIOBJ hOldFont = NULL;
//				if (g_hFont)
//					hOldFont = SelectObject(hdcPaint, g_hFont);
//				int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
//				COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
//				size_t len = 0;
//				StringCchLength(pInfo->text, MAX_PATH, &len);
//				DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
//				SetBkMode(hdcPaint, oldMode);
//				SetTextColor(hdcPaint, oldColor);
//				if (g_hFont)
//					SelectObject(hdcPaint, hOldFont);
//			}
//			EndPaint(hWnd, &ps);
//		}
//			break;
//		}
//
//		return DefWindowProc(hWnd, Msg, wParam, lParam);
//	}
//
//	/// Updates the color in the custom window.
//	VOID SetColor(HWND hWnd, COLORREF color, BOOL bRepaint)
//	{
//		TWindowInfo* pInfo = GetInfo(hWnd);
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
//	BOOL Initialize(HINSTANCE hInstance)
//	{
//		// Register the color box window class.
//		WNDCLASSEX wcex;
//		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
//
//		wcex.cbSize = sizeof(WNDCLASSEX);
//		wcex.lpfnWndProc = WindowProc;
//		wcex.cbWndExtra = sizeof(TWindowInfo*);
//		wcex.hInstance = hInstance;
//		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//		wcex.lpszClassName = WC_CUSTOMWINDOW;
//		if (0 == RegisterClassEx(&wcex))
//		{
//			OutputDebugString(TEXT("Failed to register the custom window class!"));
//			return FALSE;
//		}
//
//		//// Create the font for text-rendering
//		//NONCLIENTMETRICS ncm;
//		//ncm.cbSize = sizeof(NONCLIENTMETRICS);
//		//SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
//		//g_hFont = CreateFontIndirect(&ncm.lfMessageFont);
//		return TRUE;
//	}
//
//	VOID Uninitialize(HINSTANCE hInstance)
//	{
//		UnregisterClass(WC_CUSTOMWINDOW, hInstance);
//		DeleteObject(g_hFont);
//		g_hFont = NULL;
//	}
//
//
//}; // namespace CustomWindow
//
//
//namespace CustomWindow2
//{
//	LPCTSTR WC_CUSTOMWINDOW = TEXT("Test2");
//	HFONT g_hFont = NULL;
//
//	//각 윈도우들의 베이스 역활을 할 커스텀 스트럭쳐 
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
//	// Disable "conversion from LONG to ptr of different size" warnings.
//# pragma warning( push )
//# pragma warning( disable: 4311 )
//# pragma warning( disable: 4312 )
//
//	TWindowInfo*	GetInfo(HWND hWnd) { return (TWindowInfo*)GetWindowLongPtr(hWnd, 0); }
//	VOID			SetInfo(HWND hWnd, TWindowInfo* pWI) { SetWindowLongPtr(hWnd, 0, (LONG)pWI); }
//
//# pragma warning( pop )
//
//	/// The message processor function of the custom window.
//	LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//	{
//		// Retrieve the custom window info associated with the window.
//		TWindowInfo* pInfo = GetInfo(hWnd);
//
//		// We allocate an instance of our custom window info structure (TWindowInfo) and assign
//		// it to the window when the WM_NCCREATE message is received. This is not guaranteed
//		// to be the first message received however, so we need to check the return value of GetInfo( HWND )
//		// for NULL.
//		if (Msg == WM_NCCREATE)
//		{
//			pInfo = new TWindowInfo;
//			SetInfo(hWnd, pInfo);
//		}
//		else if (Msg == WM_NCDESTROY)
//		{
//			//assert( pInfo != NULL );
//			delete pInfo;
//			pInfo = NULL;
//		}
//
//		if (!pInfo)
//			return DefWindowProc(hWnd, Msg, wParam, lParam);
//
//
//		switch (Msg)
//		{
//		case WM_CLOSE:
//			DestroyWindow(hWnd);
//			break;
//		case WM_LBUTTONDOWN:
//		case WM_SIZE:
//		{
//						RECT rClient;
//						GetClientRect(hWnd, &rClient);
//						StringCchPrintf(pInfo->text, MAX_PATH, TEXT("2222: %d x %d"), rClient.right, rClient.bottom);
//						InvalidateRect(hWnd, NULL, FALSE);
//						UpdateWindow(hWnd);
//		}
//			break;
//		case WM_PAINT:
//		{
//						 PAINTSTRUCT ps;
//						 HDC hdcPaint = BeginPaint(hWnd, &ps);
//						 if (!hdcPaint)
//							 break;
//						 RECT rClient;
//						 GetClientRect(hWnd, &rClient);
//						 FillRect(hdcPaint, &rClient, pInfo->hBrush);
//						 // Draw text
//						 {
//							 HGDIOBJ hOldFont = NULL;
//							 if (g_hFont)
//								 hOldFont = SelectObject(hdcPaint, g_hFont);
//							 int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
//							 COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
//							 size_t len = 0;
//							 StringCchLength(pInfo->text, MAX_PATH, &len);
//							 DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
//							 SetBkMode(hdcPaint, oldMode);
//							 SetTextColor(hdcPaint, oldColor);
//							 if (g_hFont)
//								 SelectObject(hdcPaint, hOldFont);
//						 }
//						 EndPaint(hWnd, &ps);
//		}
//			break;
//		}
//
//		return DefWindowProc(hWnd, Msg, wParam, lParam);
//	}
//
//	/// Updates the color in the custom window.
//	VOID SetColor(HWND hWnd, COLORREF color, BOOL bRepaint)
//	{
//		TWindowInfo* pInfo = GetInfo(hWnd);
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
//	BOOL Initialize(HINSTANCE hInstance)
//	{
//		// Register the color box window class.
//		WNDCLASSEX wcex;
//		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
//
//		wcex.cbSize = sizeof(WNDCLASSEX);
//		wcex.lpfnWndProc = WindowProc;
//		wcex.cbWndExtra = sizeof(TWindowInfo*);
//		wcex.hInstance = hInstance;
//		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//		wcex.lpszClassName = WC_CUSTOMWINDOW;
//		if (0 == RegisterClassEx(&wcex))
//		{
//			OutputDebugString(TEXT("Failed to register the custom window class!"));
//			return FALSE;
//		}
//
//		//// Create the font for text-rendering
//		//NONCLIENTMETRICS ncm;
//		//ncm.cbSize = sizeof(NONCLIENTMETRICS);
//		//SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
//		//g_hFont = CreateFontIndirect(&ncm.lfMessageFont);
//		return TRUE;
//	}
//
//	VOID Uninitialize(HINSTANCE hInstance)
//	{
//		UnregisterClass(WC_CUSTOMWINDOW, hInstance);
//		DeleteObject(g_hFont);
//		g_hFont = NULL;
//	}
//
//
//}; // namespace CustomWindow
//
//
//namespace CustomWindow233333
//{
//	LPCTSTR WC_CUSTOMWINDOW = TEXT("Test3");
//	HFONT g_hFont = NULL;
//
//	//각 윈도우들의 베이스 역활을 할 커스텀 스트럭쳐 
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
//	// Disable "conversion from LONG to ptr of different size" warnings.
//# pragma warning( push )
//# pragma warning( disable: 4311 )
//# pragma warning( disable: 4312 )
//
//	TWindowInfo*	GetInfo(HWND hWnd) { return (TWindowInfo*)GetWindowLongPtr(hWnd, 0); }
//	VOID			SetInfo(HWND hWnd, TWindowInfo* pWI) { SetWindowLongPtr(hWnd, 0, (LONG)pWI); }
//
//# pragma warning( pop )
//
//	/// The message processor function of the custom window.
//	LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//	{
//		// Retrieve the custom window info associated with the window.
//		TWindowInfo* pInfo = GetInfo(hWnd);
//
//		// We allocate an instance of our custom window info structure (TWindowInfo) and assign
//		// it to the window when the WM_NCCREATE message is received. This is not guaranteed
//		// to be the first message received however, so we need to check the return value of GetInfo( HWND )
//		// for NULL.
//		if (Msg == WM_NCCREATE)
//		{
//			pInfo = new TWindowInfo;
//			SetInfo(hWnd, pInfo);
//		}
//		else if (Msg == WM_NCDESTROY)
//		{
//			//assert( pInfo != NULL );
//			delete pInfo;
//			pInfo = NULL;
//		}
//
//		if (!pInfo)
//			return DefWindowProc(hWnd, Msg, wParam, lParam);
//
//
//		switch (Msg)
//		{
//		case WM_CLOSE:
//			DestroyWindow(hWnd);
//			break;
//		case WM_LBUTTONDOWN:
//		case WM_SIZE:
//		{
//						RECT rClient;
//						GetClientRect(hWnd, &rClient);
//						StringCchPrintf(pInfo->text, MAX_PATH, TEXT("333: %d x %d"), rClient.right, rClient.bottom);
//						InvalidateRect(hWnd, NULL, FALSE);
//						UpdateWindow(hWnd);
//		}
//			break;
//		case WM_PAINT:
//		{
//						 PAINTSTRUCT ps;
//						 HDC hdcPaint = BeginPaint(hWnd, &ps);
//						 if (!hdcPaint)
//							 break;
//						 RECT rClient;
//						 GetClientRect(hWnd, &rClient);
//						 FillRect(hdcPaint, &rClient, pInfo->hBrush);
//						 // Draw text
//						 {
//							 HGDIOBJ hOldFont = NULL;
//							 if (g_hFont)
//								 hOldFont = SelectObject(hdcPaint, g_hFont);
//							 int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
//							 COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
//							 size_t len = 0;
//							 StringCchLength(pInfo->text, MAX_PATH, &len);
//							 DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
//							 SetBkMode(hdcPaint, oldMode);
//							 SetTextColor(hdcPaint, oldColor);
//							 if (g_hFont)
//								 SelectObject(hdcPaint, hOldFont);
//						 }
//						 EndPaint(hWnd, &ps);
//		}
//			break;
//		}
//
//		return DefWindowProc(hWnd, Msg, wParam, lParam);
//	}
//
//	/// Updates the color in the custom window.
//	VOID SetColor(HWND hWnd, COLORREF color, BOOL bRepaint)
//	{
//		TWindowInfo* pInfo = GetInfo(hWnd);
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
//	BOOL Initialize(HINSTANCE hInstance)
//	{
//		// Register the color box window class.
//		WNDCLASSEX wcex;
//		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
//
//		wcex.cbSize = sizeof(WNDCLASSEX);
//		wcex.lpfnWndProc = WindowProc;
//		wcex.cbWndExtra = sizeof(TWindowInfo*);
//		wcex.hInstance = hInstance;
//		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//		wcex.lpszClassName = WC_CUSTOMWINDOW;
//		if (0 == RegisterClassEx(&wcex))
//		{
//			OutputDebugString(TEXT("Failed to register the custom window class!"));
//			return FALSE;
//		}
//
//		//// Create the font for text-rendering
//		//NONCLIENTMETRICS ncm;
//		//ncm.cbSize = sizeof(NONCLIENTMETRICS);
//		//SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
//		//g_hFont = CreateFontIndirect(&ncm.lfMessageFont);
//		return TRUE;
//	}
//
//	VOID Uninitialize(HINSTANCE hInstance)
//	{
//		UnregisterClass(WC_CUSTOMWINDOW, hInstance);
//		DeleteObject(g_hFont);
//		g_hFont = NULL;
//	}
//
//
//}; // namespace CustomWindow
//
