#pragma once
#include "cComponentBase.h"


class cColliderComponentView : public cComponentBase
{
private:
	ENGINE::GameObject* m_pSelectObj;
	ENGINE::cColliderComponent* m_pSelectCollider;
	ENGINE::cCollisionShape* selectCollisionShape;


	HWND m_hCheck;	//üũ�ڽ�
	//HWND m_hButtonSphere;	//���Ǿ� ��ư
	HWND m_hButtonBox;		//�ڽ� ��ư
	HWND m_hRadio1;	//������ư
	HWND m_hRadio2;

	//HWND m_hDlgSphere;	//���Ǿ� �ݸ��� ��ȭ����
	HWND m_hDlgBox;		//�ڽ� �ݸ��� ��ȭ����

	HWND m_hDlgFindFrame;	//Find Frame ��ȭ����


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

