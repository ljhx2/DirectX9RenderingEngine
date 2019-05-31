#include"stdafx.h"
#include"cHierarchyView.h"
#include "cUtil.h"

HFONT g_hFont;
cHierarchyView::cHierarchyView()
{
}
cHierarchyView::~cHierarchyView()
{

}

BOOL cHierarchyView::Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hwnd;
	WC_CUSTOMWINDOW = name;
	// Register the color box window class.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)cHierarchyView::WindowProc;
	wcex.cbWndExtra = sizeof(TWindowInfo*);
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = WC_CUSTOMWINDOW;
	if (0 == RegisterClassEx(&wcex))
	{
		OutputDebugString(TEXT("Failed to register the custom window class!"));
		return FALSE;
	}
	return TRUE;
}
VOID cHierarchyView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
	DeleteObject(g_hFont);
	g_hFont = NULL;
}
void cHierarchyView::ResetTreeNode(ENGINE::GameObject* obj)
{
	//LOAD SCENE을 위해 트리 내용 reset
	TreeView_DeleteItem(GetDlgItem(g_hwndHierarchy, ID_TREE), NULL);
	InsertTreeNode((HTREEITEM)0, obj);
	SelectTreeNode(obj);

}
void cHierarchyView::InsertTreeNode(HTREEITEM pNode, ENGINE::GameObject* obj)
{
	TV_INSERTSTRUCT TI;
	HTREEITEM Node;

	TI.hParent = pNode;
	TI.hInsertAfter = TVI_LAST;
	TI.item.mask = TVIF_TEXT | TVIF_PARAM;
	TI.item.pszText = UTIL::ConstWcharToWchar(obj->GetName().c_str());
	TI.item.lParam = (LPARAM)obj;
	
	Node = TreeView_InsertItem(GetDlgItem(g_hwndHierarchy, ID_TREE), &TI);
	m_mapNode.insert(std::make_pair(obj, Node));

	for (size_t i = 0; i < obj->GetVecChild().size(); i++)
	{
		InsertTreeNode(Node, obj->GetVecChild().at(i));
	}

}
//노드추가
HTREEITEM cHierarchyView::GetTreeNode(ENGINE::GameObject* obj)
{
	if (obj == NULL) return NULL;
	
	std::map<ENGINE::GameObject*, HTREEITEM>::iterator iter;
	iter = m_mapNode.find(obj);
	
	if (iter == m_mapNode.end()) return NULL;
	else return iter->second;
}
//Node선택된것으로 표시
void cHierarchyView::SelectTreeNode(HTREEITEM Node)
{
	TreeView_SelectItem(GetDlgItem(g_hwndHierarchy, ID_TREE), Node);
}
//obj 선택된것으로 표시
void cHierarchyView::SelectTreeNode(ENGINE::GameObject* obj)
{
	HTREEITEM Node = GetTreeNode(obj);
	TreeView_SelectItem(GetDlgItem(g_hwndHierarchy, ID_TREE), Node);
	InspectorView->SetSelectGameObject(obj);
}
//현재 선택되있는 노드(선택된 것이 없으면 NULL리턴)
HTREEITEM cHierarchyView::NowSelectedTreeNode()
{
	return TreeView_GetSelection(GetDlgItem(g_hwndHierarchy, ID_TREE));
}
//노드삭제
void cHierarchyView::DeleteTreeNode(HTREEITEM Node)
{
	if (Node == NULL) return;
	//GameObject추출
	ENGINE::GameObject* obj = PickGameObjectFromTreeNode(Node);

	//map에서 제거
	std::map<ENGINE::GameObject*, HTREEITEM>::iterator iter;
	iter = m_mapNode.find(obj);
	if (iter != m_mapNode.end())
	{
		m_mapNode.erase(iter);
	}

	//GameObject삭제
	ENGINE::DeleteGameObject(obj);
	//Node삭제
	TreeView_DeleteItem(GetDlgItem(g_hwndHierarchy, ID_TREE), Node);
}
//노드에서 GameObject가져오기
ENGINE::GameObject* cHierarchyView::PickGameObjectFromTreeNode(HTREEITEM Node)
{
	if (Node == NULL) return NULL;

	TVITEM tvi;
	tvi.hItem = Node;
	tvi.mask = TVIF_PARAM;
	TreeView_GetItem(GetDlgItem(g_hwndHierarchy, ID_TREE), &tvi);
	return (ENGINE::GameObject*)tvi.lParam;
}

