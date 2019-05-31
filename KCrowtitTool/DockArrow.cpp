#include "stdafx.h"
#include "DockArrow.h"
#include "Res\resource.h"

# define WC_DOCKDIAMOND		TEXT("Dock_Arrow")
# define IMAGE_SIZE 24	// Size of each image in the diamond's image list

struct TDockDiamond
{
	HWND hwnd;
	HWND hOwner;
	HRGN hRgn;
	HRGN centerRgn;
	HRGN sideRectsRgn;
	RECT rects[9];
	DD_Button hotButton;
	POINT center;

	TDockDiamond(HWND _hwnd)
	{
		hwnd = _hwnd;
		hRgn = NULL;
		centerRgn = NULL;
		sideRectsRgn = NULL;
		hotButton = DDB_NOWHERE;
		center.x = center.y = 0;
	}
	~TDockDiamond()
	{
		if (hRgn) DeleteObject(hRgn);
		if (centerRgn) DeleteObject(centerRgn);
		if (sideRectsRgn) DeleteObject(sideRectsRgn);
	}
};

TDockDiamond* GetControlData(HWND hWnd)				{ return (TDockDiamond*)GetWindowLongPtr(hWnd, 0); }
VOID SetControlData(HWND hWnd, TDockDiamond* pDD)		{ SetWindowLongPtr(hWnd, 0, (LONG)pDD); }

ATOM DockDiamondAtom = NULL;
BOOL DockDiamondInited = FALSE;
typedef DWORD(WINAPI* PFSLWA)(HWND, DWORD, BYTE, DWORD); // SetLayeredWindowAttributes()
static PFSLWA pfSetLayeredWindowAttributes = NULL;
static HIMAGELIST hImageList = NULL; // One image list for all dock diamond instances.
static HMODULE hUser32DLL = NULL;
LRESULT CALLBACK DockDiamondWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID RectFrom2Points(RECT* pRect, POINT* p1, POINT* p2) { SetRect(pRect, p1->x, p1->y, p2->x, p2->y); }
VOID PointsFromRect(POINT* pts, RECT* r)
{
	SetPoint(&pts[0], r->left, r->top);
	SetPoint(&pts[1], r->left, r->bottom);
	SetPoint(&pts[2], r->right, r->bottom);
	SetPoint(&pts[3], r->right, r->top);
}


VOID WindowRectFromHostRect(RECT& rWindow, const RECT& rHost)
{
	rWindow = rHost;
	InflateRect(&rWindow, IMAGE_SIZE * 2, IMAGE_SIZE * 2);
}

BOOL DOCKDIAMOND_Init(HINSTANCE hInstance)
{
	if (DockDiamondInited)
		return DockDiamondAtom != NULL;
	DockDiamondInited = TRUE;

	hImageList = ImageList_LoadImage(hInstance, MAKEINTRESOURCE(IDB_DH_ARROWS),
		IMAGE_SIZE, 15, 0, IMAGE_BITMAP, LR_CREATEDIBSECTION);

	if (!hImageList)
		return FALSE;

	// Register the dock diamond window class.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = DockDiamondWndProc;
	wcex.cbWndExtra = sizeof(TDockDiamond*);
	wcex.hInstance = hInstance;
	//wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.lpszClassName = WC_DOCKDIAMOND;
	DockDiamondAtom = RegisterClassEx(&wcex);

	if (NULL == DockDiamondAtom)
		return FALSE;
	//이 dll은 도대체?
	hUser32DLL = LoadLibrary(TEXT("user32"));
	if (hUser32DLL)
		pfSetLayeredWindowAttributes = (PFSLWA)GetProcAddress(hUser32DLL, "SetLayeredWindowAttributes");

	return TRUE;
}
BOOL OffsetCenter(TDockDiamond* pDD, POINT ptNewCenter)
{
	ScreenToClient(pDD->hwnd, &ptNewCenter);
	if (pDD->center.x == ptNewCenter.x && pDD->center.y == ptNewCenter.y)
		return FALSE;

	INT xOffset = ptNewCenter.x - pDD->center.x;
	INT yOffset = ptNewCenter.y - pDD->center.y;

	pDD->center = ptNewCenter;

	OffsetRect(&pDD->rects[DDB_CLEFT], xOffset, yOffset);
	OffsetRect(&pDD->rects[DDB_CTOP], xOffset, yOffset);
	OffsetRect(&pDD->rects[DDB_CRIGHT], xOffset, yOffset);
	OffsetRect(&pDD->rects[DDB_CBOTTOM], xOffset, yOffset);
	OffsetRect(&pDD->rects[DDB_CENTER], xOffset, yOffset);

	OffsetRgn(pDD->centerRgn, xOffset, yOffset);
	return TRUE;
}



