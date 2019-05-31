#include "stdafx.h"
#include "DockHostImpl.h"

/*
inline TDockHost*	CONV(HWND hWnd)			{ return GetControlData(hWnd); }
inline DHNode*		CONV(HDHNODE hNode)		{ return (DHNode*)hNode; }
inline HDHNODE		CONV(DHNode* pNode)		{ return (HDHNODE)pNode; }
*/
DH_NODE_TYPE DockHost_GetNodeType(HDHNODE hNode)
{
	return CONV(hNode)->type;
}

HWND DockHost_GetNodeWindow(HDHNODE hNode)
{
	//getLeafNode는 impl에 잇음
	return GetLeafNode(hNode) ? GetLeafNode(hNode)->hWnd : NULL;
}

UINT DockHost_GetNumWindows(HDHNODE hNode)
{
	return DockHost_GetNodeWindow(hNode) ? 1 : 0;
}

DWORD DockHost_GetNodeStyle(HDHNODE hNode)
{
	return GetLeafNode(hNode) ? GetLeafNode(hNode)->dwStyle : 0;
}

HDHNODE DockHost_GetNodeParent(HDHNODE hNode)
{
	return CONV(hNode) ? CONV(DHNodeTree::Parent(CONV(hNode))) : 0;
}
HDHNODE DockHost_GetNodeChild1(HDHNODE hNode)
{
	return CONV(hNode) ? CONV(DHNodeTree::FirstChild(CONV(hNode))) : 0;
}

HDHNODE DockHost_GetNodeChild2(HDHNODE hNode)
{
	return CONV(hNode) ? CONV(DHNodeTree::LastChild(CONV(hNode))) : 0;
}

INT DockHost_GetDividerPosition(HDHNODE hDivider)
{
	return GetDividerNode(hDivider) ? GetDividerNode(hDivider)->pos : -1;
}

HDHNODE DockHost_GetRootNode(HWND hDockHost)
{
	return CONV(CONV(hDockHost)->GetNodeTree().First());
}

HDHNODE DockHost_GetNextNode(HWND hDockHost, HDHNODE hNode)
{
	return CONV(CONV(hDockHost)->GetNodeTree().LogicalNext(CONV(hNode)));
}


HDHNODE DockHost_GetNodeSibling(HDHNODE hNode)
{
	HDHNODE hParent = DockHost_GetNodeParent(hNode);
	if (!hParent) return NULL;
	HDHNODE hChild1 = DockHost_GetNodeChild1(hParent);
	HDHNODE hChild2 = DockHost_GetNodeChild2(hParent);
	return hChild1 == hNode ? hChild2 : hChild1;
}



