#include "stdafx.h"
#include "cTransformView.h"

cTransformView::cTransformView()
{
}

cTransformView::~cTransformView()
{
}

BOOL cTransformView::Initialize(LPCTSTR name, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	WC_COMPONENTWINDOW = name;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)cTransformView::WindowProc;;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_winWidth = 300;
	m_winHeight = 350;

	return TRUE;
}

VOID cTransformView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK cTransformView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cTransformView* pView = (cTransformView*)InspectorView->m_vCompontView["1.translateView"];

	switch (Msg)
	{
	case WM_HSCROLL:
		if (pView->m_pSelectObj)
		{
			if ((HWND)lParam == pView->m_hRotationTrackBar[0] ||
				(HWND)lParam == pView->m_hRotationTrackBar[1] ||
				(HWND)lParam == pView->m_hRotationTrackBar[2])
			{
				int posX = SendMessage(pView->m_hRotationTrackBar[0], TBM_GETPOS, 0, 0);
				int posY = SendMessage(pView->m_hRotationTrackBar[1], TBM_GETPOS, 0, 0);
				int posZ = SendMessage(pView->m_hRotationTrackBar[2], TBM_GETPOS, 0, 0);
				float fPosX = posX / 100.0f;
				float fPosY = posY / 100.0f;
				float fPosZ = posZ / 100.0f;

				pView->m_pSelectObj->SetRotation(fPosX, fPosY, fPosZ);
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		/*if (GetFocus() != g_hwndInspector)
			SetFocus(g_hwndInspector);*/
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 20030://추가키
			pView->UpdateTranslateButton();
			break;
		case TAG_COMBOBOX:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				int i = SendMessage(pView->m_tagComboBox, CB_GETCURSEL, 0, 0);
				pView->SetGameObjectTag(i);
				break;
			}
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
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void cTransformView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 5; rt.top = 5;
	rt.right = rt.left + 150; rt.bottom = rt.top + 25;
	DrawText(hdc, L"Translate Component", -1, &rt, DT_CENTER | DT_WORDBREAK);

	rt.left = 5; rt.top = 33;
	rt.right = rt.left + 100; rt.bottom = rt.top + 25;
	DrawText(hdc, L"NAME : ", -1, &rt, DT_LEFT | DT_WORDBREAK);

	//pos출력
	rt.left = 5; rt.top = 60;
	rt.right = rt.left + 75; rt.bottom = rt.top + 20;
	for (int i = 0; i < 3; i++){
		DrawText(hdc, L"POS", -1, &rt, DT_CENTER | DT_WORDBREAK);
		rt.left += 85; rt.right += 85;
	}

	//rot출력
	rt.left = 5; rt.top = 115;
	rt.right = rt.left + 75; rt.bottom = rt.top + 20;
	for (int i = 0; i < 1; i++){
		DrawText(hdc, L"ROT", -1, &rt, DT_CENTER | DT_WORDBREAK);
		rt.left += 85; rt.right += 85;
	}
	rt.left = 5; rt.top = 140;
	rt.right = rt.left + 20; rt.bottom = rt.top + 20;
	DrawText(hdc, L"X", -1, &rt, DT_CENTER | DT_WORDBREAK);
	rt.top = 170;
	rt.bottom = rt.top + 20;
	DrawText(hdc, L"Y", -1, &rt, DT_CENTER | DT_WORDBREAK);
	rt.top = 200;
	rt.bottom = rt.top + 20;
	DrawText(hdc, L"Z", -1, &rt, DT_CENTER | DT_WORDBREAK);


	//scale출력
	rt.left = 5; rt.top = 230;
	rt.right = rt.left + 75; rt.bottom = rt.top + 20;
	for (int i = 0; i < 3; i++){
		DrawText(hdc, L"SCALE", -1, &rt, DT_CENTER | DT_WORDBREAK);
		rt.left += 85; rt.right += 85;
	}
}

void cTransformView::SetTransformValue(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;

	//이름 리스트 박스 생성
	m_hNameListBox = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		70, 30, 150, 25, m_hWnd, (HMENU)9999, m_hInstance, NULL);

	//SRT 리스트 박스 생성
	for (int i = 0; i < 3; i++){
		m_hTranslateListBox[i] = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			5 + (i * 85), 80, 75, 25, m_hWnd, (HMENU)10000 + i, m_hInstance, NULL);

		//m_hRotationListBox[i] = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		//	5 + (i * 85), 135, 75, 25, m_hWnd, (HMENU)10010 + i, m_hInstance, NULL);

		m_hRotationTrackBar[i] = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE,
			30, 135 + (i * 30), 150, 30, m_hWnd, NULL, m_hInstance, NULL);

		m_hScaleListBox[i] = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			5 + (i * 85), 250, 75, 25, m_hWnd, (HMENU)10020 + i, m_hInstance, NULL);
	}

	m_hUpdateButton = CreateWindow(L"button", L"update", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		170, 5, 60, 20, m_hWnd, (HMENU)20030, m_hInstance, NULL);

	//태그 콤보 박스 생성
	m_tagComboBox = CreateWindow(L"combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
		30, 300, 150, 240, m_hWnd, (HMENU)TAG_COMBOBOX, m_hInstance, NULL);

	//태그 넣어주기
	char Items[12][20] = { "TAG_STATIC", "TAG_PLAYER", "TAG_NPC", "TAG_ENEMY", "TAG_WEAPON", "TAG_LIGHT"
						, "TAG_CAMERA", "TAG_TERRAIN", "TAG_SKYBOX", "TAG_BILLBOARD", "TAG_PARTICLE", "TAG_ETC" };
	for (int i = 0; i < 12; i++){
		TCHAR* pItem = UTIL::CharToTCHAR(Items[i]);
		SendMessage(m_tagComboBox, CB_ADDSTRING, 0, (LPARAM)pItem);
		delete pItem;
	}
	//게임 오브젝트 태그 값으로 콤보박스 값을 바꾼다
	SendMessage(m_tagComboBox, CB_SETCURSEL, GetGameObjectTagNumber(), 0);

	char str[128];

	//이름 리스트 박스 값 넣어주기
	char* name = UTIL::TCHARToChar(m_pSelectObj->GetName().c_str());
	sprintf_s(str, "%s", name);
	SetWindowTextA(m_hNameListBox, str);
	delete name;

	D3DXVECTOR3 pos = m_pSelectObj->GetTranslate();
	//SRT 리스트 박스 값 넣어주기
	sprintf_s(str, "%f", pos.x);
	SetWindowTextA(m_hTranslateListBox[0], str);
	sprintf_s(str, "%f", pos.y);
	SetWindowTextA(m_hTranslateListBox[1], str);
	sprintf_s(str, "%f", pos.z);
	SetWindowTextA(m_hTranslateListBox[2], str);


	SendMessage(m_hRotationTrackBar[0], TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));
	SendMessage(m_hRotationTrackBar[1], TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));
	SendMessage(m_hRotationTrackBar[2], TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));

	D3DXVECTOR3 rot = m_pSelectObj->GetRotation();

	SendMessage(m_hRotationTrackBar[0], TBM_SETPOS, TRUE, rot.x * 100);
	SendMessage(m_hRotationTrackBar[1], TBM_SETPOS, TRUE, rot.y * 100);
	SendMessage(m_hRotationTrackBar[2], TBM_SETPOS, TRUE, rot.z * 100);
	
	//sprintf_s(str, "%f", rot.x);
	//SetWindowTextA(m_hRotationListBox[0], str);
	//sprintf_s(str, "%f", rot.y);
	//SetWindowTextA(m_hRotationListBox[1], str);
	//sprintf_s(str, "%f", rot.z);
	//SetWindowTextA(m_hRotationListBox[2], str);

	D3DXVECTOR3 scale = m_pSelectObj->GetScale();
	sprintf_s(str, "%f", scale.x);
	SetWindowTextA(m_hScaleListBox[0], str);
	sprintf_s(str, "%f", scale.y);
	SetWindowTextA(m_hScaleListBox[1], str);
	sprintf_s(str, "%f", scale.z);
	SetWindowTextA(m_hScaleListBox[2], str);
}
void cTransformView::UpdateTranslateButton()
{
	char* chStr;
	wchar_t str[128];
	//----------------------------------------------------위치값 셋
	D3DXVECTOR3 pos;
	GetWindowText(m_hTranslateListBox[0], str, 128);
	chStr = UTIL::TCHARToChar(str);
	pos.x = atof(chStr);
	delete chStr;

	GetWindowText(m_hTranslateListBox[1], str, 128);
	chStr = UTIL::TCHARToChar(str);
	pos.y = atof(chStr);
	delete chStr;

	GetWindowText(m_hTranslateListBox[2], str, 128);
	chStr = UTIL::TCHARToChar(str);
	pos.z = atof(chStr);
	delete chStr;

	m_pSelectObj->SetTranslate(pos.x, pos.y, pos.z);

	////---회전값 셋
	//D3DXVECTOR3 rot;
	//GetWindowText(m_hRotationListBox[0], str, 128);
	//chStr = UTIL::TCHARToChar(str);
	//rot.x = atof(chStr);
	//delete chStr;
	//
	//GetWindowText(m_hRotationListBox[1], str, 128);
	//chStr = UTIL::TCHARToChar(str);
	//rot.y = atof(chStr);
	//delete chStr;
	//
	//GetWindowText(m_hRotationListBox[2], str, 128);
	//chStr = UTIL::TCHARToChar(str);
	//rot.z = atof(chStr);
	//delete chStr;
	//
	//m_pSelectObj->SetRotation(rot.x, rot.y, rot.z);

	//---크기값 셋
	D3DXVECTOR3 scale;
	GetWindowText(m_hScaleListBox[0], str, 128);
	chStr = UTIL::TCHARToChar(str);
	scale.x = atof(chStr);
	delete chStr;

	GetWindowText(m_hScaleListBox[1], str, 128);
	chStr = UTIL::TCHARToChar(str);
	scale.y = atof(chStr);
	delete chStr;

	GetWindowText(m_hScaleListBox[2], str, 128);
	chStr = UTIL::TCHARToChar(str);
	scale.z = atof(chStr);
	delete chStr;

	m_pSelectObj->SetScale(scale.x, scale.y, scale.z);
}
int cTransformView::GetGameObjectTagNumber()
{
	return (int)m_pSelectObj->GetGameObjectTag();
}
void cTransformView::SetGameObjectTag(int tagNumber)
{
	m_pSelectObj->SetGameObjectTag((ENGINE::GAMEOBJECT_TAG)tagNumber);
}

