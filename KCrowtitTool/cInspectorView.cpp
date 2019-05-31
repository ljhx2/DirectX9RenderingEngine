#include"stdafx.h"
#include"cInspectorView.h"

extern HFONT g_hFont;
cInspectorView::cInspectorView()
{
	m_pSelectGameObject = NULL;
	m_hPrefabSaveButton = NULL;
	m_vCompontView.clear();
}
cInspectorView::~cInspectorView()
{

}
HWND cInspectorView::CreateCustomWindow(LPCTSTR sName, HWND hWndHost, INT width, INT height, int resizability)
{
	HWND hWnd = CreateWindowEx(NULL, WC_CUSTOMWINDOW,
		sName, WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		m_hWnd, NULL, m_hInstance, NULL);
	if (!hWnd)
		return NULL;

	MakeDockable(hWnd, hWndHost, resizability);
	return hWnd;
}

BOOL cInspectorView::Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hwnd;
	WC_CUSTOMWINDOW = name;
	// Register the color box window class.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)cInspectorView::WindowProc;
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
VOID cInspectorView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
	DeleteObject(g_hFont);
	g_hFont = NULL;

	for each(auto p in m_vCompontView)
	{
		p.second->Uninitialize(hInstance);
		SAFE_DELETE(p.second);
	}
	m_vCompontView.clear();
}