// Internal function that is used to insert a node into the dock host tree. This is used
// by DockHost_Dock() to insert a new node, as well as DockHost_TransferNodes() to move
// nodes from one tree to another. The node is assumed to be free (not inserted into any trees).
// The function will create a divider if necessary.
VOID DockHost_InsertNode(HWND hDockHost, DHNode* pNode, DHNode* pSibling,
	DH_DockLocation dockLocation,
	INT newItemWidth, INT newItemHeight)
{
	DHNodeTree& nodeTree = GetControlData(hDockHost)->GetNodeTree();

	// If there are other nodes in the tree, we need a divider node in addition to a leaf node.
	if (pSibling)
	{
		DH_NODE_TYPE dividerType = DH_DIVIDER_H;
		if (dockLocation == DH_LEFT || dockLocation == DH_RIGHT ||
			dockLocation == DH_CLEFT || dockLocation == DH_CRIGHT)
			dividerType = DH_DIVIDER_V;

		// Create a divider and have it replace the sibling in the tree.
		DHDividerNode* pDivider = new DHDividerNode(dividerType);
		HDHNODE hDivider = CONV(pDivider);
		nodeTree.Add(pDivider, DHNodeTree::Parent(pSibling), pSibling);
		nodeTree.Remove(pSibling);
		nodeTree.Add(pSibling, pDivider);

		assert(DHNodeTree::FirstChild(pDivider) == pSibling);

		// Now insert the node. If we are docking to the left or above the divider,
		// the new node needs to be the first child, otherwise it's the last child.
		if (dockLocation == DH_LEFT || dockLocation == DH_CLEFT ||
			dockLocation == DH_TOP || dockLocation == DH_CTOP)
			nodeTree.Add(pNode, pDivider, pSibling);
		else
			nodeTree.Add(pNode, pDivider, NULL);

		// Set the divider position based on the new item's width or height.
		RECT r;
		DockHost_GetNodeRect(hDockHost, hDivider, &r);
		INT dividerPos;
		if (dividerType == DH_DIVIDER_V)
		{
			dividerPos = newItemWidth;
			if (DockHost_GetNodeChild2(hDivider) == CONV(pNode))
				dividerPos = RectWidth(r) - newItemWidth - DH_DIVIDER_WIDTH;
		}
		else
		{
			dividerPos = newItemHeight;
			if (DockHost_GetNodeChild2(hDivider) == CONV(pNode))
				dividerPos = RectHeight(r) - newItemHeight - DH_DIVIDER_HEIGHT;
		}
		DockHost_SetDividerPosition(hDockHost, hDivider, dividerPos, FALSE, TRUE);
		SIZE divSize = { RectWidth(r), RectHeight(r) };
		CONV(hDivider)->cachedSize = divSize;
	}
	else
	{
		assert(nodeTree.Empty());
		nodeTree.Add(pNode, NULL, NULL);
	}
}
// Moves the windows of a leaf node to fit the leaf node. This is called when the leaf node is
// moved/resized, when windows are added, and when the leaf node style changes.
VOID DockHost_LayoutLeafWindows(HWND hDockHost, HDHNODE hNode)
{
	DHLeafNode* pNode = GetLeafNode(hNode);
	if (!pNode)
		return;

	if (!pNode->hWnd)
		return;

	RECT rClient;
	DockHost_GetLeafNodeRects(hDockHost, hNode, NULL, &rClient, NULL, NULL, NULL, NULL, NULL);
	MoveWindow(pNode->hWnd, rClient.left, rClient.top, RectWidth(rClient), RectHeight(rClient), TRUE);
	//UpdateWindow( pNode->hWnd );
}
VOID DockHost_SetNodeWindow(HWND hDockHost, HDHNODE hNode, HWND hWnd)
{
	DHLeafNode* pNode = GetLeafNode(hNode);
	if (!pNode || pNode->hWnd == hWnd)
		return;

	// Unparent the current window, if there is one.
	if (pNode->hWnd)
	{
		HWND hOwner = GetAncestor(pNode->hWnd, GA_ROOTOWNER);

		ModifyWindowStyle(pNode->hWnd,
			WS_CHILD,
			//WS_POPUP | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION );
			pNode->dwUndockedStyle);

		SetParent(pNode->hWnd, NULL);
		SetWindowLongPtr(pNode->hWnd, GWLP_HWNDPARENT, (LONG)hOwner);

		MoveWindow(pNode->hWnd, pNode->undockedRect.left, pNode->undockedRect.top,
			RectWidth(pNode->undockedRect), RectHeight(pNode->undockedRect), TRUE);
	}
	pNode->hWnd = hWnd;
	if (hWnd)
	{
		GetWindowRect(hWnd, &pNode->undockedRect);
		pNode->dwUndockedStyle = GetWindowLong(hWnd, GWL_STYLE);

		ModifyWindowStyle(hWnd,
			WS_POPUP | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION,
			WS_CHILD);
		SetParent(hWnd, hDockHost);

		DockHost_LayoutLeafWindows(hDockHost, CONV(pNode));
	}
}




