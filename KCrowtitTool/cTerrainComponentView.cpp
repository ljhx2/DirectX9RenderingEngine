#include "stdafx.h"
#include "cUtil.h"
#include "cTerrainComponentView.h"
#include <io.h>
#include "resource.h"

cTerrainComponentView::cTerrainComponentView()
{ 
	IsDlgEnd = false; 
	m_nX = 255;
	m_nY = 255;
	m_nCell = 1;
	m_fVertexRadius = 1.0f;
	m_fHeight = 5.0f;
	m_fPaintRadius = 0.5f;
	m_nPriodFirst = 0;
	m_nPriodSecond = 0;
	m_nCurrentTexIndex = 0;
	//모든게 사라져서.. 문제군 =ㅁ=...
	for (int i = 0; i < 8; ++i)
		lstrcpy(m_szSelectedTex[i], L"Grass_001.jpg");
}

cTerrainComponentView::~cTerrainComponentView()
{

}

BOOL cTerrainComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
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
	WndClass.lpfnWndProc = (WNDPROC)cTerrainComponentView::WindowProc;;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_winWidth = 300;
	m_winHeight = 1600;

	return TRUE;
}
VOID cTerrainComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	DestroyWindow(m_hWnd);

}

LRESULT CALLBACK cTerrainComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cTerrainComponentView* pView = (cTerrainComponentView*)InspectorView->m_vCompontView["9.TerrainView"];

	switch (Msg)
	{
	case WM_CREATE:
		pView->CreateDlg(hWnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
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
void cTerrainComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 5; rt.top = 5;
	rt.right = rt.left + 150; rt.bottom = rt.top + 25;
	DrawText(hdc, L"Terrain Component", -1, &rt, DT_CENTER | DT_WORDBREAK);

}
void cTerrainComponentView::CreateDlg(HWND hWnd)
{
	HWND hTabDlg = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_DIG_TERRAIN), hWnd, Dlg1Proc);
	ShowWindow(hTabDlg, SW_SHOW);
}

