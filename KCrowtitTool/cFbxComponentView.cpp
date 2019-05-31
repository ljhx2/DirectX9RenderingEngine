#include "stdafx.h"
#include "cUtil.h"
#include "cFbxComponentView.h"


cFbxComponentView::cFbxComponentView()
{
	m_hMeterialListView = NULL;

	m_idMeterialDiffuseEditBox[0] = 4006;
	m_idMeterialDiffuseEditBox[1] = 4007;
	m_idMeterialDiffuseEditBox[2] = 4008;
	m_idMeterialDiffuseEditBox[3] = 4009;
}
cFbxComponentView::~cFbxComponentView()
{
}

BOOL cFbxComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
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
	WndClass.lpfnWndProc = (WNDPROC)cFbxComponentView::WindowProc;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	
	WndClass.lpfnWndProc = (WNDPROC)cFbxComponentView::HierarchyProc;
	WndClass.lpszClassName = L"FbxComponentViewChildCls";
	RegisterClass(&WndClass);

	m_winWidth = 300;
	m_winHeight = 400;

	return TRUE;
}
VOID cFbxComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	UnregisterClass(L"FbxComponentViewChildCls", hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK cFbxComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cFbxComponentView* pView = (cFbxComponentView*)InspectorView->m_vCompontView["4.fbxView"];
	int s = 0;
	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if (GetFocus() != g_hwndInspector)
			SetFocus(g_hwndInspector);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 4001://하이라키 set 버튼
			pView->CreateHierarchyWindow();
			break;
		case 4005:
			pView->AddMeterialPath();
			break;
		case 4010:
			pView->SelectMeterialDiffuseUpdate();
			break;
		}
		break;
	case WM_PAINT:
		hdcPaint = BeginPaint(hWnd, &ps);
		if (!hdcPaint)
			break;

		pView->drawText(hdcPaint);

		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		break;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		hdr = (LPNMHDR)lParam;
		nlv = (LPNMLISTVIEW)lParam;
		if (hdr->hwndFrom == pView->m_hMeterialListView) {
			switch (hdr->code) {
				// 선택된 항목을 에디트에 보여준다.
			case LVN_ITEMCHANGED:
				if (nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)) {
				}
				break;
			}
		}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
