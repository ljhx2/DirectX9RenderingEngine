#include "stdafx.h"
#include "resource.h"
#include "cUtil.h"
#include "cScriptComponentView.h"

#define MAXBUFFER 1048576

cScriptComponentView::cScriptComponentView()
{
	m_pSelectObj = NULL;
	m_pSelectScript = NULL;
}


cScriptComponentView::~cScriptComponentView()
{
}

BOOL cScriptComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
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
	WndClass.lpfnWndProc = (WNDPROC)cScriptComponentView::WindowProc;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_SCRIPT_EDIT_MENU);
	WndClass.lpfnWndProc = (WNDPROC)cScriptComponentView::EditWindowProc;
	WndClass.lpszClassName = L"ScriptEditWin";
	RegisterClass(&WndClass);

	m_winWidth = 300;
	m_winHeight = 100;

	return TRUE;
}

VOID cScriptComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	UnregisterClass(L"ScriptEditWin", hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK cScriptComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cScriptComponentView* pView = (cScriptComponentView*)InspectorView->m_vCompontView["10.scriptView"];

	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_EDIT_BUTTON:
			pView->EditButtonEvent();
			break;
		case ID_LOAD_BUTTON:
			pView->LoadButtonEvent();
			break;
		}
		break;
	case WM_PAINT:
		hdcPaint = BeginPaint(hWnd, &ps);
		if (!hdcPaint)	break;

		pView->drawText(hdcPaint);

		EndPaint(hWnd, &ps);
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
LRESULT CALLBACK cScriptComponentView::EditWindowProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	cScriptComponentView* pView = (cScriptComponentView*)InspectorView->m_vCompontView["10.scriptView"];

	switch (iMessage) {
	case WM_CREATE:
		pView->m_hScriptEditBox = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL
			| ES_MULTILINE | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL,
			10, 10, 200, 25, hWnd, (HMENU)ID_SCRIPT_EDIT_BOX, pView->m_hInstance, NULL);
		SendMessage(pView->m_hScriptEditBox, EM_LIMITTEXT, (WPARAM)MAXBUFFER, 0);

		pView->LoadScriptData();
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SCRIPT_SAVE:
			pView->SaveScriptData();
			break;
		case ID_SCRIPT_EDIT_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_SIZE:
		MoveWindow(pView->m_hScriptEditBox, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void cScriptComponentView::InitScriptComponentView(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;
	m_pSelectScript = pSelectObj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE);

	m_hEditButton = CreateWindow(L"button", L"edit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		5, 40, 40, 20, m_hWnd, (HMENU)ID_EDIT_BUTTON, m_hInstance, NULL);
	m_hLoadButton = CreateWindow(L"button", L"load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		60, 40, 40, 20, m_hWnd, (HMENU)ID_LOAD_BUTTON, m_hInstance, NULL);

	m_hPathEditBox = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		5, 70, 200, 20, m_hWnd, (HMENU)ID_PATH_EDITBOX, m_hInstance, NULL);
	SetWindowTextA(m_hPathEditBox, m_pSelectScript->GetScriptPath());
}
void cScriptComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 5; rt.top = 5;
	rt.right = rt.left + 150; rt.bottom = rt.top + 25;
	DrawText(hdc, L"Script Component", -1, &rt, DT_CENTER | DT_WORDBREAK);
}
void cScriptComponentView::GetRelativePath(std::wstring inPath, char* outPath)
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
void cScriptComponentView::EditButtonEvent()
{
	m_hScriptEditWin = CreateWindow(L"ScriptEditWin", L"hierarchySet", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 1000, 600, m_hWnd, (HMENU)NULL, m_hInstance, NULL);
}
void cScriptComponentView::LoadButtonEvent()
{
	std::wstring filePath;
	char chPath[512];

	if (cUtil::OpenFile(TEXT("ALL FILE(*.*)\0*.*\0"), &filePath))
	{

		GetRelativePath(filePath, chPath);

		//리스트에 추가
		SetWindowTextA(m_hPathEditBox, chPath);

		//scriptComponent에 추가
		m_pSelectScript->Init(m_pSelectObj, chPath);
	}
}
void cScriptComponentView::LoadScriptData()
{
	HANDLE hFile;
	DWORD dwSize;

	TCHAR* pPath = UTIL::CharToTCHAR(m_pSelectScript->GetScriptPath());

	char* pBuf = (char *)malloc(MAXBUFFER);

	hFile = CreateFile(pPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile((HANDLE)hFile, pBuf, MAXBUFFER, &dwSize, NULL);
	pBuf[dwSize] = 0;
	SetWindowTextA(m_hScriptEditBox, pBuf);

	CloseHandle(hFile);
	free(pBuf);

	SendMessage(m_hScriptEditBox, EM_SETMODIFY, (WPARAM)FALSE, 0);

	delete pPath;
}
void cScriptComponentView::SaveScriptData()
{
	HANDLE hFile;
	DWORD Written;
	TCHAR* pPath = UTIL::CharToTCHAR(m_pSelectScript->GetScriptPath());

	char* pBuf = (char *)malloc(MAXBUFFER);

	int length = GetWindowTextA(m_hScriptEditBox, pBuf, MAXBUFFER);
	hFile = CreateFile(pPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, pBuf, length, &Written, NULL);
	
	CloseHandle(hFile);
	free(pBuf);

	SendMessage(m_hScriptEditBox, EM_SETMODIFY, (WPARAM)FALSE, 0);

	//다시 스크립트 시작한다
	m_pSelectScript->Init(m_pSelectObj, m_pSelectScript->GetScriptPath());
}