//스크롤 변수
int yPos;
int yMax;
LRESULT CALLBACK cInspectorView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	//스크롤 변수
	int yInc;

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

		yPos = 0;
		yMax = 1000;
	}
	else if (Msg == WM_NCDESTROY)
	{
		//assert( pInfo != NULL );
		delete pInfo;
		pInfo = NULL;
	}

	if (!pInfo)
		return DefWindowProc(hWnd, Msg, wParam, lParam);


	switch (Msg)
	{
	case WM_VSCROLL:
		yInc = 0;
		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			yInc = -1;
			break;
		case SB_LINEDOWN:
			yInc = 1;
			break;
		case SB_PAGEUP:
			yInc = -20;
			break;
		case SB_PAGEDOWN:
			yInc = 20;
			break;
		case SB_THUMBTRACK:
			yInc = HIWORD(wParam) - yPos;
			break;
		default:
			break;
		}
		// 새로운 위치는 최소한 0 이상
		if (yPos + yInc < 0)
			yInc = -yPos;
		// 새로운 위치는 최대한 yMax 이하
		if (yPos + yInc > yMax)
			yInc = yMax - yPos;
		// yInc=max(-yPos, min(yInc, yMax-yPos));
		// 새로운 위치 계산
		yPos = yPos + yInc;
		// 스크롤시키고 썸 위치를 다시 계산한다.
		ScrollWindow(hWnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if (GetFocus() != g_hwndInspector)
			SetFocus(g_hwndInspector);
		break;
	case WM_COMMAND:
		int idx;
		switch (LOWORD(wParam)) {
		case 101://saveprefab버튼
			InspectorView->SavePrefab();
			break;
		}
	case WM_SIZE:
	{
					RECT rClient;
					GetClientRect(hWnd, &rClient);

					//각 컴포넌트 window의 사이즈 변경
					int stackY = 0;
					std::map<std::string, cComponentBase*>::iterator iter;
					for (iter = InspectorView->m_vCompontView.begin(); iter != InspectorView->m_vCompontView.end(); ++iter)
					{
						int windowHeight = iter->second->m_winHeight;
						iter->second->ChangeWindowSize(0, stackY, rClient.right, windowHeight);
						stackY += windowHeight;
					}

					StringCchPrintf(pInfo->text, MAX_PATH, TEXT("Inspector size: %d x %d"), rClient.right, rClient.bottom);
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
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
void cInspectorView::SetSelectGameObject(ENGINE::GameObject* pSelectGameObject)
{
	

	m_pSelectGameObject = pSelectGameObject;

	
	for each(auto p in m_vCompontView)
	{
		p.second->Uninitialize(m_hInstance);
		SAFE_DELETE(p.second);
	}
	m_vCompontView.clear();

	//ComponentView 생성위치 y값
	int stackY = 0;
	
	//ComponentView 생성

	cTransformView* pTransView = new cTransformView;
	m_vCompontView.insert(std::make_pair("1.translateView", pTransView));

	pTransView->Initialize(L"1.translateView", m_hInstance);
	pTransView->CreateCustomWindow(L"1.translateView", g_hwndInspector, 0, stackY);
	pTransView->SetTransformValue(m_pSelectGameObject);
	
	stackY += pTransView->m_winHeight;
	
	//
	//cStaticMeshComponentView* pTransView2 = new cStaticMeshComponentView;
	//pTransView2->Initialize(L"testView", m_hInstance);
	//pTransView2->CreateCustomWindow(L"testView", g_hwndInspector, 0, stackY);
	//pTransView2->SetTransformValue(m_pSelectGameObject->GetName(), m_pSelectGameObject->GetTranslate(), m_pSelectGameObject->GetRotation(), m_pSelectGameObject->GetScale());
	//m_vCompontView.push_back(pTransView2);
	//


	int componentSize = m_pSelectGameObject->GetComponentSize();

	for (int i = 0; i < componentSize; i++)
	{
		if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eSTATIC_MESH_TYPE)
		{

		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eSKINNED_MESH_TYPE)
		{

		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eFBX_MESH_TYPE)
		{
			cFbxComponentView* pFbxView = new cFbxComponentView;
			m_vCompontView.insert(std::make_pair("4.fbxView", pFbxView));

			pFbxView->Initialize(L"4.fbxView", m_hInstance);
			pFbxView->CreateCustomWindow(L"4.fbxView", g_hwndInspector, 0, stackY);
			pFbxView->InitFbxComponentView(m_pSelectGameObject);
			stackY += pFbxView->m_winHeight;
		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eCOLLIDER_TYPE)
		{
			cColliderComponentView* pColliderView = new cColliderComponentView;
			m_vCompontView.insert(std::make_pair("5.colliderView", pColliderView));

			pColliderView->Initialize(L"5.colliderView", m_hInstance);
			pColliderView->CreateCustomWindow(L"5.colliderView", g_hwndInspector, 0, stackY);
			pColliderView->SetColliderValue(m_pSelectGameObject);
			stackY += pColliderView->m_winHeight;
		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
		{
			cAniComponentView* pAniView = new cAniComponentView;
			pAniView->Initialize(L"6.aniView", m_hInstance);
			m_vCompontView.insert(std::make_pair("6.aniView", pAniView));
			pAniView->CreateCustomWindow(L"6.aniView", g_hwndInspector, 0, stackY);
			pAniView->SetAniValue((ENGINE::cAnimComponent*)m_pSelectGameObject->GetCompoent(i));
			stackY += pAniView->m_winHeight;
		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eCAMERA_TYPE)
		{
			cCameraComponentView* pCameraView = new cCameraComponentView;
			pCameraView->Initialize(L"7.cameraView", m_hInstance);
			m_vCompontView.insert(std::make_pair("7.cameraView", pCameraView));
			pCameraView->CreateCustomWindow(L"7.cameraView", g_hwndInspector, 0, stackY);
			pCameraView->SetCameraValue(m_pSelectGameObject);
			stackY += pCameraView->m_winHeight;
		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eSKYBOX_TYPE)
		{
			cSkyBoxComponentView * pSkyBoxView = new cSkyBoxComponentView;
			pSkyBoxView->Initialize(L"8.SkyBoxView", m_hInstance);
			m_vCompontView.insert(std::make_pair("8.SkyBoxView", pSkyBoxView));
			pSkyBoxView->CreateCustomWindow(L"8.SkyBoxView", g_hwndInspector, 0, stackY);
			pSkyBoxView->InitSkyBoxComponentView(m_pSelectGameObject);
			stackY += pSkyBoxView->m_winHeight;
		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eTERRAIN_TYPE)
		{
			cTerrainComponentView * pTerrainView = new cTerrainComponentView;
			pTerrainView->Initialize(L"9.TerrainView", m_hInstance);
			m_vCompontView.insert(std::make_pair("9.TerrainView", pTerrainView));
			pTerrainView->CreateCustomWindow(L"9.TerrainView", g_hwndInspector, 0, stackY);
			pTerrainView->InitTerrainComponentView(m_pSelectGameObject);
			stackY += pTerrainView->m_winHeight;
		}
		else if (m_pSelectGameObject->GetCompoent(i)->GetType() == ENGINE::eSCRIPT_TYPE)
		{
			cScriptComponentView * pScriptView = new cScriptComponentView;
			pScriptView->Initialize(L"10.scriptView", m_hInstance);
			m_vCompontView.insert(std::make_pair("10.scriptView", pScriptView));
			pScriptView->CreateCustomWindow(L"10.scriptView", g_hwndInspector, 0, stackY);
			pScriptView->InitScriptComponentView(m_pSelectGameObject);
			stackY += pScriptView->m_winHeight;
		}
		else
		{

		}
	}

	//프리팹 저장 버튼
	if (m_hPrefabSaveButton)
		DestroyWindow(m_hPrefabSaveButton);
	m_hPrefabSaveButton = CreateWindow(L"button", L"save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, stackY + 20, 80, 25, g_hwndInspector, (HMENU)m_idPrefabButton, m_hInstance, NULL);
	stackY += 45;

	//스크롤 조정
	yMax = stackY;
	SetScrollRange(g_hwndInspector, SB_VERT, 0, stackY, TRUE);
	SetScrollPos(g_hwndInspector, SB_VERT, 0, TRUE);
}
void cInspectorView::SavePrefab()
{
	ENGINE::cPrefabParser psrser;
	std::string szFileName;
	UTIL::WStringToString(m_pSelectGameObject->GetName(), szFileName);
	szFileName.erase(szFileName.size()-4,4);
	szFileName += ".prefab";
	char szName[256];
	strcpy_s(szName, szFileName.c_str());
	psrser.SavePrefab(m_pSelectGameObject, szName);
}
