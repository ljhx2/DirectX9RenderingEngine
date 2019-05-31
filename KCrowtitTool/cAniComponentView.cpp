#include "stdafx.h"
#include "cAniComponentView.h"


cAniComponentView::cAniComponentView()
{
	m_hClildAddWin = NULL;
	m_hListView = NULL;
	m_pAniComponent = NULL;
}


cAniComponentView::~cAniComponentView()
{
}

BOOL cAniComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	WC_COMPONENTWINDOW = name;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = sizeof(TWindowInfo*);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)cAniComponentView::WindowProc;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = (WNDPROC)cAniComponentView::ChildProc;
	WndClass.lpszClassName = L"AniComponentChildCls";
	RegisterClass(&WndClass);

	m_winWidth = 300;
	m_winHeight = 350;
	
	return TRUE;
}
VOID cAniComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	UnregisterClass(L"AniComponentChildCls", hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK cAniComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cAniComponentView* pView;
	pView = (cAniComponentView*)InspectorView->m_vCompontView["6.aniView"];

	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if (GetFocus() != g_hwndInspector)
			SetFocus(g_hwndInspector);
		break;
	case WM_PAINT:
		hdcPaint = BeginPaint(hWnd, &ps);
		if (!hdcPaint)	break;

		pView->drawText(hdcPaint);

		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		break;
	case WM_COMMAND:
		int idx;
		switch (LOWORD(wParam)) {
		case 1000://추가키
			pView->Addbutton();
			break;
		case 1001://삭제키
			idx = ListView_GetNextItem(pView->m_hListView, -1, LVNI_ALL | LVNI_SELECTED);
			if (idx == -1)  {
				MessageBox(hWnd, L"삭제할 항목을 먼저 선택하십시요", L"알림", MB_OK);
			}
			else {
				//삭제할 리스트 뷰 이름 불러와서 애니컴포넌트에서 지워준다 
				TCHAR * pwstr = (LPWSTR)malloc(sizeof(TCHAR)* 100);
				memset(pwstr, 0, sizeof(pwstr));
				ListView_GetItemText(pView->m_hListView, idx, 0, pwstr, 255);
				pView->DeleteAniClip(pwstr);
				delete pwstr;

				//리스트 뷰에서도 삭제
				ListView_DeleteItem(pView->m_hListView, idx);
			}
			break;
		case 1002://삭제키
			idx = ListView_GetNextItem(pView->m_hListView, -1, LVNI_ALL | LVNI_SELECTED);
			if (idx == -1)  {
				MessageBox(hWnd, L"플레이할 항목을 먼저 선택하십시요", L"알림", MB_OK);
			}
			else {
				//플레이할 리스트 뷰 이름 불러와서 애니컴포넌트에서 지워준다 
				TCHAR * pwstr = (LPWSTR)malloc(sizeof(TCHAR)* 100);
				memset(pwstr, 0, sizeof(pwstr));
				ListView_GetItemText(pView->m_hListView, idx, 0, pwstr, 255);
				pView->PlayClip(pwstr);
				delete pwstr;
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
LRESULT CALLBACK cAniComponentView::ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	cAniComponentView* pView = (cAniComponentView*)InspectorView->m_vCompontView["6.aniView"];
	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 20010://ok버튼
			TCHAR chTtemp[128];
			char* chTemp;
			std::string name;
			int index;
			int start;
			int end;
			bool isLoop;

			//이름
			GetWindowText(pView->m_hNameEdit, chTtemp, 128);
			chTemp = UTIL::TCHARToChar(chTtemp);
			name = chTemp;
			delete chTemp;

			//인덱스
			GetWindowText(pView->m_hIndexEdit, chTtemp, 128);
			chTemp = UTIL::TCHARToChar(chTtemp);
			index = atoi(chTemp);
			delete chTemp;

			//시작
			GetWindowText(pView->m_hStartEdit, chTtemp, 128);
			chTemp = UTIL::TCHARToChar(chTtemp);
			start = atoi(chTemp);
			delete chTemp;
			
			//끝
			GetWindowText(pView->m_hEndEdit, chTtemp, 128);
			chTemp = UTIL::TCHARToChar(chTtemp);
			end = atoi(chTemp);
			delete chTemp;

			//인덱스
			GetWindowText(pView->m_hIsLoopEdit, chTtemp, 128);
			chTemp = UTIL::TCHARToChar(chTtemp);
			if (strcmp(chTemp, "true") == 0)
				isLoop = true;
			else
				isLoop = false;
			
			delete chTemp;

			pView->AddClipList(name,index,start,end,isLoop);
			pView->AddAniClip(name, index, start, end, isLoop);

			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		RECT rt;
		hdc = BeginPaint(hWnd, &ps);

		rt.left = 10; rt.top = 0;
		rt.right = rt.left + 50; rt.bottom = rt.top + 30;
		DrawText(hdc, L"name", -1, &rt, DT_CENTER | DT_WORDBREAK);

		rt.left = 70; rt.top = 0;
		rt.right = rt.left + 50; rt.bottom = rt.top + 30;
		DrawText(hdc, L"index", -1, &rt, DT_CENTER | DT_WORDBREAK);

		rt.left = 130; rt.top = 0;
		rt.right = rt.left + 50; rt.bottom = rt.top + 30;
		DrawText(hdc, L"start", -1, &rt, DT_CENTER | DT_WORDBREAK);

		rt.left = 190; rt.top = 0;
		rt.right = rt.left + 50; rt.bottom = rt.top + 30;
		DrawText(hdc, L"end", -1, &rt, DT_CENTER | DT_WORDBREAK);

		rt.left = 250; rt.top = 0;
		rt.right = rt.left + 50; rt.bottom = rt.top + 30;
		DrawText(hdc, L"isLoop", -1, &rt, DT_CENTER | DT_WORDBREAK);

		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void cAniComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 0; rt.top = 60;
	rt.right = rt.left + 75; rt.bottom = rt.top + 20;
	DrawText(hdc, L"ANI", -1, &rt, DT_CENTER | DT_WORDBREAK);
}

void cAniComponentView::SetAniValue(ENGINE::cAnimComponent* pAniComponent)
{
	m_pAniComponent = pAniComponent;
	//리스트 뷰 생성
	

	// 리스트 컨트롤을 자세히 보기로 만든다.
	m_hListView = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS,
		0, 0, 300, 300, m_hWnd, NULL, m_hInstance, NULL);

	LV_COLUMN COL;
	// 헤더를 추가한다.
	COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt = LVCFMT_LEFT;
	COL.cx = 60;
	COL.pszText = L"이름";			// 첫번째 헤더
	COL.iSubItem = 0;
	ListView_InsertColumn(m_hListView, 0, &COL);

	COL.pszText = L"인덱스";		// 두번째 헤더
	COL.iSubItem = 1;
	ListView_InsertColumn(m_hListView, 1, &COL);

	COL.pszText = L"시작";			// 세번째 헤더
	COL.iSubItem = 2;
	ListView_InsertColumn(m_hListView, 2, &COL);

	COL.pszText = L"종료";			// 세번째 헤더
	COL.iSubItem = 3;
	ListView_InsertColumn(m_hListView, 3, &COL);

	COL.pszText = L"루프";			// 세번째 헤더
	COL.iSubItem = 4;
	ListView_InsertColumn(m_hListView, 4, &COL);

	std::map<std::string, ENGINE::ST_AnimClip*>* pvAniClip;
	std::map<std::string, ENGINE::ST_AnimClip*>::iterator iter;

	pvAniClip = m_pAniComponent->GetAniClip();
	int number = 0;
	for (iter = pvAniClip->begin(); iter != pvAniClip->end(); ++iter)
	{
		AddClipList(iter->second->name, iter->second->setIndex, iter->second->start, iter->second->end, iter->second->isLoop);
		number++;
	}
	
	//버튼 3개 추가
	m_hAddAniClipButton = CreateWindow(L"button", L"Add", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 310, 80, 25, m_hWnd, (HMENU)addAniClipButtonID, m_hInstance, NULL);
	m_hDeleteAniClipButton = CreateWindow(L"button", L"Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 310, 80, 25, m_hWnd, (HMENU)deleteAniClipButtonID, m_hInstance, NULL);
	m_hPlayAniClipButton = CreateWindow(L"button", L"Play", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 310, 80, 25, m_hWnd, (HMENU)playAniClipButtonID, m_hInstance, NULL);
}

void cAniComponentView::Addbutton()
{
	m_hClildAddWin = CreateWindow(L"AniComponentChildCls", L"aniClip입력", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 100, 100, 330, 140,
		m_hWnd, (HMENU)NULL, m_hInstance, NULL);

	m_hNameEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		10, 30, 50, 25, m_hClildAddWin, (HMENU)20001, m_hInstance, NULL);
	m_hIndexEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		70, 30, 50, 25, m_hClildAddWin, (HMENU)20002, m_hInstance, NULL);
	m_hStartEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		130, 30, 50, 25, m_hClildAddWin, (HMENU)20003, m_hInstance, NULL);
	m_hEndEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		190, 30, 50, 25, m_hClildAddWin, (HMENU)20004, m_hInstance, NULL);
	m_hIsLoopEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		250, 30, 50, 25, m_hClildAddWin, (HMENU)20005, m_hInstance, NULL);

	m_hOkButton = CreateWindow(L"button", L"ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		160, 70, 80, 25, m_hClildAddWin, (HMENU)20010, m_hInstance, NULL);
}
void cAniComponentView::AddClipList(std::string name, int index, int start, int end, bool isLoop)
{
	LV_ITEM LI;

	LI.mask = LVIF_TEXT | LVIF_IMAGE;
	LI.state = 0;
	LI.stateMask = 0;
	LI.iImage = 0;
	LI.iSubItem = 0;
	LI.iItem = 0;
	LI.pszText = UTIL::CharToTCHAR(name.c_str());
	LI.cchTextMax = 20;
	ListView_InsertItem(m_hListView, &LI);

	TCHAR* ptchName;
	std::string temp;
	char chTemp[128];
	memset(chTemp, 0, 128);

	_itoa(index, chTemp, 10);
	temp = chTemp;
	ptchName = UTIL::CharToTCHAR(temp.c_str());
	ListView_SetItemText(m_hListView, 0, 1, ptchName);
	delete ptchName;

	_itoa(start, chTemp, 10);
	temp = chTemp;
	ptchName = UTIL::CharToTCHAR(temp.c_str());
	ListView_SetItemText(m_hListView, 0, 2, UTIL::CharToTCHAR(temp.c_str()));
	delete ptchName;

	_itoa(end, chTemp, 10);
	temp = chTemp;
	ptchName = UTIL::CharToTCHAR(temp.c_str());
	ListView_SetItemText(m_hListView, 0, 3, UTIL::CharToTCHAR(temp.c_str()));
	delete ptchName;

	if (isLoop){
		ListView_SetItemText(m_hListView, 0, 4, L"true");
	}
	else if (!isLoop){
		ListView_SetItemText(m_hListView, 0, 4, L"false");
	}
}
void cAniComponentView::DeleteClipList()
{
	int idx = ListView_GetNextItem(m_hListView, -1, LVNI_ALL | LVNI_SELECTED);

	if (idx == -1)  {
		MessageBox(m_hWnd, L"삭제할 항목을 먼저 선택하십시요", L"알림", MB_OK);
	}
	else {
		ListView_DeleteItem(m_hListView, idx);
	}
}
void cAniComponentView::AddAniClip(std::string name, int index, int start, int end, bool isLoop)
{
	m_pAniComponent->AddClip(name, index, start, end, isLoop);
}
void cAniComponentView::DeleteAniClip(TCHAR* name)
{
	std::string stName;

	char* chName = UTIL::TCHARToChar(name);
	stName = chName;
	m_pAniComponent->DeleteClip(stName);
	delete chName;
}
void cAniComponentView::PlayClip(TCHAR* name)
{
	std::string stName;

	char* chName = UTIL::TCHARToChar(name);
	stName = chName;
	m_pAniComponent->Play(stName);
	delete chName;
}