#include "stdafx.h"
#include "cColliderComponentView.h"
#include "resource.h"

cColliderComponentView::cColliderComponentView()
{
}


cColliderComponentView::~cColliderComponentView()
{
}

BOOL cColliderComponentView::Initialize(LPCTSTR name, HINSTANCE hInstance)
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
	WndClass.lpfnWndProc = (WNDPROC)cColliderComponentView::WindowProc;;
	WndClass.lpszClassName = WC_COMPONENTWINDOW;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_winWidth = 300;
	m_winHeight = 200;

	return TRUE;
}

VOID cColliderComponentView::Uninitialize(HINSTANCE hInstance)
{
	UnregisterClass(WC_COMPONENTWINDOW, hInstance);
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK cColliderComponentView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdcPaint;
	cColliderComponentView* pView = (cColliderComponentView*)InspectorView->m_vCompontView["5.colliderView"];

	switch (Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CHECKBOX:
			if (SendMessage(pView->m_hCheck, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				SendMessage(pView->m_hCheck, BM_SETCHECK, BST_CHECKED, 0);
				pView->m_pSelectCollider->SetRenderCollisionSphere(true);
			}
			else
			{
				SendMessage(pView->m_hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
				pView->m_pSelectCollider->SetRenderCollisionSphere(false);
			}

			break;
		case ID_RADIO1:
			pView->m_pSelectCollider->SetCollisionTYPE(ENGINE::eMOVABLE_COLLISION);
			break;
		case ID_RADIO2:
			pView->m_pSelectCollider->SetCollisionTYPE(ENGINE::eSTATIC_COLLISION);
			break;
		//case ID_BUTTONSPHERE:
		//	pView->CreateDialogSphere(hWnd);
		//	break;
		case ID_BUTTONBOX:
			pView->CreateDialogBox(hWnd);
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

void cColliderComponentView::drawText(HDC hdc)
{
	RECT rt;

	rt.left = 5; rt.top = 5;
	rt.right = rt.left + 150; rt.bottom = rt.top + 25;
	DrawText(hdc, L"Collider Component", -1, &rt, DT_CENTER | DT_WORDBREAK);

}

void cColliderComponentView::SetColliderValue(ENGINE::GameObject* pSelectObj)
{
	m_pSelectObj = pSelectObj;
	m_pSelectCollider = pSelectObj->GetComponent<ENGINE::cColliderComponent*>(ENGINE::eCOLLIDER_TYPE);

	//체크박스 생성
	m_hCheck = CreateWindow(TEXT("button"), TEXT("Bounding Sphere"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		20, 30, 150, 25, m_hWnd, (HMENU)ID_CHECKBOX, m_hInstance, NULL);

	if (m_pSelectCollider->GetRenderCOllisionSphere())
	{
		SendMessage(m_hCheck, BM_SETCHECK, BST_CHECKED, 0);
	}
	else
	{
		SendMessage(m_hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	
	//라디오 버튼
	m_hRadio1 = CreateWindow(TEXT("button"), TEXT("Movable"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		30, 60, 120, 20, m_hWnd, (HMENU)ID_RADIO1, m_hInstance, NULL);

	m_hRadio2 = CreateWindow(TEXT("button"), TEXT("Static"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		150, 60, 120, 20, m_hWnd, (HMENU)ID_RADIO2, m_hInstance, NULL);

	ENGINE::ECOLLISION_TYPE type = m_pSelectCollider->GetCollisionTYPE();
	if (type == ENGINE::eMOVABLE_COLLISION)
	{
		CheckRadioButton(m_hWnd, ID_RADIO1, ID_RADIO2, ID_RADIO1);
	}
	else if (type == ENGINE::eSTATIC_COLLISION)
	{
		CheckRadioButton(m_hWnd, ID_RADIO1, ID_RADIO2, ID_RADIO2);
	}


	//스피어 버튼 생성
	//m_hButtonSphere = CreateWindow(TEXT("button"), TEXT("Sphere Collision"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	//	30, 80, 120, 30, m_hWnd, (HMENU)ID_BUTTONSPHERE, m_hInstance, NULL);

	//박스 버튼 생성
	m_hButtonBox = CreateWindow(TEXT("button"), TEXT("Box Collision"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		160, 80, 120, 30, m_hWnd, (HMENU)ID_BUTTONBOX, m_hInstance, NULL);

}

/*
void cColliderComponentView::CreateDialogSphere(HWND hWnd)
{
	if (!IsWindow(m_hDlgSphere))
	{
		m_hDlgSphere = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_SPHERECOLLISION), hWnd, DlgSphereProc);
		ShowWindow(m_hDlgSphere, SW_SHOW);
	}
}
*/

void cColliderComponentView::CreateDialogBox(HWND hWnd)
{
	if (!IsWindow(m_hDlgBox))
	{
		m_hDlgBox = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_BOXCOLLISION), hWnd, DlgBoxProc);
		ShowWindow(m_hDlgBox, SW_SHOW);
	}
}

void cColliderComponentView::CreateDialogFindFrame(HWND hWnd)
{
	if (!IsWindow(m_hDlgFindFrame))
	{
		m_hDlgFindFrame = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_FINDFRAME), hWnd, DlgFindFrameProc);
		ShowWindow(m_hDlgFindFrame, SW_SHOW);
	}
}

/*
BOOL cColliderComponentView::DlgSphereProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	cColliderComponentView* pView = (cColliderComponentView*)InspectorView->m_vCompontView["5.colliderView"];

	switch (message)
	{
	case WM_INITDIALOG:
	{
		pView->selectCollisionSphere = NULL;
		//Rotate 슬라이더 범위지정
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_X_SPHERECOLLISION), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Y_SPHERECOLLISION), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Z_SPHERECOLLISION), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));

		//Translate Spin버튼 Buddy지정
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_X_SPHERECOLLISION), UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_X_SPHERECOLLISION), 0);
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Y_SPHERECOLLISION), UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Y_SPHERECOLLISION), 0);
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Z_SPHERECOLLISION), UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Z_SPHERECOLLISION), 0);
		//Translate Spin 범위지정
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_X_SPHERECOLLISION), UDM_SETRANGE, 0, MAKELPARAM(10000, -10000));
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Y_SPHERECOLLISION), UDM_SETRANGE, 0, MAKELPARAM(10000, -10000));
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Z_SPHERECOLLISION), UDM_SETRANGE, 0, MAKELPARAM(10000, -10000));


		//list 초기화
		for each(auto iter in *(pView->m_pSelectCollider->GetMapCollisionSphere()))
		{
			WCHAR* name = UTIL::CharToTCHAR(iter.first.c_str());
			SendMessage(GetDlgItem(hDlg, IDC_LIST_SPHERECOLLISION), LB_ADDSTRING, 0, (LPARAM)name);
			free(name);
		}
	}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST_SPHERECOLLISION:	//리스트박스 변경
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				pView->EditByCode = TRUE;
		
				int index;
				wchar_t collisionName[256];
				index = SendMessage(GetDlgItem(hDlg, IDC_LIST_SPHERECOLLISION), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_LIST_SPHERECOLLISION), LB_GETTEXT, index, (LPARAM)collisionName);
		
				pView->selectCollisionSphere = pView->m_pSelectCollider->GetClassCollisionSphere(UTIL::TCHARToChar(collisionName));

				//리스트에서 선택한 항목에 따라 에디트박스 변경

				//체크박스변경
				if (pView->selectCollisionSphere->GetIsRender())
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_SPHERECOLLISION), BM_SETCHECK, BST_CHECKED, 0);
				}
				else
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_SPHERECOLLISION), BM_SETCHECK, BST_UNCHECKED, 0);
				}

				//Scale 에디트 박스 변경
				char scale[128];
				sprintf(scale, "%.2f", pView->selectCollisionSphere->GetLocalScale());
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_SPHERECOLLISION), UTIL::CharToTCHAR(scale));

				//Rotate 슬라이더 변경
				D3DXVECTOR3 rotation = pView->selectCollisionSphere->GetLocalRotation();
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_X_SPHERECOLLISION), TBM_SETPOS, TRUE, rotation.x * 100);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Y_SPHERECOLLISION), TBM_SETPOS, TRUE, rotation.y * 100);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Z_SPHERECOLLISION), TBM_SETPOS, TRUE, rotation.z * 100);

				//Translate 에디트 박스 변경
				D3DXVECTOR3 position = pView->selectCollisionSphere->GetLocalPosition();
				char pos[128];
				sprintf(pos, "%.2f", position.x);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_X_SPHERECOLLISION), UTIL::CharToTCHAR(pos));
				sprintf(pos, "%.2f", position.y);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Y_SPHERECOLLISION), UTIL::CharToTCHAR(pos));
				sprintf(pos, "%.2f", position.z);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Z_SPHERECOLLISION), UTIL::CharToTCHAR(pos));
		
				pView->EditByCode = FALSE;
			}
				return TRUE;
			}
		}
			return TRUE;
		case IDC_CHECK_SPHERECOLLISION:	//체크박스 변경
		{
			if (pView->selectCollisionSphere != NULL)
			{
				if (SendMessage(GetDlgItem(hDlg, IDC_CHECK_SPHERECOLLISION), BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_SPHERECOLLISION), BM_SETCHECK, BST_CHECKED, 0);
					pView->selectCollisionSphere->SetIsRender(true);
				}
				else
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_SPHERECOLLISION), BM_SETCHECK, BST_UNCHECKED, 0);
					pView->selectCollisionSphere->SetIsRender(false);
				}

			}
		}
			return TRUE;
		case IDC_EDIT_SCALE_SPHERECOLLISION:	//Scale 에디트 박스 변경
		{
			switch (HIWORD(wParam))
			{
				case EN_CHANGE:
				{
					if (pView->selectCollisionSphere != NULL)
					{
						if (pView->EditByCode == FALSE)
						{
							TCHAR scale[128];
							GetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_SPHERECOLLISION), scale, 128);

							float scaling = atof(UTIL::TCHARToChar(scale));
							pView->selectCollisionSphere->SetLocalScale(scaling);
						}
					}
				}
				break;
			}
		}
			return TRUE;
		case IDC_EDIT_TRANSLATE_X_SPHERECOLLISION:	//Translate 에디트박스 변경
		case IDC_EDIT_TRANSLATE_Y_SPHERECOLLISION:
		case IDC_EDIT_TRANSLATE_Z_SPHERECOLLISION:
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				if (pView->selectCollisionSphere != NULL)
				{
					if (pView->EditByCode == FALSE)
					{
						TCHAR posX[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_X_SPHERECOLLISION), posX, 128);
						TCHAR posY[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Y_SPHERECOLLISION), posY, 128);
						TCHAR posZ[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Z_SPHERECOLLISION), posZ, 128);

						float fPosX = atof(UTIL::TCHARToChar(posX));
						float fPosY = atof(UTIL::TCHARToChar(posY));
						float fPosZ = atof(UTIL::TCHARToChar(posZ));
						pView->selectCollisionSphere->SetLocalPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));
					}
				}
			}
				break;
			}
		}
			return TRUE;
		case IDC_BUTTON_ADDSPHERE_SPHERECOLLISION:	//Add Sphere 버튼
		{
			//에디트박스에 있는 문자열을 가져온다
			WCHAR newName[1024];
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_ADDSPHERE_SPHERECOLLISION), newName, 1024);
			
			if (newName[0] == '\0')
			{
				MessageBox(0, L"이름을 입력하세요", L"에러", MB_OK);
			}
			else
			{
				//콜라이더 컴포넌트에 추가한다
				pView->m_pSelectCollider->AddCollisionSphere(UTIL::TCHARToChar(newName));
				SendMessage(GetDlgItem(hDlg, IDC_LIST_SPHERECOLLISION), LB_ADDSTRING, 0, (LPARAM)newName);
			}
		}
			return TRUE;
		case IDC_BUTTON_DELSPHERE_SPHERECOLLISION:	//Del Sphere 버튼
		{
			int index;
			index = SendMessage(GetDlgItem(hDlg, IDC_LIST_SPHERECOLLISION), LB_GETCURSEL, 0, 0);
			pView->m_pSelectCollider->DelCollisionSphere(pView->selectCollisionSphere->GetName());
			pView->selectCollisionSphere = NULL;
			SendMessage(GetDlgItem(hDlg, IDC_LIST_SPHERECOLLISION), LB_DELETESTRING, (WPARAM)index, 0);
		}
			return TRUE;
		case IDC_BUTTON_FINDFRAME_SPHERECOLLISION:	//FindFrame 버튼
		{
			if (pView->selectCollisionSphere != NULL)
				pView->CreateDialogFindFrame(hDlg);
		}
			return TRUE;
		case IDC_BUTTON_DETACH_SPHERECOLLISION: //Detach 버튼
		{
			if (pView->selectCollisionSphere != NULL)
			{
				D3DXMATRIXA16* pMat = pView->m_pSelectObj->GetPWorldMatrix();
				pView->selectCollisionSphere->SetParentMatrix(pMat);
				pView->selectCollisionSphere->SetParentName("NULL");
			}
		}
			return TRUE;
		case IDCANCEL:
		{
			//EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			pView->selectCollisionSphere = NULL;
			hDlg = NULL;
		}
			return TRUE;
		}
		break;
		case WM_NOTIFY:	//Rotate 슬라이더 변경
		{
			if (pView->selectCollisionSphere != NULL)
			{
				if (wParam == IDC_SLIDER_ROTATE_X_SPHERECOLLISION ||
					wParam == IDC_SLIDER_ROTATE_Y_SPHERECOLLISION ||
					wParam == IDC_SLIDER_ROTATE_Z_SPHERECOLLISION)

				{
					int posX = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_X_SPHERECOLLISION), TBM_GETPOS, 0, 0);
					int posY = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Y_SPHERECOLLISION), TBM_GETPOS, 0, 0);
					int posZ = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Z_SPHERECOLLISION), TBM_GETPOS, 0, 0);
					float fPosX = posX / 100.0f;
					float fPosY = posY / 100.0f;
					float fPosZ = posZ / 100.0f;
					pView->selectCollisionSphere->SetLocalRotation(D3DXVECTOR3(fPosX, fPosY, fPosZ));
				}
			}
		}
			return TRUE;
		return TRUE;
	}

	return FALSE;
}
*/

BOOL cColliderComponentView::DlgBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	cColliderComponentView* pView = (cColliderComponentView*)InspectorView->m_vCompontView["5.colliderView"];

	switch (message)
	{
	case WM_INITDIALOG:
	{
		pView->selectCollisionShape = NULL;
		//Rotate 슬라이더 범위지정
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_X_BOXCOLLISION), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Y_BOXCOLLISION), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Z_BOXCOLLISION), TBM_SETRANGE, 0, MAKELPARAM(0, D3DX_PI * 2.0f * 100));

		//Translate Spin버튼 Buddy지정
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_X_BOXCOLLISION), UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_X_BOXCOLLISION), 0);
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Y_BOXCOLLISION), UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Y_BOXCOLLISION), 0);
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Z_BOXCOLLISION), UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Z_BOXCOLLISION), 0);
		//Translate Spin 범위지정
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_X_BOXCOLLISION), UDM_SETRANGE, 0, MAKELPARAM(10000, -10000));
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Y_BOXCOLLISION), UDM_SETRANGE, 0, MAKELPARAM(10000, -10000));
		SendMessage(GetDlgItem(hDlg, IDC_SPIN_TRANSLATE_Z_BOXCOLLISION), UDM_SETRANGE, 0, MAKELPARAM(10000, -10000));


		//list 초기화
		for each(auto iter in *(pView->m_pSelectCollider->GetMapCollision()))
		{
			WCHAR* name = UTIL::CharToTCHAR(iter.first.c_str());
			SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_ADDSTRING, 0, (LPARAM)name);
			free(name);
		}
	}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST_BOXCOLLISION:	//리스트박스 변경
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				pView->EditByCode = TRUE;

				int index;
				wchar_t collisionName[256];
				index = SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_GETTEXT, index, (LPARAM)collisionName);

				char* name = UTIL::TCHARToChar(collisionName);
				//pView->selectCollisionBox = pView->m_pSelectCollider->GetCollisionBox(name);
				pView->selectCollisionShape = pView->m_pSelectCollider->GetCollisionShape(name);
				delete[] name;
				//리스트에서 선택한 항목에 따라 에디트박스 변경

				//체크박스변경
				if (pView->selectCollisionShape->GetIsRender())
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_BOXCOLLISION), BM_SETCHECK, BST_CHECKED, 0);
				}
				else
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_BOXCOLLISION), BM_SETCHECK, BST_UNCHECKED, 0);
				}

				//Scale 에디트 박스 변경
				D3DXVECTOR3 vecScale = pView->selectCollisionShape->GetLocalScale();
				char scale[128];
				wchar_t* temp;

				sprintf(scale, "%.2f", vecScale.x);
				temp = UTIL::CharToTCHAR(scale);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_X_BOXCOLLISION), temp);
				free(temp);
				sprintf(scale, "%.2f", vecScale.y);
				temp = UTIL::CharToTCHAR(scale);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_Y_BOXCOLLISION), temp);
				free(temp);
				sprintf(scale, "%.2f", vecScale.z);
				temp = UTIL::CharToTCHAR(scale);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_Z_BOXCOLLISION), temp);
				free(temp);

				//Rotate 슬라이더 변경
				D3DXVECTOR3 rotation = pView->selectCollisionShape->GetLocalRotation();
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_X_BOXCOLLISION), TBM_SETPOS, TRUE, rotation.x * 100);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Y_BOXCOLLISION), TBM_SETPOS, TRUE, rotation.y * 100);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Z_BOXCOLLISION), TBM_SETPOS, TRUE, rotation.z * 100);

				//Translate 에디트 박스 변경
				D3DXVECTOR3 position = pView->selectCollisionShape->GetLocalPosition();
				char pos[128];
				
				sprintf(pos, "%.2f", position.x);
				temp = UTIL::CharToTCHAR(pos);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_X_BOXCOLLISION), temp);
				free(temp);
				sprintf(pos, "%.2f", position.y);
				temp = UTIL::CharToTCHAR(pos);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Y_BOXCOLLISION), temp);
				free(temp);
				sprintf(pos, "%.2f", position.z);
				temp = UTIL::CharToTCHAR(pos);
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Z_BOXCOLLISION), temp);
				free(temp);

				pView->EditByCode = FALSE;
			}
				return TRUE;
			}
		}
			return TRUE;
		case IDC_CHECK_BOXCOLLISION:	//체크박스 변경
		{
			if (pView->selectCollisionShape != NULL)
			{
				if (SendMessage(GetDlgItem(hDlg, IDC_CHECK_BOXCOLLISION), BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_BOXCOLLISION), BM_SETCHECK, BST_CHECKED, 0);
					pView->selectCollisionShape->SetIsRender(true);
				}
				else
				{
					SendMessage(GetDlgItem(hDlg, IDC_CHECK_BOXCOLLISION), BM_SETCHECK, BST_UNCHECKED, 0);
					pView->selectCollisionShape->SetIsRender(false);
				}

			}
		}
			return TRUE;
		case IDC_EDIT_SCALE_X_BOXCOLLISION:	//Scale 에디트 박스 변경
		case IDC_EDIT_SCALE_Y_BOXCOLLISION:
		case IDC_EDIT_SCALE_Z_BOXCOLLISION:
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				if (pView->selectCollisionShape != NULL)
				{
					if (pView->EditByCode == FALSE)
					{
						char* temp;
						TCHAR scale[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_X_BOXCOLLISION), scale, 128);
						temp = UTIL::TCHARToChar(scale);
						float scaleX = atof(temp);
						delete[] temp;
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_Y_BOXCOLLISION), scale, 128);
						temp = UTIL::TCHARToChar(scale);
						float scaleY = atof(temp);
						delete[] temp;
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_SCALE_Z_BOXCOLLISION), scale, 128);
						temp = UTIL::TCHARToChar(scale);
						float scaleZ = atof(temp);
						delete[] temp;
						
						if (pView->selectCollisionShape->GetShapeType() == ENGINE::eSPHERE_SHAPE)
						{
							float bigScale = scaleX;
							if (bigScale < scaleY) bigScale = scaleY;
							if (bigScale < scaleZ) bigScale = scaleZ;

							pView->selectCollisionShape->SetLocalScale(D3DXVECTOR3(bigScale, bigScale, bigScale));
						}
						else
						{
							pView->selectCollisionShape->SetLocalScale(D3DXVECTOR3(scaleX, scaleY, scaleZ));
						}
					}
				}
			}
				break;
			}
		}
			return TRUE;
		case IDC_EDIT_TRANSLATE_X_BOXCOLLISION:	//Translate 에디트박스 변경
		case IDC_EDIT_TRANSLATE_Y_BOXCOLLISION:
		case IDC_EDIT_TRANSLATE_Z_BOXCOLLISION:
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				if (pView->selectCollisionShape != NULL)
				{
					if (pView->EditByCode == FALSE)
					{
						TCHAR posX[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_X_BOXCOLLISION), posX, 128);
						TCHAR posY[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Y_BOXCOLLISION), posY, 128);
						TCHAR posZ[128];
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_TRANSLATE_Z_BOXCOLLISION), posZ, 128);

						char* temp;
						temp = UTIL::TCHARToChar(posX);
						float fPosX = atof(temp);
						delete[] temp;
						temp = UTIL::TCHARToChar(posY);
						float fPosY = atof(temp);
						delete[] temp;
						temp = UTIL::TCHARToChar(posZ);
						float fPosZ = atof(temp);
						delete[] temp;
						pView->selectCollisionShape->SetLocalPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));
					}
				}
			}
				break;
			}
		}
			return TRUE;
		case IDC_BUTTON_ADDBOX_BOXCOLLISION:	//Add Box 버튼
		{
			//에디트박스에 있는 문자열을 가져온다
			WCHAR newName[1024];
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_ADDBOX_BOXCOLLISION), newName, 1024);

			if (newName[0] == '\0')
			{
				MessageBox(0, L"이름을 입력하세요", L"에러", MB_OK);
			}
			else
			{
				//콜라이더 컴포넌트에 추가한다
				char* temp = UTIL::TCHARToChar(newName);
				if (!pView->m_pSelectCollider->AddCollisionBox(temp))
				{
					MessageBox(0, TEXT("AddFaild"), TEXT("에러"), MB_OK);
				}
				else
				{
					SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_ADDSTRING, 0, (LPARAM)newName);
				}
				delete[] temp;
				
			}
		}
			return TRUE;
		case IDC_BUTTON_ADDSPHERE_BOXCOLLISION2:	//Add Sphere버튼
		{
			//에디트박스에 있는 문자열을 가져온다
			WCHAR newName[1024];
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_ADDBOX_BOXCOLLISION), newName, 1024);

			if (newName[0] == '\0')
			{
				MessageBox(0, L"이름을 입력하세요", L"에러", MB_OK);
			}
			else
			{
				//콜라이더 컴포넌트에 추가한다
				char* temp = UTIL::TCHARToChar(newName);
				if (!pView->m_pSelectCollider->AddCollisionSphere(temp))
				{
					MessageBox(0, TEXT("AddFaild"), TEXT("에러"), MB_OK);
				}
				else
				{
					SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_ADDSTRING, 0, (LPARAM)newName);
				}
				delete[] temp;

			}
		}
			return TRUE;
		case IDC_BUTTON_DELBOX_BOXCOLLISION:	//Del Box 버튼
		{
			int index;
			index = SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_GETCURSEL, 0, 0);
			pView->m_pSelectCollider->DelCollision(pView->selectCollisionShape->GetName());
			pView->selectCollisionShape = NULL;
			SendMessage(GetDlgItem(hDlg, IDC_LIST_BOXCOLLISION), LB_DELETESTRING, (WPARAM)index, 0);
		}
			return TRUE;
		case IDC_BUTTON_FINDFRAME_BOXCOLLISION:	//FindFrame 버튼
		{
			if (pView->selectCollisionShape != NULL)
				pView->CreateDialogFindFrame(hDlg);
		}
			return TRUE;
		case IDC_BUTTON_DETACH_BOXCOLLISION: //Detach 버튼
		{
			if (pView->selectCollisionShape != NULL)
			{
				D3DXMATRIXA16* pMat = pView->m_pSelectObj->GetPWorldMatrix();
				pView->selectCollisionShape->SetParentMatrix(pMat);
				pView->selectCollisionShape->SetParentName("NULL");
			}
		}
			return TRUE;
		case IDCANCEL:
		{
			//EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			pView->selectCollisionShape = NULL;
			hDlg = NULL;
		}
			return TRUE;
		}
		break;
	case WM_NOTIFY:	//Rotate 슬라이더 변경
	{
		if (pView->selectCollisionShape != NULL)
		{
			if (wParam == IDC_SLIDER_ROTATE_X_BOXCOLLISION ||
				wParam == IDC_SLIDER_ROTATE_Y_BOXCOLLISION ||
				wParam == IDC_SLIDER_ROTATE_Z_BOXCOLLISION)

			{
				int posX = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_X_BOXCOLLISION), TBM_GETPOS, 0, 0);
				int posY = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Y_BOXCOLLISION), TBM_GETPOS, 0, 0);
				int posZ = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_ROTATE_Z_BOXCOLLISION), TBM_GETPOS, 0, 0);
				float fPosX = posX / 100.0f;
				float fPosY = posY / 100.0f;
				float fPosZ = posZ / 100.0f;
				pView->selectCollisionShape->SetLocalRotation(D3DXVECTOR3(fPosX, fPosY, fPosZ));
			}
		}
	}
		return TRUE;
		return TRUE;
	}

	return FALSE;
}