HDHNODE DockHost_Dock(HWND hDockHost, HWND hWndToDock, DH_DockLocation dockLocation,
	HDHNODE hBaseNode, DWORD dwNodeStyle,
	INT newItemWidth, INT newItemHeight,
	INT newItemMinWidth, INT newItemMinHeight, INT resizability,
	VOID* userData)
{
	if (DockHost_IsDockLocationRelative(dockLocation) && DockHost_IsEmpty(hDockHost))
		dockLocation = DH_LEFT;

	if (DockHost_IsDockLocationRelative(dockLocation) && (hBaseNode == NULL || DockHost_GetNodeType(hBaseNode) != DH_LEAF))
	{
		OutputDebugString(TEXT(__FUNCTION__)TEXT(": Invalid relative item specificied."));
		return FALSE;
	}

	HDHNODE hSibling = DockHost_GetRootNode(hDockHost);
	if (DockHost_IsDockLocationRelative(dockLocation))
		hSibling = hBaseNode;


	RECT rSibling, rClient;
	GetClientRect(hDockHost, &rClient);
	if (hSibling)
		DockHost_GetNodeRect(hDockHost, hSibling, &rSibling);
	else
		rSibling = rClient;


	// Calculate the pixels we can spare for the new item in x and y.
	INT spareWidth = RectWidth(rClient);
	INT spareHeight = RectHeight(rClient);

	if (!DockHost_IsEmpty(hDockHost))
	{
		if (dockLocation == DH_LEFT || dockLocation == DH_RIGHT ||
			dockLocation == DH_CLEFT || dockLocation == DH_CRIGHT)
			spareWidth = RectWidth(rSibling) - (CONV(hSibling)->GetMinSize().cx + DH_DIVIDER_WIDTH);
		else
			spareHeight = RectHeight(rSibling) - (CONV(hSibling)->GetMinSize().cy + DH_DIVIDER_HEIGHT);
	}

	if (newItemMinWidth > spareWidth || newItemMinHeight > spareHeight)
	{
		OutputDebugString(TEXT(__FUNCTION__)TEXT(": Not enough room to dock new item."));
		return FALSE;
	}

	CONV(hDockHost)->pLastHitNode = NULL;

	// Adjust the new item width and height.
	if (newItemWidth <= 0 || newItemHeight <= 0)
	{
		RECT rWnd = rSibling;
		if (hWndToDock)
			GetWindowRect(hWndToDock, &rWnd);
		if (newItemWidth <= 0)
			newItemWidth = MIN(RectWidth(rWnd), fround(RectWidth(rSibling) / 1.2f));
		if (newItemHeight <= 0)
			newItemHeight = MIN(RectHeight(rWnd), fround(RectHeight(rSibling) / 1.2f));
	}
	newItemWidth = CLAMP(newItemWidth, newItemMinWidth, spareWidth);
	newItemHeight = CLAMP(newItemHeight, newItemMinHeight, spareHeight);

	// TODO: Send the window a message to get its minimum size.
	newItemMinWidth = MAX(newItemMinWidth, 30);
	newItemMinHeight = MAX(newItemMinHeight, 25);

	DHLeafNode* pNode = new DHLeafNode;

	pNode->minSize.cx = newItemMinWidth;
	pNode->minSize.cy = newItemMinHeight;
	pNode->resizability = resizability;
	pNode->dwStyle = dwNodeStyle;
	pNode->userData = userData;

	DockHost_InsertNode(hDockHost, pNode, CONV(hSibling), dockLocation, newItemWidth, newItemHeight);

	// Now make the window a child of the control and send notification messages.
	if (hWndToDock)
		DockHost_SetNodeWindow(hDockHost, CONV(pNode), hWndToDock);

	DockHost_Notify(hDockHost, DHN_ITEMDOCK, CONV(pNode));

	InvalidateRect(hDockHost, NULL, FALSE);
	UpdateWindow(hDockHost);
	return CONV(pNode);
}


//mouseing & dock host
LRESULT DockHost_Notify(HWND hDockHost, UINT code, HDHNODE hNode)
{
	HWND hWnd = DockHost_GetNodeWindow(hNode);

	NM_DOCKHOST nm;
	nm.hdr.code = code;
	nm.hdr.hwndFrom = hDockHost;
	nm.hdr.idFrom = GetWindowLong(hDockHost, GWL_ID);
	nm.itemWnd = hWnd;

	LRESULT ret = 0;
	if (hWnd)
	{
		ret = SendMessage(hWnd, WM_NOTIFY, 0, (LPARAM)&nm);
		if (ret)
			return ret;
	}
	return SendMessage(GetParent(hDockHost), WM_NOTIFY, 0, (LPARAM)&nm);
}

