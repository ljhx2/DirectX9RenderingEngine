#pragma once
# include <commctrl.h>
# pragma comment( lib, "Comctl32.lib" )

// Messages.
#define DDM_FIRST 0x1400

// Notifications
#define DDN_SELCHANGED		(DDM_FIRST + 20)

#ifndef RECTWIDTH
#define RECTWIDTH(r)			( ((r).right - (r).left) ) ///< The height of a RECT structure.
#endif

#ifndef RECTHEIGHT
#define RECTHEIGHT(r)			( ((r).bottom - (r).top) ) ///< The width of a RECT structure.
#endif

inline void SetPoint(POINT* pt, LONG x, LONG y) { pt->x = x; pt->y = y; }
inline void SetPoint(POINTS* pts, SHORT x, SHORT y) { pts->x = x; pts->y = y; }

// TODO: Get rid of this. include DockHost.h and use the dock host location enum directly instead.
enum DD_Button
{
	DDB_NOWHERE = -1,
	DDB_LEFT = 0,
	DDB_TOP,
	DDB_RIGHT,
	DDB_BOTTOM,
	DDB_CLEFT,
	DDB_CTOP,
	DDB_CRIGHT,
	DDB_CBOTTOM,
	DDB_CENTER,
};

//sub class와 연관ㄷ ㅚㅇ ㅓ있네 

BOOL		DOCKDIAMOND_Init(HINSTANCE);
VOID		DOCKDIAMOND_Uninit();

HWND		DockDiamond_Create(HINSTANCE hInstance, HWND hOwner, const RECT& rDockHost, POINT ptCenter);
DD_Button	DockDiamond_Track(HWND hDockDiamond, POINT ptCursor); ///< ptCursor is in screen coordinates.
VOID		DockDiamond_EnableButton(HWND hDockDiamond, DD_Button button, BOOL bEnable);
VOID		DockDiamond_SetCenter(HWND hDockDiamond, POINT pt); ///< Point is in client coords (??Why??)
POINT		DockDiamond_GetCenter(HWND hDockDiamond);
VOID		DockDiamond_SetLayout(HWND hDockDiamond, const RECT& rDockHost, POINT ptCenter);
DD_Button	DockDiamond_Hit(HWND hDockDiamond, POINT pt);
DD_Button	DockDiamond_GetHotButton(HWND hDockDiamond);
