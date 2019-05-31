#pragma once
#include "DockHost.h"
#include <strsafe.h>
struct TWindowInfo
{
	COLORREF	color;
	HBRUSH		hBrush;
	TCHAR		text[MAX_PATH];

	TWindowInfo()
	{
		color = RGB(255, 255, 255);
		hBrush = CreateSolidBrush(color);
		text[0] = 0;
	}

	~TWindowInfo()
	{
		DeleteObject(hBrush);
	}
};
BOOL MakeDockable(HWND hWnd, HWND hDockHost, int resizability);

class cBaseCustomWindow
{
public:
	LPCTSTR WC_CUSTOMWINDOW;

	HWND		m_hWnd = NULL;
	HINSTANCE	m_hInstance = NULL;

public:
	cBaseCustomWindow();
	~cBaseCustomWindow();

	TWindowInfo*	GetInfo(HWND hWnd) { return (TWindowInfo*)GetWindowLongPtr(hWnd, 0); }
	VOID			SetInfo(HWND hWnd, TWindowInfo* pWI) { SetWindowLongPtr(hWnd, 0, (LONG)pWI); }

	VOID SetColor(HWND hWnd, COLORREF color, BOOL bRepaint = TRUE);
	virtual HWND CreateCustomWindow(LPCTSTR sName, HWND hWndHost, INT width, INT height, int resizability);
	virtual	BOOL Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance) = 0;
	virtual VOID Uninitialize(HINSTANCE hInstance) = 0;
//	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

};