BOOL DockHost_GetDividerPositionRange(HWND hDockHost, HDHNODE hDivider, INT* pMin, INT* pMax)
{
	if (DockHost_GetNodeType(hDivider) == DH_LEAF)
		return FALSE;

	DHDividerNode* pDivider = GetDividerNode(hDivider);

	RECT rNode;
	DockHost_GetNodeRect(hDockHost, hDivider, &rNode);
	SIZE minSizeA = pDivider->ChildA()->GetMinSize();
	SIZE minSizeB = pDivider->ChildB()->GetMinSize();
	if (DockHost_GetNodeType(hDivider) == DH_DIVIDER_H)
	{
		*pMin = minSizeA.cy;
		*pMax = RectHeight(rNode) - minSizeB.cy - DH_DIVIDER_HEIGHT;
	}
	else
	{
		*pMin = minSizeA.cx;
		*pMax = RectWidth(rNode) - minSizeB.cx - DH_DIVIDER_WIDTH;
	}
	return *pMax > *pMin;
}

// This implements the algorithm that the control uses to divide space between both sides of a divider
// when a resizing operation is taking place. Generally, if one side has a higher resizability, it is
// resized and the other side remains unchanged. Otherwise, the difference in space is divided equally
// between both sides.
VOID DockHost_DivideSpace(INT length,
	INT minLengthA,
	INT minLengthB,
	INT resizabilityA,
	INT resizabilityB,
	LONG& lengthA, // INOUT - at entry, it's the current length of A.
	LONG& lengthB // INOUT - at entry, it's the current length of B.
	)
{
	if (length == lengthA + lengthB)
		return; // Nothing to do.

	if (lengthA <= 0 || lengthB <= 0)
	{
		lengthA = length / 2;
		lengthB = length - lengthA;
	}

	if (resizabilityB > resizabilityA)
	{
		// Try to maintain lengthA.
		lengthA = CLAMP(lengthA, minLengthA, length - minLengthB);
		lengthB = length - lengthA;
	}
	else if (resizabilityB < resizabilityA)
	{
		// Try to maintain lengthB.
		lengthB = CLAMP(lengthB, minLengthB, length - minLengthA);
		lengthA = length - lengthB;
	}
	else
	{
		// Equal resizability.
		int deltaLength = length - (lengthA + lengthB);
		lengthA = lengthA + deltaLength / 2;
		if (deltaLength / 2 == 0)
			lengthA += deltaLength * length % 2;
		lengthA = CLAMP(lengthA, minLengthA, length - minLengthB);
		lengthB = length - lengthA;
	}
}

//=========================================================================================
//Get
//=========================================================================================
//실제로 실행이 안된다... 왜???/?? 
VOID DockHost_GetNodeInfo(HWND hDockHost, HDHNODE hNode, DHNodeInfo* pNI)
{
	DHNode* pNode = CONV(hNode);
	DHLeafNode* pLeafNode = GetLeafNode(hNode);
	DHDividerNode* pDividerNode = GetDividerNode(hNode);

	pNI->nodeType = pNode->type;
	DockHost_GetNodeRect(hDockHost, hNode, &pNI->rect);
	pNI->parent = CONV(DHNodeTree::Parent(pNode));
	pNI->childA = CONV(DHNodeTree::FirstChild(pNode));
	pNI->childB = CONV(DHNodeTree::LastChild(pNode));

	if (pDividerNode)	DockHost_GetDividerRect(hDockHost, hNode, &pNI->dividerRect);
	else				SetRect(&pNI->dividerRect, 0, 0, 0, 0);

	pNI->resizability = pNode->GetResizability();

	if (pLeafNode)		pNI->hWnd = pLeafNode->hWnd;
	else				pNI->hWnd = NULL;
	if (pLeafNode)		pNI->dwStyle = pLeafNode->dwStyle;
	else				pNI->dwStyle = 0;
	if (pLeafNode)		pNI->userData = pLeafNode->userData;
	else				pNI->userData = 0;
}