void cTerrainComponentView::PaintDlg(HWND hWnd)
{
	HDC MemDC = NULL;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	BitBlt(hdc, 15, 520, m_mapTextureList[m_szSelectedTex[0]]->width, m_mapTextureList[m_szSelectedTex[0]]->height, m_mapTextureList[m_szSelectedTex[0]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 105, 520, m_mapTextureList[m_szSelectedTex[1]]->width, m_mapTextureList[m_szSelectedTex[1]]->height, m_mapTextureList[m_szSelectedTex[1]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 195, 520, m_mapTextureList[m_szSelectedTex[2]]->width, m_mapTextureList[m_szSelectedTex[2]]->height, m_mapTextureList[m_szSelectedTex[2]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 15, 620, m_mapTextureList[m_szSelectedTex[3]]->width, m_mapTextureList[m_szSelectedTex[3]]->height, m_mapTextureList[m_szSelectedTex[3]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 105, 620, m_mapTextureList[m_szSelectedTex[4]]->width, m_mapTextureList[m_szSelectedTex[4]]->height, m_mapTextureList[m_szSelectedTex[4]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 195, 620, m_mapTextureList[m_szSelectedTex[5]]->width, m_mapTextureList[m_szSelectedTex[5]]->height, m_mapTextureList[m_szSelectedTex[5]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 15, 720, m_mapTextureList[m_szSelectedTex[6]]->width, m_mapTextureList[m_szSelectedTex[6]]->height, m_mapTextureList[m_szSelectedTex[6]]->hMemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 105, 720, m_mapTextureList[m_szSelectedTex[7]]->width, m_mapTextureList[m_szSelectedTex[7]]->height, m_mapTextureList[m_szSelectedTex[7]]->hMemDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);
}
BOOL CALLBACK cTerrainComponentView::Dlg1Proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	cTerrainComponentView* pView = (cTerrainComponentView*)InspectorView->m_vCompontView["9.TerrainView"];

	switch (Msg)
	{
	case WM_INITDIALOG:
		pView->InitDlg(hWnd, wParam, lParam);
		break; 
	case WM_COMMAND:
		pView->CommandDlg(hWnd, wParam, lParam);
		break;
	case WM_HSCROLL : 
		pView->HScroll(hWnd, lParam);
		break;
	case WM_PAINT:
		pView->PaintDlg(hWnd);
		break;
	case WM_NOTIFY:
		pView->Notify(hWnd, lParam);

		break;
	default:
		break;
	}
	return false;
}

void cTerrainComponentView::Notify(HWND hWnd, LPARAM lParam)
{
	NMHDR *p_hdr = (NMHDR *)lParam;
	if (p_hdr->code == NM_CLICK){
		int index;
		HWND listHwnd = GetDlgItem(hWnd, IDC_LIST_TEXTURE);
		index = ListView_GetNextItem(listHwnd, -1, LVNI_ALL | LVNI_SELECTED);
		if (index > -1)
		{
			LPNMITEMACTIVATE nia = (LPNMITEMACTIVATE)lParam;;
			TCHAR szStr[40];
			ListView_GetItemText(listHwnd, nia->iItem, 0, szStr, 255);
			lstrcpy(m_szSelectedTex[m_nCurrentTexIndex], szStr);
			m_pTerrain->SetTexIndex(m_nCurrentTexIndex - 1);
			/*선택한 파일만 저장*/
			TCHAR szPath[800];
			lstrcpy(szPath, L"Terrain/TexBrush/");
			lstrcat(szPath, m_szSelectedTex[m_nCurrentTexIndex]);
			m_pTerrain->SetGroundTexture(UTIL::TCHARToChar(szPath));
			InvalidateRect(hWnd, NULL, FALSE); 
		}
	}
}
void cTerrainComponentView::HScroll(HWND hWnd, LPARAM lParam)
{
	TCHAR szStr[20];
	/*UV & Transparency*/
	if ((HWND)lParam == GetDlgItem(hWnd, IDC_SLIDER_UV))
	{
		int pos = SendMessage(GetDlgItem(hWnd, IDC_SLIDER_UV), TBM_GETPOS, 0, 0);
		HWND uvST = GetDlgItem(hWnd, IDC_STATIC_UV);
		wsprintf(szStr, L"%d", pos);
		SetWindowText(uvST, szStr);
	//	m_pTerrain->

	}
	else if ((HWND)lParam == GetDlgItem(hWnd, IDC_SLIDER_TRCY))
	{
		DWORD pos = SendMessage(GetDlgItem(hWnd, IDC_SLIDER_TRCY), TBM_GETPOS, 0, 0);
		HWND transST = GetDlgItem(hWnd, IDC_STATIC_TRANS);
		wsprintf(szStr, L"%d", pos);
		SetWindowText(transST, szStr);
		m_pTerrain->SetAlphaBrush(pos);
	}
	ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT);
}
void cTerrainComponentView::SettingListView(HWND hlist, const TCHAR * str)
{
	//중복정보 예외처리 안넣음
	TCHAR szDir[600];
	GetCurrentDirectory(600 + 1, szDir);

	/*아이콘*/
	{
		TCHAR szStr[800];
		lstrcpy(szStr, L"Terrain/TexBrush/Icon/");
		lstrcat(szStr, str);
		wcstok(szStr, L".");
		lstrcat(szStr, L".bmp");

		//파일 경로가 잘못 되었으면...
		if (str == NULL) return;

		//DC얻는다
		HDC hdc = GetDC(hlist);

		//이미지 정보 생성
		m_pImageInfo = new IMAGE_INFO;
		m_pImageInfo->resID = 0;
		m_pImageInfo->hMemDC = CreateCompatibleDC(hdc);
		m_pImageInfo->hBit = (HBITMAP)LoadImage(m_hInstance, szStr, IMAGE_BITMAP, 48, 48, LR_LOADFROMFILE);
		m_pImageInfo->hOBit = (HBITMAP)SelectObject(m_pImageInfo->hMemDC, m_pImageInfo->hBit);
		m_pImageInfo->width = 48;
		m_pImageInfo->height = 48;

		m_mapTextureList.insert(std::make_pair(str, m_pImageInfo));
		// 얻은 DC 해제
		ReleaseDC(hlist, hdc);

		//리소스 얻기에 실패했을때 처리
		if (m_pImageInfo->hBit == 0)
		{
			::MessageBox(0, str, 0, 0);
			return ;
		}
	}

	TCHAR szTemp[256];
	lstrcpy(szTemp, str);
	LVITEM LI;
	LI.mask = LVIF_TEXT | LVIF_IMAGE;
	LI.iImage = 0;					//아이템 이미지
	LI.iSubItem = 0;
	LI.iItem = 0;
	LI.pszText = szTemp;			//아이템 이름 
	SendMessage(hlist, LVM_INSERTITEM, 0, (LPARAM)&LI);
	//ListView_SetImageList(hlist, m_pImageInfo->hBit,);
}
void cTerrainComponentView::InitDlg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_dlghWnd = hWnd;
	SetWindowPos(hWnd, HWND_TOP, 0, 35, 0, 0, SWP_NOSIZE);

	/*콤보 박스*/
	HWND combo = GetDlgItem(hWnd, IDC_COMBO_BURSH_TYPE);
	//(L"Smooth"); (L"Slope"); (L"Polygon");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Curved");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Flatter");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Hill");
	/*
	SendMessage( m_hWnd, CB_SETITEMHEIGHT, (WPARAM) -1, (LPARAM) iHeight);
	*/

	/*리스트 뷰*/
	// 텍스트와 이미지를 가진 아이템들을 등록한다.

	HWND hlistView = GetDlgItem(hWnd, IDC_LIST_TEXTURE);
	/*실제 저장*/
	{ 
		struct _finddata_t c_file;
		intptr_t hFile = _findfirst("Terrain/TexBrush/*.*", &c_file);
		if (hFile > -1)
		{
			do
			{
				if (c_file.attrib & _A_SUBDIR)
				{
					// directory 처리
				}
				else if (c_file.attrib & _A_HIDDEN)
				{
					// hidden file 처리
				}
				else
				{
					//여기서 모든 텍스쳐 정보들 저장 하면 엄청 느려짐 + but 안전
					//char szStr[800];
					//strcpy(szStr, "Terrain/TexBrush/");
					//strcat(szStr, c_file.name);
					//g_pTextureManager->GetTextureTest(szStr);
					SettingListView(hlistView, UTIL::CharToTCHAR(c_file.name));
				}
			} while (_findnext(hFile, &c_file) == 0);
		}
		_findclose(hFile);
	}

	SettingTexEdit();

	/*슬라이더 바*/
	HWND UVSliderHwnd = GetDlgItem(hWnd, IDC_SLIDER_UV);
	SendMessage(UVSliderHwnd, TBM_SETRANGE, 0, MAKELPARAM(0, 100));
	HWND trasSliderHwnd = GetDlgItem(hWnd, IDC_SLIDER_TRCY);
	SendMessage(trasSliderHwnd, TBM_SETRANGE, 0, MAKELPARAM(0, 255));
	//HWND transSTHwnd = GetDlgItem(hWnd, IDC_STATIC_TRANS);
	//SendMessage(transSTHwnd, TBM_SETRANGE, 0, MAKELPARAM(0, 255));

	/*쿼드 & 와이어 프레임모드*/
	SendMessage(GetDlgItem(hWnd, IDC_CHECK_WIREFRAME), BM_SETCHECK, BST_UNCHECKED, 0);

	IsDlgEnd = true;
	TCHAR text[50];

	/*정보 불러오기 */
	//switch (HIWORD(wParam))
	//{
	//case EN_CHANGE:
	//{
		/*wsprintf(text,L"%d",m_nX);
		SetWindowText(GetDlgItem(hWnd, IDC_SIZE_X), text);
		wsprintf(text, L"%d", m_nY);
		SetWindowText(GetDlgItem(hWnd, IDC_SIZE_Y), text);
		wsprintf(text, L"%d", m_nCell);
		SetWindowText(GetDlgItem(hWnd, IDC_SIZE_CELL), text);*/
		/*GetWindowText(GetDlgItem(hWnd, IDC_EDIT_VERTEX_TR), text, 50);
		m_fVertexRadius = (float)atof(TCHARToChar(text));
		m_pTerrain->SetRadius(m_fVertexRadius);
		GetWindowText(GetDlgItem(hWnd, IDC_EDIT_HEIGHT_TR), text, 50);
		m_fHeight = (float)atof(TCHARToChar(text));
		m_pTerrain->SetRawFileHeight(m_fHeight);
		GetWindowText(GetDlgItem(hWnd, IDC_EDIT_TEX_TR), text, 50);
		m_fPaintRadius = (float)atof(TCHARToChar(text));
		m_pTerrain->SetPaintRadius(m_fPaintRadius);
		GetWindowText(GetDlgItem(hWnd, IDC_EDIT_PRIO_1), text, 50);
		m_nPriodFirst = atoi(TCHARToChar(text));
		GetWindowText(GetDlgItem(hWnd, IDC_EDIT_PRIO_2), text, 50);
		m_nPriodSecond = atoi(TCHARToChar(text));*/
	//}
	//break;
	//}

}
void cTerrainComponentView::ChangeBrushOption(E_EDIT_BRUSH_OPTION opt)
{
	if (opt == E_EDIT_BRUSH_OPTION::TEX_EDIT)
	{
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_TEXEDIT), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_HTEDIT), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_OBJEDIT), BM_SETCHECK, BST_UNCHECKED, 0);
	}
	else if (opt == E_EDIT_BRUSH_OPTION::HEIGHT_EDIT)
	{
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_TEXEDIT), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_HTEDIT), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_OBJEDIT), BM_SETCHECK, BST_UNCHECKED, 0);
	}
	else if (opt == E_EDIT_BRUSH_OPTION::OBJECT_EDIT)
	{
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_TEXEDIT), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_HTEDIT), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(m_dlghWnd, IDC_RADIO_OBJEDIT), BM_SETCHECK, BST_CHECKED, 0);
	}
}
void cTerrainComponentView::CommandDlg(HWND hWnd,WPARAM wParam, LPARAM lParam)
{
	TCHAR text[50];

	switch (LOWORD(wParam))      // LOWORD(wParam) 컨트롤의 ID
	{
		//* ㅉㅉ*//
		case IDC_SIZE_X:
		case IDC_SIZE_Y:
		case IDC_SIZE_CELL:
		case IDC_EDIT_VERTEX_TR:
		case IDC_EDIT_HEIGHT_TR:
		case IDC_EDIT_TEX_TR:
		case IDC_EDIT_PRIO_1:
		case IDC_EDIT_PRIO_2:
		{
			if (IsDlgEnd)
			{
				switch (HIWORD(wParam))
				{
				case EN_CHANGE:
				{
					GetWindowText(GetDlgItem(hWnd, IDC_SIZE_X), text,50);
					m_nX = atoi(TCHARToChar(text));
					GetWindowText(GetDlgItem(hWnd, IDC_SIZE_Y), text, 50);
					m_nY = atoi(TCHARToChar(text));
					GetWindowText(GetDlgItem(hWnd, IDC_SIZE_CELL), text, 50);
					m_nCell = atoi(TCHARToChar(text));
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_VERTEX_TR), text, 50);
					m_fVertexRadius = (float)atof(TCHARToChar(text));
					m_pTerrain->SetRadius(m_fVertexRadius);
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_HEIGHT_TR), text, 50);
					m_fHeight = (float)atof(TCHARToChar(text));
					m_pTerrain->SetRawFileHeight(m_fHeight);
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_TEX_TR), text, 50);
					m_fPaintRadius = (float)atof(TCHARToChar(text));
					m_pTerrain->SetPaintRadius(m_fPaintRadius);
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_PRIO_1), text, 50);
					m_nPriodFirst = atoi(TCHARToChar(text));
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_PRIO_2), text, 50);
					m_nPriodSecond = atoi(TCHARToChar(text));
				}
				break;
				}
			} 
		}
			break;
		case IDC_SIZE_RESET:
			SettingSize(255, 255, 1);
			m_pTerrain->SettingSize(255,255,1);
			break;

		case IDC_SIZE_ADJUST:
			SettingSize(m_nX, m_nY, m_nCell);
			m_pTerrain->SettingSize(m_nX, m_nY, m_nCell);
			break;

		case IDC_HEIGHT_UP_TR:
		{
			m_fHeight = fabsl(m_fHeight);
			m_pTerrain->SetRawFileHeight(m_fHeight);
			TCHAR szText[128];
			swprintf(szText, L"%f", m_fHeight);
			SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_HEIGHT_TR), szText);
		}
			break;
		case IDC_HEIGHT_DN_TR:
		{
			m_fHeight = fabsl(m_fHeight) * -1;
			m_pTerrain->SetRawFileHeight(m_fHeight);
			TCHAR szText[128];
			swprintf(szText, L"%f", m_fHeight);
			SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_HEIGHT_TR), szText);
		}
			break;

		case IDC_COMBO_BURSH_TYPE:
		{
			switch (HIWORD(wParam))
			{
				case CBN_SELCHANGE:
					HWND combo = GetDlgItem(hWnd, IDC_COMBO_BURSH_TYPE);
					int num = 0;
					num = SendMessage(combo, CB_GETCURSEL, num, (LPARAM)text);
					m_pTerrain->SetBrushType(num);
					ChangeBrushOption(E_EDIT_BRUSH_OPTION::HEIGHT_EDIT);
				break;
			}
		}
			break;

		/*SAVE LOAD*/
		case IDC_HEIGHT_LOAD:
		{
			std::wstring filePath;
			if (cUtil::OpenFile(TEXT("RAW File(*.raw)\0*.raw\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				std::string name;
				cUtil::WStringToString(fileName, name);

				std::wstring format = cUtil::PickFormatFromFileName(fileName);
				 
				if (format == L"raw" || format == L"RAW")
				{
					char szStr[256];
					strcpy(szStr, "Terrain/");
					strcat(szStr, name.c_str());
					m_pTerrain->LoadHeightMap(szStr);
				}
				else
				{
					MessageBox(0, TEXT("알맞은 포맷이 아닙니다."), TEXT("에러"), MB_OK);
					break;
				}
			}
			/*정보 로드*/
			wsprintf(text, L"%d", m_pTerrain->GetRow());
			SetWindowText(GetDlgItem(hWnd, IDC_SIZE_X), text); //여기서 변경됨
			wsprintf(text, L"%d", m_pTerrain->GetCol());
			SetWindowText(GetDlgItem(hWnd, IDC_SIZE_Y), text);
			swprintf(text, L"%.1f", m_pTerrain->GetCellSpacing());
			SetWindowText(GetDlgItem(hWnd, IDC_SIZE_CELL), text);
			swprintf(text, L"%.1f", m_pTerrain->GetRadius());
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_VERTEX_TR), text);
			swprintf(text, L"%.1f", m_pTerrain->GetRawFileHeight());
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_HEIGHT_TR), text);
			swprintf(text, L"%.1f", m_pTerrain->GetPaintRadius());
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_TEX_TR), text);
		}
			break;
		case IDC_HEIGHT_SAVE:
		{
			std::wstring filePath;
			if (cUtil::SaveFile(TEXT("RAW File(*.raw)\0*.raw\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				std::string name;

				TCHAR szStr[256];
				lstrcpy(szStr, L"Terrain/");
				lstrcat(szStr, fileName.c_str());
				cUtil::WStringToString(szStr, name);
				name += ".raw";
				m_pTerrain->SaveHeightMap(name.c_str());
			}
		}
			break;
			/*브러쉬 옵션*/
		case IDC_RADIO_TEXEDIT:
			m_pTerrain->SetBrushType(BRUSH_STATE_TYPE::STATE_PAINT);
			break;
		case IDC_RADIO_HTEDIT:
			m_pTerrain->SetBrushType(BRUSH_STATE_TYPE::STATE_CURVED);
			break;
		case IDC_RADIO_OBJEDIT:
			m_pTerrain->SetBrushType(BRUSH_STATE_TYPE::STATE_OBJECT_PICK);
			break;
		case IDC_CHECK_FOG:
			if (SendMessage(GetDlgItem(hWnd, IDC_CHECK_FOG), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_FOG), BM_SETCHECK, BST_CHECKED, 0);
				m_pTerrain->SetFog(true);
			}
			else
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_FOG), BM_SETCHECK, BST_UNCHECKED, 0);
				m_pTerrain->SetFog(false);
			}
			break;
			/*누르고 있기*/
		case IDC_CHECK_PRESS:
			if (SendMessage(GetDlgItem(hWnd, IDC_CHECK_PRESS), BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_PRESS), BM_SETCHECK, BST_CHECKED, 0);
				m_pTerrain->SetPressing(true);
			}
			else
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_PRESS), BM_SETCHECK, BST_UNCHECKED, 0);
				m_pTerrain->SetPressing(false);
			}
			break;
			/*쿼드트리 */
		case IDC_CHECK_QUAD:
			break;
			/*와이어 프레임 모드*/
		case IDC_CHECK_WIREFRAME:
		{
			if (SendMessage(GetDlgItem(hWnd, IDC_CHECK_WIREFRAME), BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_WIREFRAME), BM_SETCHECK, BST_UNCHECKED, 0);
				m_pTerrain->SetWireFrameMode(false);

			}
			else
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_WIREFRAME), BM_SETCHECK, BST_CHECKED, 0);
				m_pTerrain->SetWireFrameMode(true);
			}
		}
			break;
			/*알파맵 로드 세이브*/
		case IDC_ALPHA_LOAD:
		{
			std::wstring filePath;
			if (cUtil::OpenFile(TEXT("PNG File(*.png)\0*.png\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				std::string name;
				cUtil::WStringToString(fileName, name);

				std::wstring format = cUtil::PickFormatFromFileName(fileName);

				if (format == L"png" || format == L"PNG")
				{
					m_pTerrain->LoadAlphaMap(name.c_str());
				}
				else
				{
					MessageBox(0, TEXT("알맞은 포맷이 아닙니다."), TEXT("에러"), MB_OK);
					break;
				}
			}
		}
			break;
		case IDC_ALPHA_SAVE:
		{
			std::wstring filePath;
			if (cUtil::SaveFile(TEXT("PNG File(*.png)\0*.png\0"), &filePath))
			{
				std::wstring fileName = cUtil::PickFileNameFromFullPath(filePath);
				std::string name;

				TCHAR szStr[256];
				lstrcpy(szStr, L"Terrain/Texture/");
				lstrcat(szStr, fileName.c_str());
				cUtil::WStringToString(szStr, name);
				name += ".png";
				m_pTerrain->SaveAlphaMap(name.c_str());
			}
		}

			break;
		case IDC_RADIO_RAY1: m_nCurrentTexIndex = 0;  m_pTerrain->SetTexIndex(0); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY2: m_nCurrentTexIndex = 1;  m_pTerrain->SetTexIndex(1); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY3: m_nCurrentTexIndex = 2;  m_pTerrain->SetTexIndex(2); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY4: m_nCurrentTexIndex = 3;  m_pTerrain->SetTexIndex(3); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY5: m_nCurrentTexIndex = 4;  m_pTerrain->SetTexIndex(4); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY6: m_nCurrentTexIndex = 5;  m_pTerrain->SetTexIndex(5); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY7: m_nCurrentTexIndex = 6;  m_pTerrain->SetTexIndex(6); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
		case IDC_RADIO_RAY8: m_nCurrentTexIndex = 7;  m_pTerrain->SetTexIndex(7); ChangeBrushOption(E_EDIT_BRUSH_OPTION::TEX_EDIT); break;
			//case IDC_RADIO_RAY9: m_pTerrain->SetTexIndex(7); break;
				/*우선순위 */
		case IDC_BTN_CHANGE_PRIORITY:
			m_pTerrain->ChangePriority(m_nPriodFirst, m_nPriodSecond);
			break;
	}
}