// Returns FALSE if no changes have been made.
VOID PrepareSideRects(TDockDiamond* pDD, const RECT& rDockHost)
{
	RECT rButton = { 0, 0, IMAGE_SIZE, IMAGE_SIZE };
	OffsetRect(&rButton, -IMAGE_SIZE / 2, -IMAGE_SIZE / 2);

	RECT rWindow;
	WindowRectFromHostRect(rWindow, rDockHost);
	INT padding = (RectWidth(rWindow) - RectWidth(rDockHost)) / 2 + 5 + IMAGE_SIZE / 2;

	// Calculate side rectangles. These are rects[0-3].
	pDD->rects[0] = pDD->rects[1] = pDD->rects[2] = pDD->rects[3] = rButton;

	OffsetRect(&pDD->rects[DDB_LEFT], padding, RectHeight(rWindow) / 2); // Left.
	OffsetRect(&pDD->rects[DDB_TOP], RectWidth(rWindow) / 2, padding); // Top.
	OffsetRect(&pDD->rects[DDB_RIGHT], RectWidth(rWindow) - padding, RectHeight(rWindow) / 2); // Right.
	OffsetRect(&pDD->rects[DDB_BOTTOM], RectWidth(rWindow) / 2, RectHeight(rWindow) - padding); // Bottom.

	// One polygonal region for side rectangles. We use the rectangles to calculate
	// the points of the polygon region for the side rects.
	POINT pts[16];
	PointsFromRect(pts, pDD->rects);
	PointsFromRect(pts + 4, pDD->rects + 1);
	PointsFromRect(pts + 8, pDD->rects + 2);
	PointsFromRect(pts + 12, pDD->rects + 3);

	if (pDD->sideRectsRgn)
	{
		DeleteObject(pDD->sideRectsRgn);
		pDD->sideRectsRgn = NULL;
	}

	INT pointCounts[] = { 4, 4, 4, 4 };
	pDD->sideRectsRgn = CreatePolyPolygonRgn(pts, pointCounts, 4, WINDING);
}


