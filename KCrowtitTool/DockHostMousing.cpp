#include "stdafx.h"
#include "DockHostImpl.h"

HDHNODE hClickedNode = NULL;
POINT ptMouseStart = { 0, 0 };
POINT ptPrevCursor = { 0, 0 };
BOOL bDetectDrag = FALSE;


static CloseButton hotCloseButton;
const CloseButton& DockHost_GetHotCloseButton() { return hotCloseButton; }


//클로즈 버튼 세팅 
VOID SetHotCloseButton(HWND hWnd, HDHNODE hNode, const RECT* prClose, BOOL bUpdate = TRUE)
{
	if (hotCloseButton.hNode == hNode)
		return; // Nothing to do.

	// If there was a hot button, make it cold.
	if (hotCloseButton.hNode)
		InvalidateRect(hotCloseButton.hWnd, &hotCloseButton.rClose, FALSE);

	hotCloseButton.hWnd = hWnd;
	hotCloseButton.hNode = hNode;
	if (hNode)
	{
		hotCloseButton.rClose = *prClose;
		InvalidateRect(hotCloseButton.hWnd, &hotCloseButton.rClose, FALSE);
	}
	if (bUpdate)
		UpdateWindow(hWnd);
}


VOID DividerDragging(TDockHost* pDH, DHDividerNode* pDragDivider, POINT ptCursor)
{
	INT d;
	if (pDragDivider->type == DH_DIVIDER_H)
		d = ptCursor.y - ptPrevCursor.y;
	else
		d = ptCursor.x - ptPrevCursor.x;
	ptPrevCursor = ptCursor;

	if (d == 0)
		return;

	DockHost_SetDividerPosition(pDH->hWnd, CONV(pDragDivider), DockHost_GetDividerPosition(CONV(pDragDivider)) + d, FALSE);
}