//여기 내용을 없애면 윈도우창 위의 bar 가 사라짐 
BOOL DockHost_GetLeafNodeRects(HWND hDockHost, HDHNODE hNode, RECT* prNode, RECT* prClient, RECT* prTab,
	RECT* prCaptionBar, RECT* prClose, RECT* prText, RECT* prIcon)
{
	if (DockHost_GetNodeType(hNode) != DH_LEAF)
		return FALSE;

	RECT rNode;
	DockHost_GetNodeRect(hDockHost, hNode, &rNode);

	if (prNode) *prNode = rNode;

	RECT rCaptionBar = { 0, 0, 0, 0 };
	if (DockHost_GetNodeWindow(hNode) && 0 == (DockHost_GetNodeStyle(hNode) & DHNS_NO_TEAROFF))
	{
		CalculateDockedCaptionRects(DockHost_GetNodeWindow(hNode), rNode, &rCaptionBar,
			prText, prIcon, prClose);
	}
	else
	{
		if (prCaptionBar) SetRect(prCaptionBar, 0, 0, 0, 0);
		if (prText) SetRect(prText, 0, 0, 0, 0);
		if (prIcon) SetRect(prIcon, 0, 0, 0, 0);
		if (prClose) SetRect(prClose, 0, 0, 0, 0);
	}

	RECT rClient = rNode;
	rClient.top += RectHeight(rCaptionBar);

	if (prClient) *prClient = rClient;
	if (prCaptionBar) *prCaptionBar = rCaptionBar;

	return TRUE;
}


//애들 위치값 세팅 해줄때 쓴다... ! 완벽하게 이해는 불가했다.. 
VOID DockHost_GetDividerRect(HWND hDockHost, HDHNODE hDivider, RECT* prDivider)
{

	//리프 노드일때 0.0.0으로 초기화 하는건데 실제로 들어가진 않는데..? 어째서 일까??
	if (DockHost_GetNodeType(hDivider) == DH_LEAF)
	{
		SetRect(prDivider, 0, 0, 0, 0);
		return;
	}
	DockHost_GetNodeRect(hDockHost, hDivider, prDivider);
	if (DockHost_GetNodeType(hDivider) == DH_DIVIDER_V)
	{
		prDivider->left += DockHost_GetDividerPosition(hDivider);
		prDivider->right = prDivider->left + DH_DIVIDER_WIDTH;
	}
	else
	{
		prDivider->top += DockHost_GetDividerPosition(hDivider);
		prDivider->bottom = prDivider->top + DH_DIVIDER_HEIGHT;
	}
}

VOID DockHost_GetNodeRect(HWND hDockHost, HDHNODE hNode, RECT* prNode)
{
	HDHNODE hParent = DockHost_GetNodeParent(hNode);
	if (!hParent)
	{
		//부모가 있으면 부모의 사이즈를 받아온다. 
		GetClientRect(hDockHost, prNode);
		return;
	}

	RECT rDivider;
	DockHost_GetDividerRect(hDockHost, hParent, &rDivider);
	DockHost_GetNodeRect(hDockHost, hParent, prNode);
	if (DockHost_GetNodeType(hParent) == DH_DIVIDER_H)
	{
		if (hNode == DockHost_GetNodeChild1(hParent))
			prNode->bottom = rDivider.top; // The node is above a horizontal divider.
		else
			prNode->top = rDivider.bottom; // The node is below a horizontal divider.
	}
	else
	{
		if (hNode == DockHost_GetNodeChild1(hParent))
			prNode->right = rDivider.left; // The node is to the left of a vertical divider.
		else
			prNode->left = rDivider.right; // The node is to the right of a vertical divider.
	}
}
//=========================================================================================
//SET
//=========================================================================================


