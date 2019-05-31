#pragma once
#include <windows.h>
//DockHost Control에 있는 노드에 대한 핸들 
DECLARE_HANDLE(HDHNODE);

# define WC_DOCKHOST		TEXT("dock-host")

# define DH_DIVIDER_WIDTH		6 ///< The behavioral width of a vertical divider.
# define DH_DIVIDER_HEIGHT		5 ///< The behavioral height of a horizontal divider.
# define DH_DEFAULT_MIN_WIDTH	100
# define DH_DEFAULT_MIN_HEIGHT	75
# define DH_DEFAULT_RESIZABILITY 2

//정확히 이해가 안가는군 
# define DHNS_NO_TEAROFF		0x0001 // 도킹창은 사용자에 의해 도킹 해제 할수 없다. 
# define DHNS_NO_CAPTION		0x0002 //< Dock된 노드는 캡션 표시줄이 없다.

#ifndef WMN_FIRST
#define WMN_FIRST               (0U-1000U)
#define WMN_LAST                (0U-1200U)
#endif

//설명이 필요함
# define DHN_FIRST			(WMN_LAST - 5)
# define DHN_ITEMDOCK		(DHN_FIRST + 1) ///< 도킹 되었다.  @see NM_DOCKHOST.
# define DHN_ITEMUNDOCK		(DHN_FIRST + 2) ///< 도킹해제	   @see NM_DOCKHOST.
# define DHN_CLOSE			(DHN_FIRST + 3) 
# define DHN_TEAROFF		(DHN_FIRST + 4)

#ifndef MIN
#define MIN(x,y)				( (x)<(y)? (x) : (y) )
#endif

#ifndef MAX
#define MAX(x,y)				( (x)>(y)? (x) : (y) )
#endif

//이거랑 
#ifndef LINEAR_INTERPOLATE
//#define LINEAR_INTERPOLATE(a,b,t) ( (a)*(1-(t)) + (b)*(t) )
#define LINEAR_INTERPOLATE(a,b,t)	( (a)+(t)*((b)-(a)) )
#endif


//이거 모지 
#ifndef CLAMP
#define CLAMP(x,a,b)			( (x)>(b) ? (b) : (x)<(a) ? (a) : (x) )
#endif

//키매니저 ..............................................! 수정할 것
#ifndef ISKEYDOWN
#define ISKEYDOWN(x)			( (GetKeyState((x))&0x8000)!=0 ) ///< Returns whether a key or mouse button is pressed.
#endif

enum DH_DockLocation
{
	//DH_NOWHERE = 0,
	DH_LEFT = 0,
	DH_TOP,
	DH_RIGHT,
	DH_BOTTOM,
	DH_CLEFT,
	DH_CTOP,
	DH_CRIGHT,
	DH_CBOTTOM,
	DH_CENTER
};

enum DHHT_Component
{
	DHHT_NOTHING,
	DHHT_CLIENT,
	DHHT_CAPTION_BAR,
	DHHT_TAB
};

/// Caption bar component enumeration for hit-testing.
enum DHHT_CBComponent
{
	DHHT_CB_NOTHING,
	DHHT_CB_BAR,
	DHHT_CB_CLOSE,
	DHHT_CB_ICON,
};

//Dock host의는 Tree로 배열되어 있고, 이 eum문은 트리의 노드들 타입이다. 
enum DH_NODE_TYPE
{
	DH_LEAF = 0, // 리프 노드 - 안에 창이 있음
	DH_DIVIDER_H, //가로 구분선
	DH_DIVIDER_V //세로 구분선 
};

struct NM_DOCKHOST
{
	NMHDR	hdr;
	HDHNODE	hNode; ///< Handle to the dock host node of the notification, if applicable. Can be NULL.
	HWND	itemWnd; ///< The window docked at the dock host node, or NULL.
	VOID*	nodeUserData;
};

struct DH_HitInfo
{
	HDHNODE				hNode;
	DHHT_Component		component;
	DHHT_CBComponent	captionBarComponent; ///< Used when component == DHHT_CAPTION_BAR
	INT					iTab; ///< Used when component == DHHT_TAB
};