LRESULT DockHostMouseMsgProc(TDockHost* pDH, UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWnd = pDH->hWnd;

	switch (Msg)
	{
	case WM_LBUTTONDOWN:
	{
		bHandled = TRUE;
		POINT ptCursor = { LOWORD(lParam), HIWORD(lParam) };
		ptMouseStart = ptCursor;
		ptPrevCursor = ptCursor;

		//이 DockHost Hit Test가도킹이 되어 있는 상태에서 마우스를 클릭하면 
		//도킹된 윈도우를 선택할 수 있도록 해준다. 
		hClickedNode = DockHost_Hit(hWnd, ptCursor, NULL);

		if (!hClickedNode)
			break;

		// 프로그램을 하다보면 마우스 커서 좌표가 활성화된 윈도우 영역을 벗어나더라도 
		//지속적으로 마우스 메시지를 받아야 하는 경우(그림판, 객체 에디트 같은 프로그램 작성시)가 발생하는데,
		//이럴 때 사용하는 함수가 SetCapture 함수
		SetCapture(hWnd);

		// 유저가 CloseButton을 눌렀을 때.
		if (DockHost_GetHotCloseButton().hNode == hClickedNode &&
			GetLeafNode(hClickedNode) &&
			GetLeafNode(hClickedNode)->hWnd != NULL)
		{
			// 알림 창을 허용하는 경우, 창을 해제하고 닫습니다.
			if (0 == DockHost_Notify(hWnd, DHN_CLOSE, hClickedNode))
			{
				HWND hClientWnd = GetLeafNode(hClickedNode)->hWnd;
				DockHost_Undock(hWnd, hClickedNode);
				SendMessage(hClientWnd, WM_CLOSE, 0, 0);
				hClickedNode = NULL;
			}
			break;
		}

		// 사용자가 캡션 영역을 선택 할경우, 그것에 대한 대기하는 플래그를 설정.
		if (CONV(hClickedNode)->type == DH_LEAF &&
			GetLeafNode(hClickedNode) && GetLeafNode(hClickedNode)->hWnd != NULL)
		{
			RECT rCaption, rNode;
			DockHost_GetNodeRect(pDH->hWnd, hClickedNode, &rNode);
			//도킹된 캡션의 RECT 를 계싼 
			CalculateDockedCaptionRects(GetLeafNode(hClickedNode)->hWnd, rNode, &rCaption, NULL, NULL, NULL);
			//캡션 영역에 들어가면 플래그 true;
			if (PtInRect(&rCaption, ptCursor))
			{
				bDetectDrag = TRUE;
				break;
			}
		}
	}
		break;



	case WM_LBUTTONUP:
		bHandled = TRUE;
		if (GetCapture() == hWnd)
			ReleaseCapture();
		hClickedNode = NULL;
		bDetectDrag = FALSE;
		break;



	case WM_CAPTURECHANGED:
		bHandled = TRUE;
		if ((HWND)lParam == hWnd) // Don't do anything if it's us who's gaining capture.
			break;
		// Dragging has ended.
		hClickedNode = NULL;
		bDetectDrag = FALSE;
		break;



	case WM_MOUSEMOVE:
	{
		bHandled = TRUE;

		// Ensure we receive a WM_MOUSELEAVE message.
		if (0 == (wParam & MK_LBUTTON))
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
		}

		POINT ptCursor = { LOWORD(lParam), HIWORD(lParam) };

		// 분할 드래그 처리 
		if (GetDividerNode(hClickedNode))
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			DividerDragging(pDH, GetDividerNode(hClickedNode), pt);
			break;
		}

		// Process caption area tear-off.
		if (hClickedNode && bDetectDrag)
		{
			if (0 == (wParam & MK_LBUTTON))
			{
				OutputDebugString(TEXT(__FUNCTION__)TEXT(": Unexpected program state!"));
				DebugBreak();
			}
			if (abs(ptCursor.x - ptMouseStart.x) >= GetSystemMetrics(SM_CXDRAG) ||
				abs(ptCursor.y - ptMouseStart.y) >= GetSystemMetrics(SM_CYDRAG))
			{
				bDetectDrag = FALSE;
				if (0 == DockHost_Notify(hWnd, DHN_TEAROFF, hClickedNode))
				{
					// If the notified windows do not process the notification themselves,
					// undock the corresponding window, and allow the user to continue dragging it.
					HWND hClientWnd = DockHost_GetNodeWindow(hClickedNode);
					DockHost_Undock(hWnd, hClickedNode, NULL);
					SetCapture(NULL);
					if (hClientWnd)
					{
						RECT rWnd;
						GetWindowRect(hClientWnd, &rWnd);
						POINT ptSSCursor = GetCursorPos();
						OffsetRect(&rWnd, (ptSSCursor.x - rWnd.left) - RectWidth(rWnd) / 2, ptSSCursor.y - rWnd.top);
						MoveWindow(hClientWnd, rWnd.left, rWnd.top, RectWidth(rWnd), RectHeight(rWnd), FALSE);
						SendMessage(hClientWnd, WM_SYSCOMMAND, SC_MOVE, 0);
					}
				}
				hClickedNode = NULL;
				break;
			}
		}


		HDHNODE hNode = DockHost_Hit(hWnd, ptCursor, NULL);


		// Do hot-tracking for the close button in the fake caption bar.
		//가짜 캡션 표시 줄에있는 닫기 버튼 핫 추적 작업을 수행합니다.
		BOOL bCloseHot = FALSE;
		if (GetLeafNode(hNode) && GetLeafNode(hNode)->hWnd)
		{
			RECT rClose, rNode;
			DockHost_GetNodeRect(pDH->hWnd, hNode, &rNode);
			CalculateDockedCaptionRects(GetLeafNode(hNode)->hWnd, rNode, NULL, NULL, NULL, &rClose);
			if (PtInRect(&rClose, ptCursor))
			{
				bCloseHot = TRUE;
				SetHotCloseButton(hWnd, hNode, &rClose, TRUE);
			}
		}
		if (!bCloseHot)
			SetHotCloseButton(hWnd, NULL, NULL, TRUE);


		// Set the cursor to a sizing cursor if we're over a divider.
		//아직은 잘 모르겠군 
		if (!hNode || CONV(hNode)->type == DH_LEAF)
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		else if (CONV(hNode)->type == DH_DIVIDER_H)
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		else if (CONV(hNode)->type == DH_DIVIDER_V)
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
		break;

		//마우스가 해당 윈도우를 떠날때 발생함..!
	case WM_MOUSELEAVE:
		SetHotCloseButton(hWnd, NULL, NULL, TRUE);
		break;
	}

	if (bHandled)
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	return 0;
}