BOOL DockHost_SetDividerPosition(HWND hDockHost, HDHNODE hDivider, INT pos, BOOL bForce, BOOL bUpdate)
{
	DHDividerNode* pDivider = GetDividerNode(hDivider);
	if (!pDivider)
		return FALSE;

	if (DockHost_GetDividerPosition(hDivider) == pos)
		return TRUE;

	INT minPos, maxPos;
	DockHost_GetDividerPositionRange(hDockHost, hDivider, &minPos, &maxPos);
	if (!bForce && (minPos >= maxPos || pos < minPos || pos > maxPos))
		return FALSE;

	pDivider->pos = pos;

	RECT rDivider;
	DockHost_GetDividerRect(hDockHost, hDivider, &rDivider);
	InvalidateRect(hDockHost, &rDivider, FALSE);

	DockHost_OnNodeResized(hDockHost, CONV(pDivider->ChildA()));
	DockHost_OnNodeResized(hDockHost, CONV(pDivider->ChildB()));

	if (bUpdate)
		UpdateWindow(hDockHost);

	return TRUE;
}


VOID PaintLeafNode(HWND hDockHost, DHLeafNode* pNode, HDC hDC
	//, const RECT* prcPaint // TODO: Use this to optimize.
	)
{
	HWND hWnd = DockHost_GetNodeWindow(CONV(pNode));
	RECT rNode;
	DockHost_GetNodeRect(hDockHost, CONV(pNode), &rNode);
	if (hWnd == NULL)
	{
		// A node with no window in it. Fill it with a constant color.
		RECT rIntersection;
		DockHost_GetNodeRect(hDockHost, CONV(pNode), &rNode);
		if (!IsRectEmpty(&rIntersection))
			FillRect(hDC, &rIntersection, (HBRUSH)(COLOR_APPWORKSPACE + 1));
	}
	else //if( 0 == (DockHost_GetNodeStyle( CONV(pNode) ) & DHNS_NO_TEAROFF) )
	{
		BOOL bCloseHot = DockHost_GetHotCloseButton().hNode == CONV(pNode);
		// TODO: Pass prcPaint to DrawDockedCaption().
		::DrawDockedCaption(hWnd, hDC, rNode, TRUE, bCloseHot);
	}
	//ValidateRect( hDockHost, &rNode ); // TODO: Useful?
}

VOID PaintDividerNode(HWND hDockHost, DHDividerNode* pNode, HDC hDC)
{
	RECT rDivider;
	DockHost_GetDividerRect(hDockHost, CONV(pNode), &rDivider);
	//FillRect( hDC, &rDivider, (HBRUSH)(COLOR_3DFACE+1) );
	FillRect(hDC, &rDivider,(HBRUSH)InterpolateCOLORREF(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_MENUHILIGHT), 0.25f));
	if (pNode->type == DH_DIVIDER_V)
	{
		rDivider.left += RectWidth(rDivider) / 2;
		DrawEdge(hDC, &rDivider, EDGE_ETCHED, BF_LEFT);
	}
	else
	{
		rDivider.top += RectHeight(rDivider) / 2;
		DrawEdge(hDC, &rDivider, EDGE_ETCHED, BF_TOP);
	}
}