void cTerrainComponentView::SettingTexEdit()
{
	SettingSize(255, 255, 1);

	//에딧박스
	TCHAR szText[128];
	swprintf(szText, L"%f", m_fVertexRadius);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_VERTEX_TR), szText);

	swprintf(szText, L"%f", m_fHeight);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_HEIGHT_TR), szText);

	swprintf(szText, L"%f", m_fPaintRadius);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_TEX_TR), szText);

	wsprintf(szText, L"%d", m_nPriodFirst);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_PRIO_1), szText);

	wsprintf(szText, L"%d", m_nPriodSecond);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_EDIT_PRIO_2), szText);
}
void cTerrainComponentView::SettingSize(int row, int col, int cell)
{
	//에딧박스
	TCHAR szText[128];
	wsprintf(szText, L"%d", row);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_SIZE_X), szText);

	wsprintf(szText, L"%d", col);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_SIZE_Y), szText);

	wsprintf(szText, L"%d", cell);
	SetWindowText(GetDlgItem(m_dlghWnd, IDC_SIZE_CELL), szText);
}

void cTerrainComponentView::InitTerrainComponentView(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;

	//조심 GetComponent
	ENGINE::cTerrainComponent * comp = m_pSelectObj->GetComponent<ENGINE::cTerrainComponent*>(ENGINE::eTERRAIN_TYPE);
	m_pTerrain = comp->GetTerrainData();
	ENGINE::g_pTerrainManager->SetCurrentTerrain(m_pTerrain);

}