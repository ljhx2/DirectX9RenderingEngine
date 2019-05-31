#include"stdafx.h"
#include"cGameView.h"



cGameView::cGameView()
{
}
cGameView::~cGameView()
{
	
}
BOOL cGameView::Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hwnd;
	WC_CUSTOMWINDOW = name;

	//main game;

	// Register the color box window class.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)cGameView::WindowProc;
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
VOID cGameView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
}
void cGameView::Setup()
{
	
}

# pragma warning( pop )
LRESULT CALLBACK cGameView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	TWindowInfo* pInfo = (TWindowInfo*)GetWindowLongPtr(hWnd, 0);
	if (Msg == WM_NCCREATE)
	{
		pInfo = new TWindowInfo;
		SetWindowLongPtr(hWnd, 0, (LONG)pInfo);
		
	}
	else if (Msg == WM_NCDESTROY)
	{
		delete pInfo;
		pInfo = NULL;
	}

	if (!pInfo)
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	switch (Msg)
	{
	case WM_CREATE:
	{
	}
		break;
	case WM_CLOSE:
		MessageBox(NULL, L"클로즈", L"ERROR", MB_OK | MB_ICONSTOP);
		DestroyWindow(hWnd);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			TCHAR str[256];
		//	g_pDebug->Log("스페이스바_한글 숫자 TEST %d", 3333);
			int  *(*hello) = ENGINE::g_pWorldMapanager->GetArrWorldMapInfo();
			wsprintf(str, L"%d", hello[0][124]);
			MessageBox(0,str,0,0);
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		if (GetFocus() != g_hwndGame)
			SetFocus(g_hwndGame);
	//	ENGINE::g_pWorldMapanager->SettingWorldMap();
		break;
	case WM_LBUTTONDOWN:
		if (GetFocus() != g_hwndGame)
			SetFocus(g_hwndGame);
		{
			/*터레인 클릭 다운*/
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hwndGame, &ptMouse);
			ENGINE::cTerrainData* tData = ENGINE::g_pTerrainManager->GetCurrentTerrain();
			if (tData)
			{
				tData->SetMouseState(ST_MOUSE_STATE::MOUSE_LCLICK);
				tData->SetMouse(ptMouse);
				tData->UpdateBrush();
			}
		}
	//	g_pDebug->Log("LButtonDown");
		break;
	case WM_LBUTTONUP:
	{
		/*터레인 클릭 업*/
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hwndGame, &ptMouse);

		ENGINE::cTerrainData* tData = ENGINE::g_pTerrainManager->GetCurrentTerrain();

		if (tData)
		{
			tData->SetMouseState(ST_MOUSE_STATE::MOUSE_LCLICKUP);
			tData->SetMouse(ptMouse);
			tData->UpdateBrush();
		}
	}
//g_pDebug->Log("LButtonUp");
	break;
	case WM_MOUSEMOVE:
	{
		/*터레인 클릭 무브*/
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hwndGame, &ptMouse);

		ENGINE::cTerrainData* tData = ENGINE::g_pTerrainManager->GetCurrentTerrain();

		if (tData)
		{
			if (tData->GetPressing() && (tData->GetMouseState() == ST_MOUSE_STATE::MOUSE_LCLICK))
				tData->SetMouseState(ST_MOUSE_STATE::MOUSE_LCLICK);
			else if (!tData->GetPressing())
				tData->SetMouseState(ST_MOUSE_STATE::MOUSE_MOVE);
			if ((tData->GetBrushType() != BRUSH_STATE_TYPE::STATE_NONE) && (tData->GetBrushType() != BRUSH_STATE_TYPE::STATE_OBJECT_PICK))
			{
				tData->SetMouse(ptMouse);
				tData->UpdateBrush();
			}
		}
	}
		break;
	case WM_SIZE:
	{
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);

		//투영
		RECT rc;
		GetClientRect(g_hwndGame, &rc);
		D3DXMATRIXA16 mProj;
		D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1.0f, 1000.0f);
		g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);
	}
		break;
	case WM_PAINT:
	{
		//PAINTSTRUCT ps;
		//HDC hdcPaint = BeginPaint(hWnd, &ps);
		//if (!hdcPaint)
		//	break;
		//RECT rClient;
		//GetClientRect(hWnd, &rClient);
		//FillRect(hdcPaint, &rClient, pInfo->hBrush);
		//EndPaint(hWnd, &ps);
	}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}