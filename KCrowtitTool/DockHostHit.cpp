#include "stdafx.h"
#include "DockHostImpl.h"
#include "CommCtrl.h"	//TTCHITTESTINFO 를 사용하기 위함

//첫 호출
HDHNODE DockHost_Hit(HWND hDockHost, POINT pt, DH_HitInfo* pHTI)
{
	HDHNODE hNode = DockHost_Hit(hDockHost, pt);
	if (pHTI == NULL)
		return hNode;

	pHTI->hNode = hNode;
	pHTI->iTab = -1;
	pHTI->component = hNode ? DHHT_CLIENT : DHHT_NOTHING;
	pHTI->captionBarComponent = DHHT_CB_NOTHING;
	if (hNode == NULL || DockHost_GetNodeType(hNode) != DH_LEAF)
		return hNode;

	RECT rTab, rCaptionBar, rCloseButton, rIcon;
	DockHost_GetLeafNodeRects(hDockHost, hNode, NULL, NULL, &rTab, &rCaptionBar, &rCloseButton, NULL, &rIcon);

	//캡션 바 안에 마우스가 클릭되면 ㅋㅋ 이군 
	if (PtInRect(&rCaptionBar, pt))
	{
		pHTI->component = DHHT_CAPTION_BAR;
		pHTI->captionBarComponent = DHHT_CB_BAR;
		if (PtInRect(&rCloseButton, pt))
			pHTI->captionBarComponent = DHHT_CB_CLOSE;
		else if (PtInRect(&rIcon, pt))
			pHTI->captionBarComponent = DHHT_CB_ICON;
		return hNode;
	}
	//이부분 실행이 안되는거 같은데 넣어둔 이유는???????????????????????
	DHNodeInfo nodeInfo;
	DockHost_GetNodeInfo(hDockHost, hNode, &nodeInfo);
	if (nodeInfo.hTab && PtInRect(&rTab, pt))
	{
		pHTI->component = DHHT_TAB;
		TCHITTESTINFO tchti;
		tchti.pt = PTClientToScreen(hDockHost, pt);
		pHTI->iTab = TabCtrl_HitTest(nodeInfo.hTab, &tchti);
		return hNode;
	}
	return hNode;
}

// Performs a hit test. The point is assumed to be in client coordinates.
HDHNODE DockHost_Hit(HWND hDockHost, POINT pt)
{
	if (CONV(hDockHost)->pLastHitNode)
	{
		DHNode* pNode = CONV(hDockHost)->pLastHitNode;
		RECT r = { 0, 0, 0, 0 };
		if (pNode->type == DH_LEAF)
			DockHost_GetNodeRect(hDockHost, CONV(pNode), &r);
		else
			DockHost_GetDividerRect(hDockHost, CONV(pNode), &r);
		if (PtInRect(&r, pt))
			return CONV(pNode);
	}


	RECT rClient;
	GetClientRect(hDockHost, &rClient);
	if (!PtInRect(&rClient, pt))
		return NULL;


	HDHNODE hNode = DockHost_GetRootNode(hDockHost);
	while (hNode)
	{
		RECT r;
		DockHost_GetNodeRect(hDockHost, hNode, &r);
		if (DockHost_GetNodeType(hNode) == DH_LEAF)
		{
			if (PtInRect(&r, pt))
			{
				CONV(hDockHost)->pLastHitNode = CONV(hNode);
				return hNode;
			}
			//return NULL;
			break;
		}

		// It's a divider node - if it's on the divider itself, return the divider.
		DockHost_GetDividerRect(hDockHost, hNode, &r);
		if (PtInRect(&r, pt))
			return hNode;

		// See on which side of the divider the point is and set the node to that child.
		HDHNODE a = DockHost_GetNodeChild1(hNode);
		HDHNODE b = DockHost_GetNodeChild2(hNode);
		DockHost_GetNodeRect(hDockHost, a, &r);
		if (PtInRect(&r, pt))
		{
			hNode = a;
			continue;
		}
		DockHost_GetNodeRect(hDockHost, b, &r);
		if (PtInRect(&r, pt))
		{
			hNode = b;
			continue;
		}
		OutputDebugStringA(__FUNCTION__"(): Unexpected condition - debug!");
		return NULL;
	}

	CONV(hDockHost)->pLastHitNode = NULL;
	return NULL;
}

