#include "stdafx.h"
#include "DockHost.h"
#include "DockArrow.h"

HWND		g_hDockArrow = NULL;
HWND		g_hDraggingWnd = NULL;
#define GuiAssert(exp) ((void)0)

struct WindowSubclass
{
	WindowSubclass(HWND _hWnd);
	virtual ~WindowSubclass();

	/// Override this method to define custom behavior. Don't forget to call WindowSubclass::MsgProc()
	/// at the end of your MsgProc().
	virtual LRESULT MsgProc(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return CallWindowProc(originalProc, hWnd, Msg, wParam, lParam);
	}

	HWND hWnd;
protected:
	WNDPROC originalProc;
	static LRESULT CALLBACK sMsgProc(HWND, UINT, WPARAM, LPARAM);
};

WindowSubclass::WindowSubclass(HWND _hWnd)
{
	hWnd = _hWnd;
	if (GetWindowLongPtr(hWnd, GWLP_USERDATA))
	{
		GuiAssert("WindowSubclass(): user data field must not be used" == 0);
		return;
	}
	originalProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)sMsgProc);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
}



WindowSubclass::~WindowSubclass()
{
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)originalProc);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
}


LRESULT CALLBACK WindowSubclass::sMsgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	WindowSubclass* p = (WindowSubclass*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!p)
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	return p->MsgProc(Msg, wParam, lParam);
}



struct DockableWindowSubclass : public WindowSubclass
{
	DockableWindowSubclass(HWND _hWnd, HWND _hDockHost, int _resizability) :
	WindowSubclass(_hWnd),
	hDockHost(_hDockHost),
	resizability(_resizability) { }


	void OnMoving()
	{
		if ((GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
			return;
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		RECT rDockHost;
		GetWindowRect(hDockHost, &rDockHost);

		if (g_hDraggingWnd == NULL)
		{
			g_hDockArrow = DockDiamond_Create(
				(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
				hWnd, rDockHost, RectCenter(rDockHost));
			if (!g_hDockArrow)
				return;
			g_hDraggingWnd = hWnd;
		}

		if (ISKEYDOWN(VK_CONTROL) || IsRectEmpty(&rDockHost))
		{
			ShowWindow(g_hDockArrow, SW_HIDE);
			return;
		}

		POINT ptCenter = RectCenter(rDockHost);
		HDHNODE hNode = DockHost_LeafNodeFromPoint(hDockHost, ptCursor);
		if (hNode)
		{
			RECT r;
			DockHost_GetNodeRect(hDockHost, hNode, &r);
			ptCenter = RectCenter(r);
			ptCenter = PTClientToScreen(hDockHost, ptCenter);
		}

		// TODO: Try moving the ShowWindow() call below the SetLayout() call.
		ShowWindow(g_hDockArrow, SW_SHOW);
		DockDiamond_SetLayout(g_hDockArrow, rDockHost, ptCenter);
		DockDiamond_Track(g_hDockArrow, ptCursor);
	}



	// Dock the window when ther users stops moving the window if he drops it on a dock diamond button.
	void OnExitSizeMove()
	{
		if ((GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) || !g_hDraggingWnd)
			return;

		g_hDraggingWnd = NULL;

		DD_Button hotButton = DockDiamond_GetHotButton(g_hDockArrow);

		SAFE_DESTROY_WINDOW(g_hDockArrow);

		if (ISKEYDOWN(VK_CONTROL) || hotButton == DDB_NOWHERE)
			return;

		DockHost_Dock(hDockHost,
			hWnd, (DH_DockLocation)hotButton,
			DockHost_LeafNodeFromPoint(hDockHost, GetCursorPos()));
	}



	LRESULT MsgProc(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_MOVING: OnMoving(); break;
		case WM_EXITSIZEMOVE: OnExitSizeMove(); break;
		case WM_NCDESTROY:
		{
			LRESULT ret = WindowSubclass::MsgProc(Msg, wParam, lParam);
			delete this;
			return ret;
		}
			break;
		}
		return WindowSubclass::MsgProc(Msg, wParam, lParam);
	}


	HWND hDockHost;
	int resizability;
};


// Subclasses the window so that it will be dockable.
BOOL MakeDockable(HWND hWnd, HWND hDockHost, int resizability)
{
	if (!hWnd || !hDockHost) return FALSE;
	if (!DOCKDIAMOND_Init((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE)))
	{
		OutputDebugString(TEXT("Failed to initialize dock diamond control"));
		return FALSE;
	}

	DockableWindowSubclass* p = new DockableWindowSubclass(hWnd, hDockHost, resizability);

	return TRUE;
}