// KCrowtitTool.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "KCrowtitTool.h"
#include "CustomWindow.h"	//Ŀ���� ������ ������ ����
#include "cUtil.h"

#define MAX_LOADSTRING 100

// ���� ����:
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

HWND				g_hWnd = NULL;
HINSTANCE			g_hInstance = NULL;
WINDOWPLACEMENT		g_MainWindowPlacement = { 0 };

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void				MyUnregisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		DlgWorkingCameraSet(HWND, UINT, WPARAM, LPARAM);

HWND hwndWorkingCamera = NULL;

HWND g_hDockHost = NULL;

HWND g_hwndGame = NULL;
HDHNODE g_hGameNode = NULL;

HWND g_hwndHierarchy = NULL;
HDHNODE g_hHierarchyNode = NULL;

HWND g_hwndProject = NULL;
HDHNODE g_hProjectNode = NULL;

HWND g_hwndDebug = NULL;
HDHNODE g_hDebugNode = NULL;

HWND g_hwndInspector = NULL;
HDHNODE g_hInspectorNode = NULL;

cHierarchyView * HierarchyView = NULL;
cGameView * GameView = NULL;
cProjectView * ProjectView = NULL;
cDebugView * DebugView = NULL;
cInspectorView * InspectorView = NULL;

cMainGame * g_MainGame;

short g_WheelDeltaWPARAM;

void SetUp()
{
	GameView->Setup();
	HierarchyView->Setup();
	ProjectView->Setup();

}
void Destroy()
{
	SAFE_DELETE(HierarchyView);
	SAFE_DELETE(GameView);
	SAFE_DELETE(ProjectView);
	SAFE_DELETE(DebugView);
	SAFE_DELETE(InspectorView);

}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR    lpCmdLine, _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KCROWTITTOOL, szWindowClass, MAX_LOADSTRING);

	g_hInstance = hInstance;

	//���Ⱑ ���� �������� 
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KCROWTITTOOL));

	
	

	//���ΰ��� ����
	g_MainGame = new cMainGame;
	g_MainGame->Setup(g_hwndGame);

	//������ setup
	SetUp();

	//�޼��� ����
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}
		}
		g_MainGame->Update();
		g_MainGame->Render();
	}
	
	DockHost_Uninit(hInstance);
	MyUnregisterClass(hInstance);
	
	Destroy();

	SAFE_DELETE(g_MainGame);

	return msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	g_hInstance = hInstance;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KCROWTITTOOL));
	wcex.hIconSm = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KCROWTITTOOL));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);// (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_KCROWTITTOOL);// MAKEINTRESOURCE(IDC_KCROWTITTOOL);
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
		return FALSE;


	RECT rWindow = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) - 62 };
	AdjustWindowRect(&rWindow, WS_OVERLAPPEDWINDOW, FALSE);


	g_hWnd = CreateWindowEx(NULL, wcex.lpszClassName,
		TEXT("Korean Crow-tit Tool"),
		WS_OVERLAPPEDWINDOW,
		0, 0, rWindow.right - rWindow.left,
		rWindow.bottom - rWindow.top,
		NULL, NULL, hInstance, NULL);

	if (g_hWnd == NULL)
		return FALSE;
	
	//ù ���� Ŭ���� 
	if (!DockHost_Init(hInstance))
		return FALSE;

////	�� G ȣ��Ʈ�� �־�� �� ����� �ֱ�
	g_hDockHost = CreateWindowEx(0, WC_DOCKHOST, TEXT(""), WS_CHILD, 0, 0, 0, 0,
		g_hWnd, NULL, hInstance, NULL); 

}

