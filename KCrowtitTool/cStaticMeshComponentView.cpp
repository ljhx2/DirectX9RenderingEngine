#include "stdafx.h"
#include "cStaticMeshComponentView.h"


cStaticMeshComponentView::cStaticMeshComponentView()
{
}


cStaticMeshComponentView::~cStaticMeshComponentView()
{
}

BOOL cStaticMeshComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	WC_COMPONENTWINDOW = name;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)cStaticMeshComponentView::WindowProc;;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_winWidth = 300;
	m_winHeight = 100;

	return TRUE;
}
VOID cStaticMeshComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
}

LRESULT CALLBACK cStaticMeshComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		/*if (GetFocus() != g_hwndInspector)
		SetFocus(g_hwndInspector);*/
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
void cStaticMeshComponentView::SetTransformValue(std::wstring name, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{

}