// Called when either the side or the center regions are updated. The window region
// is a combination of these two regions.
VOID OnUpdateRegion(TDockDiamond* pDD, BOOL bRedraw = FALSE)
{
	if (pDD->hRgn)
	{
		DeleteObject(pDD->hRgn);
		pDD->hRgn = NULL;
	}

	pDD->hRgn = CreateRectRgn(0, 0, 0, 0); // Combine dest can't be NULL.
	CombineRgn(pDD->hRgn, pDD->sideRectsRgn, pDD->centerRgn, RGN_OR);
	SetWindowRgn(pDD->hwnd, pDD->hRgn, FALSE);
}
HWND DockDiamond_Create(HINSTANCE hInstance, HWND hOwner, const RECT& rDockHost, POINT ptCenter)
{

	//  -------------------
	// |   _____________   |
	// |  |             |  |
	// |  |_____________|  |
	// |                   |
	//  -------------------
	// Inner rectangle is the host rectangle - we create the dock diamond window slightly
	// larger because the central part of the diamond could spill outside the dock host.
	// The difference on each side is IMAGE_SIZE*2.

	RECT rWindow;
	WindowRectFromHostRect(rWindow, rDockHost);

	HWND hDD = CreateWindowEx(WS_EX_TOPMOST | (pfSetLayeredWindowAttributes ? WS_EX_LAYERED : 0),
		WC_DOCKDIAMOND, NULL, WS_POPUP,
		rWindow.left, rWindow.top, RECTWIDTH(rWindow), RECTHEIGHT(rWindow),
		hOwner, NULL, hInstance, NULL);

	if (!hDD)
		return NULL;

	TDockDiamond* pDD = GetControlData(hDD);
	pDD->hOwner = hOwner;

	// Now calculate the central polygonal region. We calculate the points first then
	// use these to calculate rectangles.
	//
	//  00   0----11
	//       |    |
	//  2----1    10---9
	//  |              |
	//  3----4    7----8
	//       |    |
	//       5----6
	//

	POINT centerPts[12];
	SetPoint(&centerPts[0], IMAGE_SIZE, 0);
	SetPoint(&centerPts[1], IMAGE_SIZE, IMAGE_SIZE);
	SetPoint(&centerPts[2], 0, IMAGE_SIZE);
	SetPoint(&centerPts[3], 0, IMAGE_SIZE * 2);
	SetPoint(&centerPts[4], IMAGE_SIZE, IMAGE_SIZE * 2);
	SetPoint(&centerPts[5], IMAGE_SIZE, IMAGE_SIZE * 3);
	SetPoint(&centerPts[6], IMAGE_SIZE * 2, IMAGE_SIZE * 3);
	SetPoint(&centerPts[7], IMAGE_SIZE * 2, IMAGE_SIZE * 2);
	SetPoint(&centerPts[8], IMAGE_SIZE * 3, IMAGE_SIZE * 2);
	SetPoint(&centerPts[9], IMAGE_SIZE * 3, IMAGE_SIZE);
	SetPoint(&centerPts[10], IMAGE_SIZE * 2, IMAGE_SIZE);
	SetPoint(&centerPts[11], IMAGE_SIZE * 2, 0);


	// Calculate central rects from points.
	RectFrom2Points(&pDD->rects[DDB_CLEFT], centerPts + 2, centerPts + 4);
	RectFrom2Points(&pDD->rects[DDB_CTOP], centerPts + 0, centerPts + 10);
	RectFrom2Points(&pDD->rects[DDB_CRIGHT], centerPts + 10, centerPts + 8);
	RectFrom2Points(&pDD->rects[DDB_CBOTTOM], centerPts + 4, centerPts + 6);
	RectFrom2Points(&pDD->rects[DDB_CENTER], centerPts + 1, centerPts + 7);

	pDD->centerRgn = CreatePolygonRgn(centerPts, 12, WINDING);

	pDD->center.x = IMAGE_SIZE + IMAGE_SIZE / 2;
	pDD->center.y = pDD->center.x;

	OffsetCenter(pDD, RectCenter(rDockHost));

	PrepareSideRects(pDD, rDockHost);

	OnUpdateRegion(pDD, FALSE);

	if (pfSetLayeredWindowAttributes)
		pfSetLayeredWindowAttributes(hDD, RGB(255, 255, 255), 200, LWA_COLORKEY | LWA_ALPHA);

	ShowWindow(hDD, SW_SHOW);
	//g_Log( "dock diamond created. Parent: %d", hDockHost );

	// TODO:
	// DoHitTest() to propery highlight button under cursor.

	return hDD;
}

inline bool operator == (const RECT& r1, const RECT& r2)
{
	return r1.left == r2.left && r1.right == r2.right && r1.top == r2.top && r1.bottom == r2.bottom;
}
inline bool operator != (const RECT& r1, const RECT& r2) { return !(r1 == r2); }

VOID DockDiamond_SetLayout(HWND hDockDiamond, const RECT& rDockHost, POINT ptCenter)
{
	TDockDiamond* pDD = GetControlData(hDockDiamond);
	RECT rWindow, rWindowNew;
	GetWindowRect(hDockDiamond, &rWindow);
	WindowRectFromHostRect(rWindowNew, rDockHost);
	BOOL bUpdate = FALSE;

	if (rWindow != rWindowNew)
	{
		MoveWindow(hDockDiamond, rWindowNew.left, rWindowNew.top, RectWidth(rWindowNew), RectHeight(rWindowNew), FALSE);
		PrepareSideRects(pDD, rDockHost);
		bUpdate = TRUE;
	}
	if (OffsetCenter(pDD, ptCenter))
		bUpdate = TRUE;
	if (bUpdate)
	{
		InvalidateRect(hDockDiamond, NULL, TRUE);
		OnUpdateRegion(pDD, TRUE);
		UpdateWindow(hDockDiamond);
	}
}