void MyUnregisterClass(HINSTANCE hInstance)
{
	HierarchyView->Uninitialize(hInstance);
	GameView->Uninitialize(hInstance);
	ProjectView->Uninitialize(hInstance);
	DebugView->Uninitialize(hInstance);
	InspectorView->Uninitialize(hInstance);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInstance = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	if (g_hDockHost)
	{
		SendMessage(g_hWnd, WM_SIZE, 0, 0);
		ShowWindow(g_hDockHost, SW_SHOW);

		int resizability = DH_DEFAULT_RESIZABILITY + 1;
		GameView = new cGameView;
		GameView->Initialize(TEXT("GameView"), g_hwndGame, g_hInstance);
		g_hwndGame = GameView->CreateCustomWindow(TEXT("Game"), g_hDockHost, 1152, 864, resizability);
		g_hGameNode = DockHost_Dock(g_hDockHost, g_hwndGame, DH_CENTER);
		
		// ��ü â ũ�⸦ ������ �� ó�� ũ�Ⱑ �������� �� �ֵ��� �ڵ� �����⿡�� �� ���� ������ ������ �����մϴ�.
		//DockHost_SetNodeResizability(g_hGameViewNode, resizability, TRUE);
		resizability = DH_DEFAULT_RESIZABILITY;
		HierarchyView = new cHierarchyView;
		HierarchyView->Initialize(TEXT("HierarchyView"), g_hwndHierarchy, g_hInstance);
		g_hwndHierarchy = HierarchyView->CreateCustomWindow(TEXT("Hierarchy"), g_hDockHost, 450, 600, resizability);
		HierarchyView->SetColor(g_hwndHierarchy, RGB(200, 200, 255));
		g_hHierarchyNode = DockHost_Dock(g_hDockHost, g_hwndHierarchy, DH_LEFT);
	
		ProjectView = new cProjectView;
		ProjectView->Initialize(TEXT("ProjectView"), g_hwndProject, g_hInstance);
		g_hwndProject = ProjectView->CreateCustomWindow(TEXT("Project"), g_hDockHost, 300, 300, resizability);
		ProjectView->SetColor(g_hwndProject, RGB(255, 200, 200));
		g_hProjectNode = DockHost_Dock(g_hDockHost, g_hwndProject, DH_CBOTTOM, g_hHierarchyNode);
		
		DebugView = new cDebugView;
		DebugView->Initialize(TEXT("DebugView"), g_hwndDebug, g_hInstance);
		g_hwndDebug = DebugView->CreateCustomWindow(TEXT("Debug"), g_hDockHost, 600, 300, resizability);
		DebugView->SetColor(g_hwndDebug, RGB(255, 200, 100));
		g_hDebugNode = DockHost_Dock(g_hDockHost, g_hwndDebug, DH_CBOTTOM, g_hGameNode);

		InspectorView = new cInspectorView;
		InspectorView->Initialize(TEXT("InspectorView"), g_hwndInspector, g_hInstance);
		g_hwndInspector = InspectorView->CreateCustomWindow(TEXT("Inspector"), g_hDockHost, 300, 600, resizability);
		InspectorView->SetColor(g_hwndInspector, RGB(100, 255, 100));
		g_hInspectorNode = DockHost_Dock(g_hDockHost, g_hwndInspector, DH_RIGHT, g_hGameNode);
	
		
	}


   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}