BOOL cColliderComponentView::DlgFindFrameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	cColliderComponentView* pView = (cColliderComponentView*)InspectorView->m_vCompontView["5.colliderView"];

	switch (message)
	{
	case WM_INITDIALOG:
	{
		std::map<std::string, D3DXMATRIXA16>* pvBipMatrixMap;
		std::map<std::string, D3DXMATRIXA16>::iterator iter;
		TCHAR* name;
		pvBipMatrixMap = pView->m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE)->GetBipMatrixMap();
		int i = 0;
		for (iter = pvBipMatrixMap->begin(); iter != pvBipMatrixMap->end(); ++iter)
		{
			name = UTIL::CharToTCHAR(iter->first.c_str());
			SendMessage(GetDlgItem(hDlg, IDC_LIST_FINDFRAME), LB_ADDSTRING, i, (LPARAM)name);
			delete name;
			i++;
		}
	}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			int index;
			wchar_t frameName[256];
			index = SendMessage(GetDlgItem(hDlg, IDC_LIST_FINDFRAME), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LIST_FINDFRAME), LB_GETTEXT, index, (LPARAM)frameName);

			if (index == -1)	//리스트박스에서 선택된 것이 없을때
			{
				MessageBox(0, L"선택된 리스트가 없습니다", L"에러", MB_OK);
			}
			else
			{

				char* szFrameName = UTIL::TCHARToChar(frameName);

				ENGINE::cSkinnedMeshComponent* skinnedComp = pView->m_pSelectObj->GetComponent<ENGINE::cSkinnedMeshComponent*>(ENGINE::eSKINNED_MESH_TYPE);
				ENGINE::cFbxComponent* fbxComp = pView->m_pSelectObj->GetComponent<ENGINE::cFbxComponent*>(ENGINE::eFBX_MESH_TYPE);
				if (skinnedComp != NULL)
				{
					D3DXMATRIXA16* pMat = skinnedComp->GetMatrixFromeFrame(szFrameName);
					
					if (pView->selectCollisionShape != NULL)
					{
						pView->selectCollisionShape->SetParentMatrix(pMat);
						pView->selectCollisionShape->SetParentName(szFrameName);
					}

					//if (pView->selectCollisionSphere != NULL)
					//{
					//	pView->selectCollisionSphere->SetParentMatrix(pMat);
					//	pView->selectCollisionSphere->SetParentName(szFrameName);
					//}
					//else if (pView->selectCollisionBox != NULL)
					//{
					//	pView->selectCollisionBox->SetParentMatrix(pMat);
					//	pView->selectCollisionBox->SetParentName(szFrameName);
					//}
				}
				else if (fbxComp != NULL)
				{
					D3DXMATRIXA16* pMat = fbxComp->GetMatrixFromeFrame(szFrameName);
					
					if (pView->selectCollisionShape != NULL)
					{
						pView->selectCollisionShape->SetParentMatrix(pMat);
						pView->selectCollisionShape->SetParentName(szFrameName);
					}
					
					//if (pView->selectCollisionSphere != NULL)
					//{
					//	pView->selectCollisionSphere->SetParentMatrix(pMat);
					//	pView->selectCollisionSphere->SetParentName(szFrameName);
					//}
					//else if (pView->selectCollisionBox != NULL)
					//{
					//	pView->selectCollisionBox->SetParentMatrix(pMat);
					//	pView->selectCollisionBox->SetParentName(szFrameName);
					//}
				}

				delete[] szFrameName;

				DestroyWindow(hDlg);
				hDlg = NULL;
			}
		}
			return TRUE;
		case IDCANCEL:
		{
			//EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			hDlg = NULL;
		}
			return TRUE;
		}
		break;
	
		return TRUE;
	}

	return FALSE;
}