struct DHNodeInfo
{
	DH_NODE_TYPE	nodeType; //노드 타입 
	RECT			rect; //노드의 사각형 , 노드가 분할될경우, 양쪽에 노드를 캡슐화 함[?~]
	HDHNODE			parent; //부모노드 
	HDHNODE			childA; //차일드 _ 자식 노드 왼쪽 
	HDHNODE			childB; // 자식노드 오른쪽
	RECT			dividerRect; //분할된 실제 사각형이라고 함.. 뭐지??
	LONG			resizability;	//높이 크기 조정 우선 순위 값이 높을수록 크기가 조절될 가능성을 의미
	///< A divider's resizability is equal to the smallest resizability in its descentents.
	//리프 노드 전용 내용들..!
	HWND			hWnd; //The window docked in the node.
	HWND			hTab; //리프 노드만 한개 이상의 창을 가질 수 있다. 다른 애들은 다 NULL.
	DWORD			dwStyle; //노드의 스타일. DHNS_* constants 를 참조 
	VOID*			userData; //DochHost_Dock()에 공급된 사용자 정의 값 
};

BOOL DockHost_Init(HINSTANCE);
VOID DockHost_Uninit(HINSTANCE);
BOOL DockHost_IsDockHost(HWND hWnd);
VOID DockHost_SetNodeResizability(HDHNODE hNode, INT newResizability, BOOL bRelative);


HDHNODE DockHost_Dock(HWND hDockHost,
	HWND hWnd,
	DH_DockLocation dockLocation,
	HDHNODE hBaseNode = NULL,
	DWORD dwNodeStyle = 0,
	INT requestedWidth = -1, INT requestedHeight = -1,
	INT minWidth = -1, INT minHeight = -1,
	INT resizability = DH_DEFAULT_RESIZABILITY,
	VOID* userData = NULL);

/// Undocks an item. The passed node handle will become invalid after this call.
BOOL	DockHost_Undock(HWND hDockHost, HDHNODE hNode, HWND hNewOwner = NULL);

/// Searches for and undocks the specified window.
BOOL	DockHost_UndockWindow(HWND hDockHost, HWND hWnd, HWND hNewOwner = NULL);

//DockHost_hit 
//DockHost에서 둘다 쓰이고.... 
//지정된 노드를 리턴 해줌. 
DH_NODE_TYPE DockHost_GetNodeType(HDHNODE hNode);
HDHNODE	DockHost_GetRootNode(HWND hDockHost); ///< Returns the root node, or NULL if the control is empty.
HDHNODE	DockHost_GetNextNode(HWND hDockHost, HDHNODE hNode); ///< Returns the next node in depth-first traversal.
HDHNODE DockHost_GetNodeParent(HDHNODE node); ///< Retursn the parent of a node.
HDHNODE DockHost_GetNodeChild1(HDHNODE node); ///< For dividers only. Returns the node above or to the left of the divider.
HDHNODE DockHost_GetNodeChild2(HDHNODE node); ///< For dividers only. Returns the node below or to the right of the divider.
HDHNODE DockHost_GetNodeSibling(HDHNODE node); ///< Returns the sibling (other child of its parent) if it has one.

HWND DockHost_GetNodeWindow(HDHNODE hNode);
UINT DockHost_GetNumWindows(HDHNODE hNode);
DWORD DockHost_GetNodeStyle(HDHNODE hNode);

/// Returns the divider's position. See DockHost_SetDividerPosition() for more information on divider positions.
INT		DockHost_GetDividerPosition(HDHNODE hDivider);

//클라이언트 좌표로 노드의 RECT를 가져옴 
//음.. 정확히 모르겠음
VOID	DockHost_GetNodeRect(HWND hDockHost, HDHNODE hNode, RECT* prNode);
VOID	DockHost_GetDividerRect(HWND hDockHost, HDHNODE hDividerNode, RECT* prDivider);

/// Returns the range of valid values for divider positions that will not violate minimum size requirements
/// of child nodes.
/// Returns FALSE if the divider node is too small to contain both children without breaking minimum size
/// requirements. If that's the case, max will be smaller or equal to min. Returns TRUE otherwise.
BOOL	DockHost_GetDividerPositionRange(HWND hDockHost, HDHNODE hDivider, INT* min, INT* max);