VOID DockHost_OnNodeResized(HWND hDockHost, HDHNODE hNode)
{
	RECT rNode;
	SIZE oldSize, newSize;
	DockHost_GetNodeRect(hDockHost, hNode, &rNode);
	newSize.cx = RectWidth(rNode);
	newSize.cy = RectHeight(rNode);
	oldSize = CONV(hNode)->cachedSize;
	CONV(hNode)->cachedSize = newSize;

	if (GetLeafNode(hNode))
	{
		InvalidateRect(hDockHost, &rNode, TRUE);
		DockHost_LayoutLeafWindows(hDockHost, hNode);
		UpdateWindow(hDockHost);
		return;
	}
	DHDividerNode* pDivider = GetDividerNode(hNode);
	assert(pDivider);


	SIZE sizeA = pDivider->ChildA()->cachedSize;
	SIZE sizeB = pDivider->ChildB()->cachedSize;

	INT dividerWidth = pDivider->type == DH_DIVIDER_V ? DH_DIVIDER_WIDTH : 0;
	INT dividerHeight = pDivider->type == DH_DIVIDER_H ? DH_DIVIDER_HEIGHT : 0;

	// Calculate the new width values for both sides.
	DockHost_DivideSpace(newSize.cx - dividerWidth,
		pDivider->ChildA()->GetMinSize().cx,
		pDivider->ChildB()->GetMinSize().cx,
		pDivider->ChildA()->GetResizability(),
		pDivider->ChildB()->GetResizability(),
		sizeA.cx, sizeB.cx);

	// Calculate the new height values for both sides.
	DockHost_DivideSpace(newSize.cy - dividerHeight,
		pDivider->ChildA()->GetMinSize().cy,
		pDivider->ChildB()->GetMinSize().cy,
		pDivider->ChildA()->GetResizability(),
		pDivider->ChildB()->GetResizability(),
		sizeA.cy, sizeB.cy);

	if (pDivider->type == DH_DIVIDER_H)
		DockHost_SetDividerPosition(hDockHost, hNode, sizeA.cy, TRUE, FALSE);
	else
		DockHost_SetDividerPosition(hDockHost, hNode, sizeA.cx, TRUE, FALSE);


	RECT rDivider;
	DockHost_GetDividerRect(hDockHost, hNode, &rDivider);
	InvalidateRect(hDockHost, &rNode, TRUE);

	DockHost_OnNodeResized(hDockHost, DockHost_GetNodeChild1(hNode));
	DockHost_OnNodeResized(hDockHost, DockHost_GetNodeChild2(hNode));

	UpdateWindow(hDockHost);
}

LRESULT CALLBACK DockHostWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_NCCREATE)
	{
		TDockHost* pDH = new TDockHost(hWnd);
		SetControlData(hWnd, pDH);
		return TRUE;
	}

	TDockHost* pDH = GetControlData(hWnd);
	if (!pDH)
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	BOOL bHandled = FALSE;
	//다른 호스트의 마우스 입력을 받으면 딴 애들은 못 받게 해줌 
	LRESULT ret = DockHostMouseMsgProc(pDH, Msg, wParam, lParam, bHandled);
	if (bHandled)
		return ret;

	switch (Msg)
	{
	case WM_NCDESTROY:
		delete pDH;
		break;


	case WM_ERASEBKGND:
		return 1;


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (DockHost_IsEmpty(hWnd))
			//FillRect( hdc, &ps.rcPaint, (HBRUSH)(COLOR_3DFACE+1) );
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_APPWORKSPACE + 1));
		else
		{
			for (HDHNODE hNode = DockHost_GetRootNode(hWnd);
				hNode != NULL;
				hNode = DockHost_GetNextNode(hWnd, hNode))
			{
				if (GetLeafNode(hNode))
					PaintLeafNode(hWnd, GetLeafNode(hNode), hdc);
				else
					PaintDividerNode(hWnd, GetDividerNode(hNode), hdc);
			}
		}
		EndPaint(hWnd, &ps);
	}
		return 0;



	case WM_GETMINMAXINFO:
		// Apparently this message is sent to the message processor even before the
		// WM_NCCREATE message where pDH is initialized. So we have to check pDH for NULL.
		if (pDH != NULL && !DockHost_IsEmpty(hWnd))
		{
			MINMAXINFO* pMMI = (MINMAXINFO*)lParam;
			SIZE minSize = CONV(DockHost_GetRootNode(hWnd))->GetMinSize();

			DWORD style = GetWindowLong(hWnd, GWL_STYLE);
			RECT r = { 0, 0, minSize.cx, minSize.cy };
			AdjustWindowRect(&r, style, FALSE);

			pMMI->ptMinTrackSize.x = RectWidth(r);
			pMMI->ptMinTrackSize.y = RectHeight(r);
		}
		break;


		/*case WM_RBUTTONDOWN:
		DockHost_PrintInfo( hWnd );
		break;*/




	case WM_SIZE:
		if (DockHost_GetRootNode(hWnd))
		{
			RECT rect;
			DockHost_GetNodeRect(hWnd, DockHost_GetRootNode(hWnd), &rect);
			rect.right = LOWORD(lParam);
			rect.bottom = HIWORD(lParam);
			//return FALSE != DockHost_Resize( pDH, CONV(DockHost_GetRootNode( hWnd )), rect, FALSE );
			DockHost_OnNodeResized(hWnd, DockHost_GetRootNode(hWnd));//, rect );
		}
		break;

	} // switch( Msg )


	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

