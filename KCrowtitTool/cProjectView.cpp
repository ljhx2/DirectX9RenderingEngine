#include"stdafx.h"
#include"cProjectView.h"
#include "cUtil.h"

extern HFONT g_hFont;
cProjectView::cProjectView()
{
}
cProjectView::~cProjectView()
{

}
BOOL cProjectView::Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hwnd;
	WC_CUSTOMWINDOW = name;
	// Register the color box window class.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)cProjectView::WindowProc;
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
VOID cProjectView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_CUSTOMWINDOW, hInstance);
	DeleteObject(g_hFont);
	g_hFont = NULL;
}

//HWND hList;
//HWND hLoadFile;
//HWND hInstantiate;

void cProjectView::Setup()
{
	RECT rClient;
	GetClientRect(g_hwndProject, &rClient);

	/*hList = */CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY | WS_VSCROLL, 10, 10, rClient.right - 30, rClient.bottom - 80, g_hwndProject, (HMENU)ID_LIST, m_hInstance, NULL);

	/*hLoadFile = */CreateWindow(TEXT("button"), TEXT("Load File"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rClient.right - 200, rClient.bottom - 50, 80, 30, g_hwndProject, (HMENU)ID_BUTTON_LOADFILE, m_hInstance, NULL);

	/*hInstantiate = */CreateWindow(TEXT("button"), TEXT("Instantiate"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rClient.right - 100, rClient.bottom - 50, 80, 30, g_hwndProject, (HMENU)ID_BUTTON_INSTANTIATE, m_hInstance, NULL);
}

TCHAR SelectStr[128];
LRESULT CALLBACK cProjectView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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


	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_LBUTTONDOWN:
		if (GetFocus() != g_hwndProject)
			SetFocus(g_hwndProject);
		break;
	case WM_SIZE:
	{

		RECT rClient;
		GetClientRect(hWnd, &rClient);
		StringCchPrintf(pInfo->text, MAX_PATH, TEXT("Project View size: %d x %d"), rClient.right, rClient.bottom);

		MoveWindow(GetDlgItem(hWnd, ID_LIST), 10, 10, rClient.right - 30, rClient.bottom - 80, TRUE);
		MoveWindow(GetDlgItem(hWnd, ID_BUTTON_LOADFILE), rClient.right - 200, rClient.bottom - 50, 80, 30, TRUE);
		MoveWindow(GetDlgItem(hWnd, ID_BUTTON_INSTANTIATE), rClient.right - 100, rClient.bottom - 50, 80, 30, TRUE);

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
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_LOADFILE:
		{
			std::wstring filePath;
			if (cUtil::OpenFile(TEXT("Prefab File(*.prefab)\0*.prefab\0FBX File(*.fbx)\0*.fbx\0X File(*.x)\0*.x\0OBJ File(*.obj)\0*.obj\0RAW File(*.raw)\0*.raw\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				std::string name;
				cUtil::WStringToString(fileName, name);
				
				std::wstring format = cUtil::PickFormatFromFileName(fileName);

				bool ableAdd = false;

				if (format == L"FBX" || format == L"fbx")
				{					
					if (ENGINE::g_pFbxManager->AddFbxData("Prefabs/", name.c_str()))
						ableAdd = true;
				}
				else if (format == L"X" || format == L"x")
				{
					if (ENGINE::g_pSkinnedMeshManager->AddSkinnedMesh("Prefabs/", name.c_str()))
						ableAdd = true;
				}
				else if (format == L"OBJ" || format == L"obj")
				{
					if (ENGINE::g_pStaticMeshManager->AddStaticMesh("Prefabs/", name.c_str()))
						ableAdd = true;
				}
				else if (format == L"PREFAB" || format == L"prefab")
				{
					ENGINE::GameObject* obj = ENGINE::Instantiate(name.c_str());

					HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
					HierarchyView->InsertTreeNode(rootNode, obj);
					HierarchyView->SelectTreeNode(obj);

					//ENGINE::cPrefabParser psrser;
					//char temp[256];
					//strcpy(temp, name.c_str());
					//psrser.LoadPrefab(temp);
					ableAdd = true;
				}
				else if (format == L"raw" || format == L"RAW")
				{
					//SY 추가 
					char szStr[256];
					strcpy(szStr,"Terrain/");
					strcat(szStr, name.c_str());
					if (ENGINE::g_pTerrainManager->AddTerrain(szStr, name.c_str()))
						ableAdd = true;
				}
				else
				{
					MessageBox(0, TEXT("알맞은 포맷이 아닙니다."), TEXT("에러"), MB_OK);
					return 0;
				}
				
				if (ableAdd) SendMessage(GetDlgItem(hWnd, ID_LIST), LB_ADDSTRING, 0, (LPARAM)fileName.c_str());
				else MessageBox(0, TEXT("이미 Load 한 파일입니다."), TEXT(" "), MB_OK);
						
			}
		}		//case ID_BUTTON_LOADFILE
			break;
		case ID_BUTTON_INSTANTIATE:
		{
			std::wstring format = cUtil::PickFormatFromFileName(SelectStr);
			ENGINE::GameObject* obj;
			if (format == L"FBX" || format == L"fbx")
			{
				obj = ENGINE::InstantiateFbx(UTIL::TCHARToChar(SelectStr));

				ENGINE::cAnimComponent* animComp = obj->GetComponent<ENGINE::cAnimComponent*>(ENGINE::eANIMATION_TYPE);
				/*animComp->AddClip("idle", 0, 0, 84, true);
				animComp->AddClip("run", 0, 85, 105, true);

				ENGINE::cScriptComponent* pScriptComponent = new ENGINE::cScriptComponent(obj, "Script/lua_input.txt");
				obj->AddComponent(pScriptComponent);*/
			}
			else if (format == L"X" || format == L"x")
			{
				obj = ENGINE::InstantiateSkinnedMesh(UTIL::TCHARToChar(SelectStr));
				ENGINE::cAnimComponent* animComp = obj->GetComponent<ENGINE::cAnimComponent*>(ENGINE::eANIMATION_TYPE);
				animComp->AddClip("testt", 0, 0, 200, true);
				animComp->Play("testt");
			}
			else if (format == L"RAW" || format == L"raw")
			{
				obj = ENGINE::InstantiateTerrain(UTIL::TCHARToChar(SelectStr));
			}
			else if (format == L"OBJ" || format == L"obj")
			{
				obj = ENGINE::InstantiateStaticMesh(UTIL::TCHARToChar(SelectStr));
			}
			else //if (format == L"PREFAB" || format == L"prefab")
			{
				obj = ENGINE::Instantiate(UTIL::TCHARToChar(SelectStr));
			}
			HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
			HierarchyView->InsertTreeNode(rootNode, obj);
			HierarchyView->SelectTreeNode(obj);
		}		//case ID_BUTTON_INSTANTIATE
			break;
		case ID_LIST:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				int iSelectIndex;
				iSelectIndex = SendMessage(GetDlgItem(hWnd, ID_LIST), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hWnd, ID_LIST), LB_GETTEXT, iSelectIndex, (LPARAM)SelectStr);
			} //case LBN_SELCHANGE
				break;
			} //switch(HIWORD(wParam))
		}	//case ID_LIST
			break;
		}		//switch(LOWORD(wParam)
	}	//WM_COMMAND
		break;
	}	 //switch(Msg)

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}