DD_Button DockDiamond_Hit(HWND hDockDiamond, POINT pt)
{
	TDockDiamond* pDD = GetControlData(hDockDiamond);
	//if( !PtInRegion( pDD->hRgn, pt->x, pt->y ) )
	//	return DH_NOWHERE;

	// Check the last hit rectangle first - it's more likely the mouse is still in it.
	if (pDD->hotButton != DDB_NOWHERE)
	if (PtInRect(&pDD->rects[pDD->hotButton], pt))
		return pDD->hotButton;

	// First check the center because the center can be drawn over one of the side rects.
	INT i;
	for (i = (INT)DDB_CLEFT; i <= (INT)DDB_CENTER; i++)
	if (PtInRect(&pDD->rects[i], pt))
		return (DD_Button)i;

	for (i = (INT)DDB_LEFT; i<(INT)DDB_CLEFT; i++)
	if (PtInRect(&pDD->rects[i], pt))
		return (DD_Button)i;

	return DDB_NOWHERE;
}


//창 누를때마다 여기 들어옴 
DD_Button DockDiamond_Track(HWND hDockDiamond, POINT ptCursor)
{
	ScreenToClient(hDockDiamond, &ptCursor);
	TDockDiamond* pDD = GetControlData(hDockDiamond);
	DD_Button btn = DockDiamond_Hit(hDockDiamond, ptCursor);
	if (btn != pDD->hotButton)
	{
		pDD->hotButton = btn;
		InvalidateRect(hDockDiamond, NULL, FALSE); // TODO: Invalidate only rects that need it.
		RedrawWindow(hDockDiamond, NULL, pDD->hRgn, RDW_UPDATENOW);
		if (pDD->hOwner)
		{
			NMHDR nmhdr;
			nmhdr.code = DDN_SELCHANGED;
			nmhdr.hwndFrom = hDockDiamond;
			nmhdr.idFrom = GetWindowLong(hDockDiamond, GWL_ID);
			SendMessage(pDD->hOwner, WM_NOTIFY, (WPARAM)btn, (LPARAM)&nmhdr);
		}
	}
	OutputDebugString(TEXT("DockDiamond_Track()ing\n"));
	return btn;
}

DD_Button DockDiamond_GetHotButton(HWND hDockDiamond)
{
	TDockDiamond* pDD = GetControlData(hDockDiamond); return pDD->hotButton;
}


VOID Paint(TDockDiamond* pDD, HDC hdc)
{
	if (!hImageList)
		return;

	for (INT i = (INT)DDB_LEFT; i < (INT)DDB_CLEFT; i++)
	{
		INT iImage = i;
		if (i == (INT)pDD->hotButton)
			iImage += 5;
		ImageList_Draw(hImageList, iImage, hdc, pDD->rects[i].left, pDD->rects[i].top, 0);
	}

	for (INT i = (INT)DDB_CLEFT; i <= (INT)DDB_CENTER; i++)
	{
		INT iImage = i - (INT)DDB_CLEFT;
		if (i == (INT)pDD->hotButton)
			iImage += 5;
		ImageList_Draw(hImageList, iImage, hdc, pDD->rects[i].left, pDD->rects[i].top, 0);
	}
}


LRESULT CALLBACK DockDiamondWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	TDockDiamond* pDD = GetControlData(hWnd);
	switch (Msg)
	{
	case WM_NCCREATE:
		pDD = new TDockDiamond(hWnd);
		SetControlData(hWnd, pDD);
		return pDD != NULL;

	case WM_NCDESTROY:
		delete pDD;
		break;

		/*case WM_KILLFOCUS:
		DestroyWindow(hWnd);
		break;*/

	case WM_ERASEBKGND:
		return 1;

	case WM_CREATE:
		if (pDD->hOwner)
		{
			NMHDR nmhdr;
			nmhdr.code = DDN_SELCHANGED;
			nmhdr.hwndFrom = hWnd;
			nmhdr.idFrom = GetWindowLong(hWnd, GWL_ID);
			SendMessage(pDD->hOwner, WM_NOTIFY, (WPARAM)DDB_NOWHERE, (LPARAM)&nmhdr);
		}
		break;


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Paint(pDD, hdc);
		EndPaint(hWnd, &ps);
	}
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