//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_WheelDeltaWPARAM = 0;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		int a = 0;
	}
		break;
	case WM_COMMAND:

		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case ID_GAMEOBJECT_LIGHT:
		{
			ENGINE::GameObject* obj = ENGINE::InstantiateLight(ENGINE::eLIGHT_DIRECTIONAL, D3DXVECTOR3(100, 100, 100), D3DXCOLOR(0.8, 0.8, 0.8, 1));
			HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
			HierarchyView->InsertTreeNode(rootNode, obj);
			HierarchyView->SelectTreeNode(obj);
		}
			break;
		case ID_GAMEOBJECT_CAMERA:
		{
			ENGINE::GameObject* obj = ENGINE::InstantiateCamera(g_hwndGame);
			HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
			HierarchyView->InsertTreeNode(rootNode, obj);
			HierarchyView->SelectTreeNode(obj);
		}
			break;
		case ID_3DOBJECT_TERRAIN:
		{
			static int count = 1;
			char szName[256];
			sprintf(szName,"NewTerrain%d.raw",count++);
			ENGINE::GameObject* obj = ENGINE::InstantiateTerrain(szName);
			HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
			HierarchyView->InsertTreeNode(rootNode, obj);
			HierarchyView->SelectTreeNode(obj);
		}
			break;
		case ID_GAMEOBJECT_SKYBOX:
		{
			//ENGINE::GameObject* obj = ENGINE::InstantiateSkyBox("mp_overrated/plains-of-abraham_up.tga",	//up
			//	"mp_overrated/highly-overrated_dn.tga",	//down
			//	"mp_overrated/highly-overrated_lf.tga",	//left
			//	"mp_overrated/highly-overrated_rt.tga",	//right
			//	"mp_overrated/highly-overrated_bk.tga",	//5 - bk
			//	"mp_overrated/highly-overrated_ft.tga"); //4 - fornt
			ENGINE::GameObject* obj = ENGINE::InstantiateSkyBox("Islands.dds"); //4 - fornt

			HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
			HierarchyView->InsertTreeNode(rootNode, obj);
			HierarchyView->SelectTreeNode(obj);
		}
		break;
		/*Save Scnee*/
		case ID_SAVE_SCENE:
		{
			std::wstring filePath;
			if (cUtil::SaveFile(TEXT("XML File(*.xml)\0*.xml\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				fileName += L".xml";
				std::string name;
				cUtil::WStringToString(fileName, name);
				ENGINE::SceneSave(name.c_str());
			}
		}break;

		case ID_LOAD_SCENE:
		{
		//	ENGINE::GameObject* obj = ENGINE::SceneLoad("World.xml");
			ENGINE::GameObject* obj = ENGINE::SceneLoad("Village.xml");
			HierarchyView->ResetTreeNode(obj);

		/*	std::wstring filePath;
			if (cUtil::OpenFile(TEXT("XML File(*.xml)\0*.xml\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				std::string name;
				cUtil::WStringToString(fileName, name);

				std::wstring format = cUtil::PickFormatFromFileName(fileName);

				if (format == L"xml" || format == L"XML")
				{
					ENGINE::GameObject* obj = ENGINE::SceneLoad(name.c_str());
					HierarchyView->ResetTreeNode(obj);
				}	
				else
				{
					MessageBox(0, TEXT("�˸��� ������ �ƴմϴ�."), TEXT("����"), MB_OK);
					break;
				}
			}*/
		}

		break;
		case ID_MENU_WORKINGCAMERASET:
		{
			hwndWorkingCamera = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CAMERA_SET), hWnd, DlgWorkingCameraSet);
			ShowWindow(hwndWorkingCamera, SW_SHOW);
		}
			break;
		case ID_GAMEVIEW  : 
			if (!GetWindowLong(g_hwndGame, GWL_HINSTANCE))
			{
				int resizability = DH_DEFAULT_RESIZABILITY + 1;
				g_hwndGame = GameView->CreateCustomWindow(TEXT("Game"), g_hDockHost, 1152, 864, resizability);
				g_hGameNode = DockHost_Dock(g_hDockHost, g_hwndGame, DH_CENTER);
				}
			break;
		case ID_HIERARCHY : 
			if (!GetWindowLong(g_hwndHierarchy, GWL_HINSTANCE))
			{
				g_hwndHierarchy = HierarchyView->CreateCustomWindow(TEXT("Hierarchy"), g_hDockHost, 450, 600, DH_DEFAULT_RESIZABILITY);
				HierarchyView->SetColor(g_hwndHierarchy, RGB(200, 200, 255));
				g_hHierarchyNode = DockHost_Dock(g_hDockHost, g_hwndHierarchy, DH_LEFT);
			}
			break;
		case ID_DEBUG     : 
			if (!GetWindowLong(g_hwndDebug, GWL_HINSTANCE))
			{
				g_hwndDebug = DebugView->CreateCustomWindow(TEXT("Debug"), g_hDockHost, 600, 300, DH_DEFAULT_RESIZABILITY);
				DebugView->SetColor(g_hwndDebug, RGB(255, 200, 100));
				g_hDebugNode = DockHost_Dock(g_hDockHost, g_hwndDebug, DH_CBOTTOM, g_hGameNode);
			}
			break;
		case ID_PROJECT   : 
			if (!GetWindowLong(g_hwndProject, GWL_HINSTANCE))
			{
				g_hwndProject = ProjectView->CreateCustomWindow(TEXT("Project"), g_hDockHost, 300, 300, DH_DEFAULT_RESIZABILITY);
				ProjectView->SetColor(g_hwndProject, RGB(255, 200, 200));
				g_hProjectNode = DockHost_Dock(g_hDockHost, g_hwndProject, DH_CBOTTOM, g_hHierarchyNode);
			}
			break;
		case ID_INSPECTOR : 
			if (!GetWindowLong(g_hwndInspector, GWL_HINSTANCE))
			{
				g_hwndInspector = InspectorView->CreateCustomWindow(TEXT("Inspector"), g_hDockHost, 300, 600, DH_DEFAULT_RESIZABILITY);
				InspectorView->SetColor(g_hwndInspector, RGB(100, 255, 100));
				g_hInspectorNode = DockHost_Dock(g_hDockHost, g_hwndInspector, DH_RIGHT, g_hGameNode);
			}
			break;
		case IDM_ABOUT:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
	//	else if (wParam == VK_RETURN)
	//	//	ToggleFullscreen(hWnd, g_MainWindowPlacement);
		break;
	case WM_MOUSEWHEEL:
		g_WheelDeltaWPARAM = GET_WHEEL_DELTA_WPARAM(wParam);

		break;
	case WM_SIZE:
		if (g_hDockHost)
		{
			RECT rClient;
			GetClientRect(hWnd, &rClient);
			MoveWindow(g_hDockHost, 0, 0, RectWidth(rClient), RectHeight(rClient), TRUE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL EditByCode = FALSE;

BOOL CALLBACK DlgWorkingCameraSet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	cWorkingCamera* camera = g_MainGame->GetCamera();
	switch (message)
	{
	case WM_INITDIALOG:
	{
		EditByCode = TRUE;
		//üũ�ڽ�
		if (camera->GetViewThisCamera())
			SendMessage(GetDlgItem(hDlg, IDC_CHECK_CAMERASET), BM_SETCHECK, BST_CHECKED, 0);
		else
			SendMessage(GetDlgItem(hDlg, IDC_CHECK_CAMERASET), BM_SETCHECK, BST_UNCHECKED, 0);

		//�����̴�
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1_CAMERA), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2_CAMERA), TBM_SETRANGE, 0, MAKELPARAM(0, 5.0f * 100));

		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1_CAMERA), TBM_SETPOS, TRUE, camera->GetFovy() * 100);
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2_CAMERA), TBM_SETPOS, TRUE, camera->GetAspect() * 100);

	
		float z_near = camera->GetZNear();
		char zNear[128];
		wchar_t* temp;
		sprintf(zNear, "%.2f", z_near);
		temp = UTIL::CharToTCHAR(zNear);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1_CAMERA), temp);
		free(temp);
		
		float z_far = camera->GetZFar();
		char zFar[128];
		sprintf(zFar, "%.2f", z_far);
		temp = UTIL::CharToTCHAR(zFar);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT2_CAMERA), temp);
		free(temp);
		
		EditByCode = FALSE;

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
			break;
		case IDC_CHECK_CAMERASET:
		{
			if (SendMessage(GetDlgItem(hDlg, IDC_CHECK_CAMERASET), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				SendMessage(GetDlgItem(hDlg, IDC_CHECK_CAMERASET), BM_SETCHECK, BST_CHECKED, 0);
				ENGINE::g_pCameraManager->ChangeCamera(camera);
				//camera->SetViewThisCamera(true);
			}
			else
			{
				SendMessage(GetDlgItem(hDlg, IDC_CHECK_CAMERASET), BM_SETCHECK, BST_UNCHECKED, 0);
				camera->SetViewThisCamera(false);
			}
		}
			break;
		case IDC_EDIT1_CAMERA:
		case IDC_EDIT2_CAMERA:
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
			
				if (EditByCode == FALSE)
				{

					TCHAR zNear[128];
					GetWindowText(GetDlgItem(hDlg, IDC_EDIT1_CAMERA), zNear, 128);
					TCHAR zFar[128];
					GetWindowText(GetDlgItem(hDlg, IDC_EDIT2_CAMERA), zFar, 128);

					char* temp;
					temp = UTIL::TCHARToChar(zNear);
					float fZNear = atof(temp);
					delete[] temp;
					temp = UTIL::TCHARToChar(zFar);
					float fZFar = atof(temp);
					delete[] temp;

					camera->SetZNear(fZNear);
					camera->SetZFar(fZFar);
				}
			}
			}
				break;
		}
	}
	return TRUE;
	case WM_NOTIFY:
	{
		if (wParam == IDC_SLIDER1_CAMERA)
		{
			int pos = SendMessage(GetDlgItem(hDlg, IDC_SLIDER1_CAMERA), TBM_GETPOS, 0, 0);
			float fPos = pos / 100.0f;
			camera->SetFovy(fPos);
		}
		if (wParam == IDC_SLIDER2_CAMERA)
		{
			int pos = SendMessage(GetDlgItem(hDlg, IDC_SLIDER2_CAMERA), TBM_GETPOS, 0, 0);
			float fPos = pos / 100.0f;
			camera->SetAspect(fPos);
		}
	}
	return TRUE;
	}
	return FALSE;
}