//이게모 지??
VOID DockHost_SetNodeResizability(HDHNODE hNode, INT newResizability, BOOL bRelative)
{
	if (!GetLeafNode(hNode))
		return;
	if (bRelative)
		GetLeafNode(hNode)->resizability += newResizability;
	else
		GetLeafNode(hNode)->resizability = newResizability;
}


VOID DockHost_RemoveNode(HWND hDockHost, DHNode* pNode)
{
	HDHNODE hNode = CONV(pNode);
	DHNodeTree& nodeTree = GetControlData(hDockHost)->GetNodeTree();
	DHNode* pParent = CONV(DockHost_GetNodeParent(hNode));
	DHNode* pSibling = CONV(DockHost_GetNodeSibling(hNode));

	RECT rUpdate;
	DockHost_GetNodeRect(hDockHost, pParent ? CONV(pParent) : hNode, &rUpdate);
	InvalidateRect(hDockHost, &rUpdate, TRUE);

	nodeTree.Remove(pNode);

	if (pParent)
	{
		assert(pSibling); // It's a binary tree. If it has a parent, it should have a sibling.

		nodeTree.Remove(pSibling);
		nodeTree.Add(pSibling, DHNodeTree::Parent(pParent), pParent);
		nodeTree.Remove(pParent);
		delete pParent;
		DockHost_OnNodeResized(hDockHost, CONV(pSibling));
	}
}


static ATOM DockHostAtom = NULL;
static BOOL DockHostInited = FALSE;
BOOL DockHost_Init(HINSTANCE hInstance)
{
	if (DockHostInited)
		return DockHostAtom != NULL;
	DockHostInited = TRUE;

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DockHostWndProc;
	wcex.cbWndExtra = sizeof(TDockHost*);
	wcex.hInstance = hInstance;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszClassName = WC_DOCKHOST;
	DockHostAtom = RegisterClassEx(&wcex);
	if (NULL == DockHostAtom)
		return FALSE;

	return TRUE;
}

// Release resources used by and unregister the DockHost window class.
VOID DockHost_Uninit(HINSTANCE hInstance)
{
	if (!DockHostInited)
		return;
	DockHostInited = FALSE;

	if (DockHostAtom != NULL)
	{
		UnregisterClass((TCHAR*)DockHostAtom, hInstance);
		DockHostAtom = NULL;
	}
}


BOOL DockHost_IsDockHost(HWND hWnd)
{
	if (!IsWindow(hWnd)) return FALSE;
	WINDOWINFO wi = { 0 };
	GetWindowInfo(hWnd, &wi);
	return wi.atomWindowType == DockHostAtom;
}


BOOL DockHost_Undock(HWND hDockHost, HDHNODE hNode, HWND hNewWindowOwner)
{
	if (!hNode || CONV(hNode)->type != DH_LEAF)
		return FALSE;

	if (0 != DockHost_Notify(hDockHost, DHN_ITEMUNDOCK, hNode))
		return FALSE;

	CONV(hDockHost)->pLastHitNode = NULL;

	DockHost_SetNodeWindow(hDockHost, hNode, NULL);
	DHNode* pNode = CONV(hNode);
	DockHost_RemoveNode(hDockHost, pNode);
	delete pNode;

	UpdateWindow(hDockHost);
	return TRUE;
}


