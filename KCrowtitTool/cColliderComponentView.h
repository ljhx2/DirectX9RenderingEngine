#pragma once
#include "cComponentBase.h"


class cColliderComponentView : public cComponentBase
{
private:
	ENGINE::GameObject* m_pSelectObj;
	ENGINE::cColliderComponent* m_pSelectCollider;
	ENGINE::cCollisionShape* selectCollisionShape;


	HWND m_hCheck;	//체크박스
	//HWND m_hButtonSphere;	//스피어 버튼
	HWND m_hButtonBox;		//박스 버튼
	HWND m_hRadio1;	//라디오버튼
	HWND m_hRadio2;

	//HWND m_hDlgSphere;	//스피어 콜리젼 대화상자
	HWND m_hDlgBox;		//박스 콜리전 대화상자

	HWND m_hDlgFindFrame;	//Find Frame 대화상자


	BOOL EditByCode;
	//ID
	enum
	{
		ID_CHECKBOX = 0,
		//ID_BUTTONSPHERE,
		ID_BUTTONBOX,
		ID_RADIO1,
		ID_RADIO2
	};
public:
	


public:
	cColliderComponentView();
	~cColliderComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	//static BOOL CALLBACK DlgSphereProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DlgBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DlgFindFrameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void drawText(HDC hdc);
	void SetColliderValue(ENGINE::GameObject* pSelectObj);
	//void CreateDialogSphere(HWND hWnd);
	void CreateDialogBox(HWND hWnd);
	void CreateDialogFindFrame(HWND hWnd);



};