LRESULT CALLBACK cFbxComponentView::HierarchyProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	cFbxComponentView* pView = (cFbxComponentView*)InspectorView->m_vCompontView["4.fbxView"];
	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 4003:{//ok버튼
			int index = SendMessage(pView->m_hBipListBox, LB_GETCURSEL, 0, 0);
			if (index != -1){

				TCHAR tchValue[128];
				char* chValue;
				SendMessage(pView->m_hBipListBox, LB_GETTEXT, index, (LPARAM)tchValue);

				chValue = UTIL::TCHARToChar(tchValue);
				pView->m_pSelectObj->GetParent()->AttachObjectToFrame(chValue, pView->m_pSelectObj);
				delete chValue;
			}

			DestroyWindow(hWnd);
			break;
		}
		case 4004://cencle버튼
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void cFbxComponentView::InitFbxComponentView(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;

	m_hHierarchyButton = CreateWindow(L"button", L"SetHierarchy", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		110, 5, 100, 25, m_hWnd, (HMENU)m_idHierarchyButton, m_hInstance, NULL);

	//리스트 뷰  생성
	m_hMeterialListView = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS,
		5, 50, 300, 150, m_hWnd, (HMENU)m_idMeterialListView, m_hInstance, NULL);

	LV_COLUMN COL;
	COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt = LVCFMT_LEFT;
	COL.cx = 150;
	COL.pszText = L"매쉬이름";	
	COL.iSubItem = 0;
	ListView_InsertColumn(m_hMeterialListView, 0, &COL);

	COL.pszText = L"경로";	
	COL.iSubItem = 1;
	ListView_InsertColumn(m_hMeterialListView, 1, &COL);

	//선택된 오브젝트의 재질 데이터 검색하고 리스트 뷰에 넣어준다
	int selectObjMeshsize = m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->GetFbxGroupDataNum();
	for (int i = 0; i < selectObjMeshsize; i++){
		AddMeterialName(m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->GetFbxGroupDataName(i));
	}
	m_hMeterialSearchButton = CreateWindow(L"button", L"Meterial Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 210, 150, 25, m_hWnd, (HMENU)m_idMeterialSearchButton, m_hInstance, NULL);

	//재질 diffuse editBox 생성
	for (int i = 0; i < 4; i++)
		m_hMeterialDiffuseEditBox[i] = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		5 + (i * 60), 250, 50, 20, m_hWnd, (HMENU)m_idMeterialDiffuseEditBox[i], m_hInstance, NULL);

	m_hMeterialDiffuseUpdateButton = CreateWindow(L"button", L"Update", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 290, 80, 25, m_hWnd, (HMENU)m_idMeterialDiffuseUpdateButton, m_hInstance, NULL);

}
void cFbxComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 0; rt.top = 5;
	rt.right = rt.left + 100; rt.bottom = rt.top + 20;
	DrawText(hdc, L"FBX MESH", -1, &rt, DT_CENTER | DT_WORDBREAK);
}
void cFbxComponentView::CreateHierarchyWindow()
{
	m_hHierarchyWin = CreateWindow(L"FbxComponentViewChildCls", L"hierarchySet", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 100, 100, 220, 450,
		m_hWnd, (HMENU)NULL, m_hInstance, NULL);

	m_hBipListBox = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |WS_VSCROLL,
		10, 10, 180, 300, m_hHierarchyWin, (HMENU)m_idBipListBox, m_hInstance, NULL);

	m_hOkButton = CreateWindow(L"button", L"ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 350, 80, 25, m_hHierarchyWin, (HMENU)m_idOkButton, m_hInstance, NULL);
	m_hCencleButton = CreateWindow(L"button", L"cencle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 350, 80, 25, m_hHierarchyWin, (HMENU)m_idCencleButton, m_hInstance, NULL);

	

	std::map<std::string, D3DXMATRIXA16>* pvBipMatrixMap;
	std::map<std::string, D3DXMATRIXA16>::iterator iter;
	TCHAR* name;
	pvBipMatrixMap = m_pSelectObj->GetParent()->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->GetBipMatrixMap();
	int i = 0;
	for (iter = pvBipMatrixMap->begin(); iter != pvBipMatrixMap->end(); ++iter)
	{
		name = UTIL::CharToTCHAR(iter->first.c_str());
		SendMessage(m_hBipListBox, LB_ADDSTRING, i, (LPARAM)name);
		delete name;
		i++;
	}
}
void cFbxComponentView::AddMeterialName(std::string name)
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
	ListView_InsertItem(m_hMeterialListView, &LI);

	//매쉬에 이미 지정된 텍스쳐 정보가 있다면 넣어준다
	char* pPath = m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->GetMeterialTexturePath(name.c_str());
	int size = m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->GetMeterialTextureNum();
	if (pPath != NULL){
		std::string temp;
		temp = pPath;
		TCHAR* ptchName = UTIL::CharToTCHAR(temp.c_str());
		ListView_SetItemText(m_hMeterialListView, 0, 1, ptchName);
		delete ptchName;
	}
}
void cFbxComponentView::AddMeterialPath()
{
	int idx = ListView_GetNextItem(m_hMeterialListView, -1, LVNI_ALL | LVNI_SELECTED);
	if (idx == -1)  {
		MessageBox(m_hWnd, L"삭제할 항목을 먼저 선택하십시요", L"알림", MB_OK);
	}
	else {
		std::wstring filePath;
		char chPath[512];

		if (cUtil::OpenFile(TEXT("ALL FILE(*.*)\0*.*\0"), &filePath))
		{

			GetRelativePath(filePath, chPath);

			//리스트에 추가
			TCHAR* ptchName = UTIL::CharToTCHAR(chPath);
			ListView_SetItemText(m_hMeterialListView, idx, 1, ptchName);
			delete ptchName;

			//fbxComponent에 추가
			TCHAR tchName[255];
			ListView_GetItemText(m_hMeterialListView, idx, 0, tchName, 255);

			char* chName = UTIL::TCHARToChar(tchName);

			m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->SetMeterialTexture(chName, chPath, 1.0, 1.0, 1.0, 1.0);

			delete chName;
		}
	}
}
void cFbxComponentView::GetRelativePath(std::wstring inPath, char* outPath)
{
	char tempPath[512];

	//wstring -> char*
	std::string name;
	cUtil::WStringToString(inPath, name);
	strcpy(tempPath, name.c_str());

	int length = strlen(tempPath);
	int i = 0;
	while (i < length){//Resource부터의 상대경로 저장
		if (tempPath[i] == 'R' && tempPath[i + 1] == 'e' && tempPath[i + 2] == 's' && tempPath[i + 3] == 'o' &&
			tempPath[i + 4] == 'u' && tempPath[i + 5] == 'r' && tempPath[i + 6] == 'c' && tempPath[i + 7] == 'e')
		{
			strcpy(outPath, tempPath + i + 9);
			break;
		}
		
		i++;
	}
}
void cFbxComponentView::SelectMeterialDiffuseUpdate()
{
	int idx = ListView_GetNextItem(m_hMeterialListView, -1, LVNI_ALL | LVNI_SELECTED);
	if (idx == -1)  {
		MessageBox(m_hWnd, L"삭제할 항목을 먼저 선택하십시요", L"알림", MB_OK);
	}
	else{
		TCHAR tchName[255];
		ListView_GetItemText(m_hMeterialListView, idx, 0, tchName, 255);
		char* chName = UTIL::TCHARToChar(tchName);

		TCHAR tchPath[255];
		ListView_GetItemText(m_hMeterialListView, idx, 1, tchPath, 255);
		char* chPath = UTIL::TCHARToChar(tchPath);

		D3DXVECTOR4 diffuse;
		char* chDiffuse;
		wchar_t tchDiffuse[128];

		GetWindowText(m_hMeterialDiffuseEditBox[0], tchDiffuse, 128);
		chDiffuse = UTIL::TCHARToChar(tchDiffuse);
		diffuse.x = atof(chDiffuse);
		delete chDiffuse;

		GetWindowText(m_hMeterialDiffuseEditBox[1], tchDiffuse, 128);
		chDiffuse = UTIL::TCHARToChar(tchDiffuse);
		diffuse.y = atof(chDiffuse);
		delete chDiffuse;

		GetWindowText(m_hMeterialDiffuseEditBox[2], tchDiffuse, 128);
		chDiffuse = UTIL::TCHARToChar(tchDiffuse);
		diffuse.z = atof(chDiffuse);
		delete chDiffuse;

		GetWindowText(m_hMeterialDiffuseEditBox[3], tchDiffuse, 128);
		chDiffuse = UTIL::TCHARToChar(tchDiffuse);
		diffuse.w = atof(chDiffuse);
		delete chDiffuse;

		m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->SetMeterialTexture(chName, chPath, diffuse.x, diffuse.y, diffuse.z, diffuse.w);

		delete chName;
		delete chPath;
	}
}