void cHierarchyView::Setup()
{
	RECT rClient;
	GetClientRect(g_hwndHierarchy, &rClient);
	
	//트리뷰 생성
	CreateWindow(WC_TREEVIEW, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER |
		TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_EDITLABELS, 
		10, 10, rClient.right - 30, rClient.bottom - 80, g_hwndHierarchy, (HMENU)ID_TREE, m_hInstance, NULL);


	//트리 내용 채우기
	InsertTreeNode((HTREEITEM)0, ENGINE::GetSceneRoot());

	//Delete버튼생성
	CreateWindow(TEXT("button"), TEXT("Delete"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rClient.right - 190, rClient.bottom - 50, 80, 30, g_hwndHierarchy, (HMENU)ID_BUTTON_DELETE, m_hInstance, NULL);

	////Save버튼생성
	//CreateWindow(TEXT("button"), TEXT("Save"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	//	rClient.left + 40, rClient.bottom - 50, 80, 30, g_hwndHierarchy, (HMENU)ID_BUTTON_SAVE, m_hInstance, NULL);
}

//드래그 & 드롭에 사용되는 전역 변수들
HIMAGELIST himl;
BOOL g_bDrag;
HTREEITEM hDrag;
HTREEITEM hTarget, hOldTarget = NULL;

//트리의 항목 하나를 복사하고 복사한 항목의 핸들을 리턴한다.
HTREEITEM CopyTreeItem(HWND hTree, HTREEITEM hSrc, HTREEITEM hDest)
{
	HTREEITEM hNew;
	TVINSERTSTRUCT TI;
	TVITEMEX TvEx;
	TCHAR Text[256];
	ENGINE::GameObject* pParam;

	TvEx.hItem = hSrc;
	TvEx.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TvEx.pszText = Text;
	TvEx.cchTextMax = 256;
	TreeView_GetItem(hTree, &TvEx);
	TI.itemex.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.itemex.pszText = Text;

	pParam = (ENGINE::GameObject*)TvEx.lParam;
	TI.itemex.lParam = (LPARAM)pParam;
	TI.itemex.iImage = TvEx.iImage;
	TI.itemex.iSelectedImage = TvEx.iSelectedImage;
	TI.hParent = hDest;
	TI.hInsertAfter = TVI_LAST;
	hNew = TreeView_InsertItem(hTree, &TI);

	pParam->SetParent(HierarchyView->PickGameObjectFromTreeNode(hDest));

	return hNew;
}

// 트리의 항목을 차일드까지 한꺼번에 복사한다.
HTREEITEM CopyTreeItems(HWND hTree, HTREEITEM hSrc, HTREEITEM hDest)
{
	HTREEITEM hNew, hChild;

	hNew = CopyTreeItem(hTree, hSrc, hDest);

	hChild = TreeView_GetChild(hTree, hSrc);
	while (hChild != NULL)
	{
		CopyTreeItems(hTree, hChild, hNew);
		hChild = TreeView_GetNextSibling(hTree, hChild);
	}

	return hNew;
}

LRESULT CALLBACK cHierarchyView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// Retrieve the custom window info associated with the window.
	TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);

	// We allocate an instance of our custom window info structure (TWindowInfo) and assign
	// it to the window when the WM_NCCREATE message is received. This is not guaranteed
	// to be the first message received however, so we need to check the return value of GetInfo( HWND )
	// for NULL.
	if (Msg == WM_NCCREATE)
	{
		pInfo = new TWindowInfo;
		SetWindowLongPtr(hWnd, 0, (LONG)pInfo);
	}
	else if (Msg == WM_NCDESTROY)
	{
		//assert( pInfo != NULL );
		delete pInfo;
		pInfo = NULL;
	}

	if (!pInfo)
		return DefWindowProc(hWnd, Msg, wParam, lParam);


	//Drag & Drop 변수
	POINT pt;
	int hotx, hoty;
	RECT rt;
	HTREEITEM hItem;
	TCHAR str[255];
	ENGINE::GameObject* pParam;


	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
	{
		if (GetFocus() != g_hwndHierarchy)
			SetFocus(g_hwndHierarchy);
	}
		break;
	case WM_SIZE:
	{
					RECT rClient;
					GetClientRect(hWnd, &rClient);
					StringCchPrintf(pInfo->text, MAX_PATH, TEXT("Hierarch size: %d x %d"), rClient.right, rClient.bottom);

					MoveWindow(GetDlgItem(g_hwndHierarchy, ID_TREE), 10, 10, rClient.right - 30, rClient.bottom - 80, TRUE);
					MoveWindow(GetDlgItem(g_hwndHierarchy, ID_BUTTON_DELETE), rClient.right - 200, rClient.bottom - 50, 80, 30, TRUE);

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
	}
		break;
	case WM_PAINT:
	{
					 PAINTSTRUCT ps;
					 HDC hdcPaint = BeginPaint(hWnd, &ps);
					 if (!hdcPaint)
						 break;
					 RECT rClient;
					 GetClientRect(hWnd, &rClient);
					 FillRect(hdcPaint, &rClient, pInfo->hBrush);
					 // Draw text
					 {
						 HGDIOBJ hOldFont = NULL;
						 if (g_hFont)
							 hOldFont = SelectObject(hdcPaint, g_hFont);
						 int oldMode = SetBkMode(hdcPaint, TRANSPARENT);
						 COLORREF oldColor = SetTextColor(hdcPaint, 0x000000);
						 size_t len = 0;
						 StringCchLength(pInfo->text, MAX_PATH, &len);
						 DrawText(hdcPaint, pInfo->text, (int)len, &rClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
						 SetBkMode(hdcPaint, oldMode);
						 SetTextColor(hdcPaint, oldColor);
						 if (g_hFont)
							 SelectObject(hdcPaint, hOldFont);
					 }
					 EndPaint(hWnd, &ps);
	}
		break;
	case WM_NOTIFY:
		LPNMHDR hdr;
		HTREEITEM current;
		LPNMTREEVIEW ntv;
		hdr = (LPNMHDR)lParam;
		ntv = (LPNMTREEVIEW)lParam;
		if (hdr->hwndFrom == GetDlgItem(g_hwndHierarchy, ID_TREE))
		{
			switch (hdr->code)
			{
			case TVN_DELETEITEM:
				break;
			case TVN_SELCHANGED:
				current = HierarchyView->NowSelectedTreeNode();
				InspectorView->SetSelectGameObject(HierarchyView->PickGameObjectFromTreeNode(current));
				break;
			case TVN_BEGINDRAG:
				//드래그 이미지를 만든다.
				hDrag = ntv->itemNew.hItem;
				himl = TreeView_CreateDragImage(GetDlgItem(g_hwndHierarchy, ID_TREE), hDrag);

				//핫 스팟 계산
				TreeView_GetItemRect(GetDlgItem(g_hwndHierarchy, ID_TREE), hDrag, &rt, TRUE);
				hotx = ntv->ptDrag.x - rt.left;
				hoty = ntv->ptDrag.y - rt.top;
				ImageList_BeginDrag(himl, 0, hotx, hoty);
				
				//드래그 이미지 그림
				pt.x = ntv->ptDrag.x;
				pt.y = ntv->ptDrag.y;
				ClientToScreen(hWnd, &pt);
				ImageList_DragEnter(NULL, pt.x, pt.y);

				//커서 캡처
				SetCapture(hWnd);
				g_bDrag = TRUE;
		
				return 0;
			}
		}
		break;
	case WM_MOUSEMOVE:
		TVHITTESTINFO hti;

		if (g_bDrag)
		{
			//드래그중인 이미지 그림
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);
			ImageList_DragMove(pt.x, pt.y);

			GetWindowRect(GetDlgItem(g_hwndHierarchy, ID_TREE), &rt);
			ScreenToClient(hWnd, (POINT*)&rt);
			hti.pt.x = LOWORD(lParam) - rt.left;
			hti.pt.y = HIWORD(lParam) - rt.top;
			hTarget = TreeView_HitTest(GetDlgItem(g_hwndHierarchy, ID_TREE), &hti);
			if (hTarget != NULL) {
				ImageList_DragLeave(NULL);
				TreeView_SelectDropTarget(GetDlgItem(g_hwndHierarchy, ID_TREE), hTarget);
				UpdateWindow(GetDlgItem(g_hwndHierarchy, ID_TREE));
				ImageList_DragEnter(NULL, pt.x, pt.y);

				// 드롭 타겟이 바뀌면 다시 1초동안 기다린다.
				if (hTarget != hOldTarget) {
					hOldTarget = hTarget;
					KillTimer(hWnd, 1);
					SetTimer(hWnd, 1, 1000, NULL);
				}
			}

			// 커서가 리스트의 위에 있으면 위로 스크롤. 마우스 위치나 화면 위치 둘 다 화면 좌표로
			// 바꾼후 비교해야 한다. 커서 값이 음수가 될 수 있으므로 (short) 캐스팅도 생략할 수 없다.
			GetWindowRect(GetDlgItem(g_hwndHierarchy, ID_TREE), &rt);
			if ((short)pt.y < rt.top) {
				SetTimer(hWnd, 2, 200, NULL);
				ImageList_DragLeave(NULL);
				SendMessage(GetDlgItem(g_hwndHierarchy, ID_TREE), WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
				ImageList_DragEnter(NULL, pt.x, pt.y);
			}
			else {
				KillTimer(hWnd, 2);
			}

			// 커서가 리스트의 아래에 있으면 아래로 스크롤
			if ((short)pt.y > rt.bottom) {
				SetTimer(hWnd, 3, 200, NULL);
				ImageList_DragLeave(NULL);
				SendMessage(GetDlgItem(g_hwndHierarchy, ID_TREE), WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
				ImageList_DragEnter(NULL, pt.x, pt.y);
			}
			else {
				KillTimer(hWnd, 3);
			}
		}
		break;
	case WM_LBUTTONUP:
		if (g_bDrag) {
			KillTimer(hWnd, 1);
			ImageList_DragLeave(NULL);
			ImageList_EndDrag();
			TreeView_SelectDropTarget(GetDlgItem(g_hwndHierarchy, ID_TREE), NULL);
			ReleaseCapture();
			g_bDrag = FALSE;
			ImageList_Destroy(himl);

			// 드래그된 항목과 드롭된 항목이 같으면 리턴
			if (hDrag == hTarget)
				return 0;

			// 드롭된 항목이 드래그된 항목의 자식 항목이면 리턴
			hItem = hTarget;
			while (hItem != NULL) {
				hItem = TreeView_GetParent(GetDlgItem(g_hwndHierarchy, ID_TREE), hItem);
				if (hItem == hDrag)
					return 0;
			}

			//타겟이 없으면 루트노드의 자식으로
			if (hTarget == (HTREEITEM)0)
			{
				hTarget = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
			}

			// 드래그된 항목을 드롭된 항목 아래로 옮기고 타겟을 확장한다.
			CopyTreeItems(GetDlgItem(g_hwndHierarchy, ID_TREE), hDrag, hTarget);
			TreeView_Expand(GetDlgItem(g_hwndHierarchy, ID_TREE), hTarget, TVE_EXPAND);
			TreeView_DeleteItem(GetDlgItem(g_hwndHierarchy, ID_TREE), hDrag);

			// 스크롤 타이머 파괴
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
		}
		break;
	case WM_TIMER:
		if (wParam == 1) {
			// 드롭 타겟 위에서 1초간 머무르면 타겟을 확장해 준다.
			hItem = TreeView_GetDropHilight(GetDlgItem(g_hwndHierarchy, ID_TREE));
			if ((hItem != NULL) && (hItem == hTarget)) {
				ImageList_DragShowNolock(FALSE);
				TreeView_Expand(GetDlgItem(g_hwndHierarchy, ID_TREE), hTarget, TVE_EXPAND);
				UpdateWindow(GetDlgItem(g_hwndHierarchy, ID_TREE));
				ImageList_DragShowNolock(TRUE);
			}
		}
		if (wParam == 2) {
			SendMessage(GetDlgItem(g_hwndHierarchy, ID_TREE), WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
		}
		if (wParam == 3) {
			SendMessage(GetDlgItem(g_hwndHierarchy, ID_TREE), WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
		}
		return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_DELETE:
		{
			HTREEITEM Node = HierarchyView->NowSelectedTreeNode();
			if (Node != NULL) HierarchyView->DeleteTreeNode(Node);
		}	//case ID_BUTTON_DELETE
			break;
		}	//switch(HIWORD(wParam))
	}	//case WM_COMMAND
		break;
	}	//switch(Msg)

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}