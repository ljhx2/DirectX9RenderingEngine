#include "stdafx.h"
#include "cUtil.h"
#include "cSkyBoxComponentView.h"


cSkyBoxComponentView::cSkyBoxComponentView()
{

}

cSkyBoxComponentView::~cSkyBoxComponentView()
{

}

BOOL cSkyBoxComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
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
	WndClass.lpfnWndProc = (WNDPROC)cSkyBoxComponentView::WindowProc;;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_winWidth = 300;
	m_winHeight = 100;

	return TRUE;
}
VOID cSkyBoxComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	DestroyWindow(m_hWnd);

}

LRESULT CALLBACK cSkyBoxComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cSkyBoxComponentView* pView = (cSkyBoxComponentView*)InspectorView->m_vCompontView["8.SkyBoxView"];

	switch (Msg)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SKYBOX_LOAD_BUTTON:
			pView->LoadSkyBox();
			break;
		case ID_SKYBOX_SAVE_BUTTON:
			pView->SaveSkyBox();
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		/*if (GetFocus() != g_hwndInspector)
		SetFocus(g_hwndInspector);*/
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


void cSkyBoxComponentView::InitSkyBoxComponentView(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;


	m_hLoadButton = CreateWindow(L"button", L"LOAD", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		45, 40, 50, 30, m_hWnd, (HMENU)ID_SKYBOX_LOAD_BUTTON, m_hInstance, NULL);

	m_hSaveButton = CreateWindow(L"button", L"SAVE", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		145, 40, 50, 30, m_hWnd, (HMENU)ID_SKYBOX_SAVE_BUTTON, m_hInstance, NULL);
}

void cSkyBoxComponentView::LoadSkyBox()
{
	std::wstring filePath; 
	if (cUtil::OpenFile(TEXT("XML File(*.xml)\0*.xml\0"), &filePath))
	{
		std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
		std::string name;
		cUtil::WStringToString(fileName, name);

		std::wstring format = cUtil::PickFormatFromFileName(fileName);

		if (format == L"xml" || format == L"XML")
		{
			char szStr[256];
			strcpy(szStr, "SkyBox/");
			strcat(szStr, name.c_str());
			ENGINE::cSkyBoxComponent * comp = m_pSelectObj->GetComponent<ENGINE::cSkyBoxComponent*>(ENGINE::eSKYBOX_TYPE);
			comp->ChangeTexture(szStr);
		}
		else
		{
			MessageBox(0, TEXT("알맞은 포맷이 아닙니다."), TEXT("에러"), MB_OK);
		}
	}
}

void cSkyBoxComponentView::SaveSkyBox()
{

	std::wstring filePath;
	if (cUtil::SaveFile(TEXT("XML File(*.xml)\0*.xml\0"), &filePath))
	{
		std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
		fileName += L".xml";
		std::string name;
		cUtil::WStringToString(fileName, name);
		ENGINE::cSkyBoxComponent * comp = m_pSelectObj->GetComponent<ENGINE::cSkyBoxComponent*>(ENGINE::eSKYBOX_TYPE);
		comp->SaveSkyBox(name.c_str());
	}
}
void cSkyBoxComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 5; rt.top = 5;
	rt.right = rt.left + 150; rt.bottom = rt.top + 25;
	DrawText(hdc, L"SkyBox Component", -1, &rt, DT_CENTER | DT_WORDBREAK);

}
