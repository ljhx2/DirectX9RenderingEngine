#pragma once
#include "DockHost.h"
#include "IntrusiveWeakTree.h"

struct DHNode
{
	DHNode(DH_NODE_TYPE _type) : type(_type) { cachedSize.cx = cachedSize.cy = 0; }
	CSTB::TreeHook	hook;
	DH_NODE_TYPE	type;
	SIZE			cachedSize;
	virtual SIZE	GetMinSize() const = 0;
	virtual LONG	GetResizability() const = 0;
};
typedef CSTB::IntrusiveWeakTreeT< DHNode, offsetof(DHNode, DHNode::hook) > DHNodeTree;


struct TDockHost
{
private:
	DHNodeTree m_NodeTree;
public:
	HWND		hWnd;
	INT			numNodes;
	INT			numLeaves;
	INT			numDividers;
	DHNode*		pLastHitNode;

	TDockHost(HWND _hWnd)
	{
		hWnd = _hWnd;
		numNodes = 0;
		numLeaves = 0;
		numDividers = 0;
		pLastHitNode = NULL;
	}

	DHNodeTree& GetNodeTree() { return m_NodeTree; }
};
//리프노드 
struct DHLeafNode : public DHNode
{
	SIZE	minSize;
	LONG	resizability;
	HWND	hWnd;
	DWORD	dwUndockedStyle;
	DWORD	dwStyle;
	RECT	undockedRect;
	VOID*	userData;

	DHLeafNode() : DHNode(DH_LEAF)
	{
		minSize.cx = minSize.cy = 0;
		resizability = DH_DEFAULT_RESIZABILITY;
		hWnd = NULL;
		dwUndockedStyle = 0;
		dwStyle = 0;
		SetRect(&undockedRect, 0, 0, 0, 0);
		userData = 0;
	}

	SIZE GetMinSize() const { return this->minSize; }
	LONG GetResizability() const { return this->resizability; }
};

//이거 실제로 사용하는지 확인 해봐야함. .ㄷㄷㄷㄷ 
class DHDividerNode : public DHNode
{
public:
	INT pos;

	DHDividerNode(DH_NODE_TYPE dividerType) : DHNode(dividerType) { pos = -1; }
	DHNode* ChildA() const { return DHNodeTree::FirstChild(this); }
	DHNode* ChildB() const { return DHNodeTree::LastChild(this); }

	SIZE GetMinSize() const
	{
		SIZE minSizeA = ChildA()->GetMinSize();
		SIZE minSizeB = ChildB()->GetMinSize();
		SIZE retSize;

		if (type == DH_DIVIDER_H)
		{
			retSize.cx = MAX(minSizeA.cx, minSizeB.cx);
			retSize.cy = minSizeA.cy + minSizeB.cy + DH_DIVIDER_HEIGHT;
		}
		else
		{
			retSize.cx = minSizeA.cx + minSizeB.cx + DH_DIVIDER_WIDTH;
			retSize.cy = MAX(minSizeA.cy, minSizeB.cy);
		}
		return retSize;
	}

	LONG GetResizability() const
	{
		INT ra = ChildA()->GetResizability();
		INT rb = ChildB()->GetResizability();
		return MAX(ra, rb);
	}
};

inline TDockHost*	GetControlData(HWND hWnd)					{ return (TDockHost*)GetWindowLongPtr(hWnd, 0); }
inline VOID			SetControlData(HWND hWnd, TDockHost* pDH)	{ SetWindowLongPtr(hWnd, 0, (LONG)pDH); }

//이게 모지... 컨트롤 데이터를 얻어온다??
inline TDockHost*	CONV(HWND hWnd)			{ return GetControlData(hWnd); }
inline DHNode*		CONV(HDHNODE hNode)		{ return (DHNode*)hNode; }
inline HDHNODE		CONV(DHNode* pNode)		{ return (HDHNODE)pNode; }


inline DHLeafNode* GetLeafNode(DHNode* pNode)
{
	return pNode && pNode->type == DH_LEAF ? (DHLeafNode*)pNode : NULL;
}


inline DHDividerNode* GetDividerNode(DHNode* pNode)
{
	return pNode && pNode->type == DH_LEAF ? NULL : (DHDividerNode*)pNode;
}

inline DHLeafNode*		GetLeafNode(HDHNODE hNode)	{ return GetLeafNode(CONV(hNode)); }
//실제로 사용한느가??
inline DHDividerNode*	GetDividerNode(HDHNODE hNode) { return GetDividerNode(CONV(hNode)); }


//CaptionBar
VOID CalculateDockedCaptionRects(HWND hWnd, const RECT& rClient, RECT* prCaption, RECT* prText, RECT* prIcon, RECT* prClose);
BOOL DrawDockedCaption(HWND hWnd, HDC hdc, const RECT& rClient, BOOL bActive, BOOL bCloseHot);


inline VOID DrawBitmap(HDC hDC, HBITMAP hBitmap, INT xDest, INT yDest, INT cxDest, INT cyDest,
	INT srcX = 0, INT srcY = 0, DWORD rop = SRCCOPY)
{
	HDC hTempDC = CreateCompatibleDC(hDC);
	HGDIOBJ hOldBMP = SelectObject(hTempDC, hBitmap);

	SIZE sz = GetBitmapSize(hBitmap);

	if (cxDest == 0) cxDest = sz.cx;
	if (cyDest == 0) cyDest = sz.cy;

	StretchBlt(hDC, xDest, yDest, cxDest, cyDest, hTempDC, srcX, srcY, sz.cx, sz.cy, rop);
	//BitBlt( hDC, xDest, yDest, cxDest, cyDest, hTempDC, srcX, srcY, rop );

	SelectObject(hTempDC, hOldBMP);
	DeleteDC(hTempDC);
}

//HostMousing
LRESULT DockHostMouseMsgProc(TDockHost* pDH, UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
LRESULT DockHost_Notify(HWND hDockHost, UINT code, HDHNODE hNode);

//HostMousing
struct CloseButton
{
	HDHNODE hNode;
	HWND hWnd;
	RECT rClose;
	CloseButton() { hNode = NULL; hWnd = NULL; }
};
const CloseButton&	DockHost_GetHotCloseButton();