//---------------------
//사이즈 기본 셋팅 ...! 
/// Transfers nodes (and associated windows) from one dock host window to another.
/// @param hSrcDockHost		The source dock host window - the dock host window containing the nodes that are going to be moved.
/// @param hSrcNode			The node that will be moved to the new window. All children are also moved.
/// @param hDestDockHost	The destination dock host window.
/// @param dockLocation		The location where the node will be inserted in the destination dock host window.
/// @param hBaseNode		The node in the destination dock host window, relative to which the node will be inserted.
///							This can be NULL.
BOOL	DockHost_TransferNodes(HWND hSrcDockHost, HDHNODE hSrcNode,
	HWND hDestDockHost,
	DH_DockLocation dockLocation,
	HDHNODE hBaseNode);

/// Sets the position of a divider node. The position is relative.
/// This will fail if the new position violates the minimum size restrictions of child nodes
/// unless bForce is TRUE. It will also fail if the specified node is not a divider node.
/// The function will also fail if the specified position is outside of the node's rectangle space.
VOID DockHost_OnNodeResized(HWND hDockHost, HDHNODE hNode);
BOOL	DockHost_SetDividerPosition(HWND hDockHost, HDHNODE hDivider, INT pos, BOOL bForce = FALSE, BOOL bUpdate = TRUE);

/// Retrieves info about a node. See the DHNodeInfo structure.
VOID DockHost_GetNodeInfo(HWND hDockHost, HDHNODE, DHNodeInfo* pNI);

// Dock Host CaptionBar
//특정 잎 노드의 모든 구성 요소 사각형을 반환
BOOL DockHost_GetLeafNodeRects(HWND hDockHost, HDHNODE hNode, RECT* prNode,RECT* prClient,
										RECT* prTab, RECT* prCaptionBar, RECT* prCloseButton,
										RECT* prText, RECT* prIcon);
//Dockhost hit / mousing
HDHNODE DockHost_Hit(HWND hDockHost, POINT pt);
HDHNODE DockHost_Hit(HWND hDockHost, POINT pt, DH_HitInfo* pHTI);


/// Floating point rounding.
inline LONG fround(float f)	{ if (f < 0) return (LONG)(f - 0.5f); return (LONG)(f + 0.5f); }

/// Linear interpolation between two COLORREF values.
inline COLORREF InterpolateCOLORREF(COLORREF c1, COLORREF c2, float t)
{
	INT r = (INT)LINEAR_INTERPOLATE(GetRValue(c1), GetRValue(c2), t);
	INT g = (INT)LINEAR_INTERPOLATE(GetGValue(c1), GetGValue(c2), t);
	INT b = (INT)LINEAR_INTERPOLATE(GetBValue(c1), GetBValue(c2), t);
	return RGB(CLAMP(r, 0, 255), CLAMP(g, 0, 255), CLAMP(b, 0, 255));
}
inline SIZE GetBitmapSize(HBITMAP hBmp)
{
	SIZE sz = { 0 };
	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);
	sz.cx = bm.bmWidth;
	sz.cy = bm.bmHeight;
	return sz;
}

//DockHost.cpp
inline BOOL DockHost_IsEmpty(HWND hDockHost)
{
	return NULL == DockHost_GetRootNode(hDockHost);
}

inline HDHNODE DockHost_LeafNodeFromPoint(HWND hDockHost, POINT ptScreen)
{
	ScreenToClient(hDockHost, &ptScreen);
	HDHNODE node = DockHost_Hit(hDockHost, ptScreen, NULL);
	return (node && DockHost_GetNodeType(node) == DH_LEAF) ? node : NULL;
}
inline BOOL DockHost_IsDockLocationRelative(DH_DockLocation dockLocation)
{
	return (INT)dockLocation >= (INT)DH_CLEFT;
}

/// Sets and clears style flags for a particular window.
inline VOID ModifyWindowStyle(HWND hWnd, DWORD flagsToDisable, DWORD flagsToEnable)
{
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	SetWindowLong(hWnd, GWL_STYLE, (style & ~flagsToDisable) | flagsToEnable);
}

//Hit와 SubClass 에서 사용된다. 
//기본 ClientToscreen이 있다 ㄷㄷ 
inline POINT PTClientToScreen(HWND hWnd, POINT pt) { ::ClientToScreen(hWnd, &pt); return pt; }

