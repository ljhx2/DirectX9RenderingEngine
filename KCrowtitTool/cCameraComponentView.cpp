#include "stdafx.h"
#include "cCameraComponentView.h"
#include "resource.h"


cCameraComponentView::cCameraComponentView()
{
}


cCameraComponentView::~cCameraComponentView()
{
}

BOOL cCameraComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
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
	WndClass.lpfnWndProc = (WNDPROC)cCameraComponentView::WindowProc;;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_winWidth = 300;
	m_winHeight = 300;

	return TRUE;
}

VOID cCameraComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK cCameraComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cCameraComponentView* pView = (cCameraComponentView*)InspectorView->m_vCompontView["7.cameraView"];

	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON:
			pView->CreateDialogCamera(hWnd);
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

void cCameraComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 5; rt.top = 5;
	rt.right = rt.left + 150; rt.bottom = rt.top + 25;
	DrawText(hdc, L"Camera Component", -1, &rt, DT_CENTER | DT_WORDBREAK);

}

void cCameraComponentView::SetCameraValue(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;
	m_pSelectCamera = pSelectObj->GetComponent<ENGINE::cCameraComponent*>(ENGINE::eCAMERA_TYPE);

	// 버튼 생성
	m_hButton = CreateWindow(TEXT("button"), TEXT("Setting"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		160, 0, 100, 30, m_hWnd, (HMENU)ID_BUTTON, m_hInstance, NULL);

}

void cCameraComponentView::CreateDialogCamera(HWND hWnd)
{
	if (!IsWindow(m_hDlgCamera))
	{
		m_hDlgCamera = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_CAMERASET), hWnd, DlgProc);
		ShowWindow(m_hDlgCamera, SW_SHOW);
	}
}

BOOL cCameraComponentView::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	cCameraComponentView* pView = (cCameraComponentView*)InspectorView->m_vCompontView["7.cameraView"];

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetWindowPos(hDlg, HWND_TOP, 0, 35, 0, 0, SWP_NOSIZE);
		pView->EditByCode = TRUE;

		//체크박스
		if (pView->m_pSelectCamera->GetViewThisCamera())
			SendMessage(GetDlgItem(hDlg, IDC_CHECK_VIEW_THIS_CAMERA), BM_SETCHECK, BST_CHECKED, 0);
		else
			SendMessage(GetDlgItem(hDlg, IDC_CHECK_VIEW_THIS_CAMERA), BM_SETCHECK, BST_UNCHECKED, 0);


		//슬라이더 범위지정
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER), TBM_SETPOS, TRUE, pView->m_pSelectCamera->GetFovy() * 100);


		//에딧박스
		wchar_t* temp;

		float width = pView->m_pSelectCamera->GetWidth();
		char szWidth[128];
		sprintf(szWidth, "%.2f", width);
		temp = UTIL::CharToTCHAR(szWidth);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_WIDTH), temp);
		free(temp);

		float height = pView->m_pSelectCamera->GetHeight();
		char szHeight[128];
		sprintf(szHeight, "%.2f", height);
		temp = UTIL::CharToTCHAR(szHeight);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_HEIGHT), temp);
		free(temp);

		float z_near = pView->m_pSelectCamera->GetZNear();
		char zNear[128];
		sprintf(zNear, "%.2f", z_near);
		temp = UTIL::CharToTCHAR(zNear);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_NEAR), temp);
		free(temp);

		float z_far = pView->m_pSelectCamera->GetZFar();
		char zFar[128];
		sprintf(zFar, "%.2f", z_far);
		temp = UTIL::CharToTCHAR(zFar);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_FAR), temp);
		free(temp);

		pView->EditByCode = FALSE;
	}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
		{
			DestroyWindow(hDlg);
			hDlg = NULL;
		}
			return TRUE;
		case IDC_CHECK_VIEW_THIS_CAMERA:
		{
			if (SendMessage(GetDlgItem(hDlg, IDC_CHECK_VIEW_THIS_CAMERA), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				SendMessage(GetDlgItem(hDlg, IDC_CHECK_VIEW_THIS_CAMERA), BM_SETCHECK, BST_CHECKED, 0);
				//pView->m_pSelectCamera->SetViewThisCamera(true);
				ENGINE::g_pCameraManager->ChangeCamera(pView->m_pSelectCamera);
				
			}
			else
			{
				SendMessage(GetDlgItem(hDlg, IDC_CHECK_VIEW_THIS_CAMERA), BM_SETCHECK, BST_UNCHECKED, 0);
				//pView->m_pSelectCamera->SetViewThisCamera(false);
				ENGINE::g_pCameraManager->ChangeCamera(g_MainGame->GetCamera());
			}
		}
			break;
		case IDC_EDIT_WIDTH:
		case IDC_EDIT_HEIGHT:
		case IDC_EDIT_NEAR:
		case IDC_EDIT_FAR:
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				if (pView->EditByCode == FALSE)
				{
					TCHAR Width[128];
					GetWindowText(GetDlgItem(hDlg, IDC_EDIT_WIDTH), Width, 128);
					TCHAR Height[128];
					GetWindowText(GetDlgItem(hDlg, IDC_EDIT_HEIGHT), Height, 128);
					TCHAR zNear[128];
					GetWindowText(GetDlgItem(hDlg, IDC_EDIT_NEAR), zNear, 128);
					TCHAR zFar[128];
					GetWindowText(GetDlgItem(hDlg, IDC_EDIT_FAR), zFar, 128);

					char* temp;
					temp = UTIL::TCHARToChar(Width);
					float fWidth = atof(temp);
					delete[] temp;
					temp = UTIL::TCHARToChar(Height);
					float fHeight = atof(temp);
					delete[] temp;
					temp = UTIL::TCHARToChar(zNear);
					float fZNear = atof(temp);
					delete[] temp;
					temp = UTIL::TCHARToChar(zFar);
					float fZFar = atof(temp);
					delete[] temp;

					pView->m_pSelectCamera->SetWidth(fWidth);
					pView->m_pSelectCamera->SetHeight(fHeight);
					pView->m_pSelectCamera->SetZNear(fZNear);
					pView->m_pSelectCamera->SetZFar(fZFar);
				}
			}
			}
			break;
		}
			break;
		}
		break;
	case WM_NOTIFY:	//Rotate 슬라이더 변경
	{
		if (wParam == IDC_SLIDER)
		{
			int pos = SendMessage(GetDlgItem(hDlg, IDC_SLIDER), TBM_GETPOS, 0, 0);
			float fPos = pos / 100.0f;
			pView->m_pSelectCamera->SetFovy(fPos);
		}
		
	}
	return TRUE;
	}

	return FALSE;
}