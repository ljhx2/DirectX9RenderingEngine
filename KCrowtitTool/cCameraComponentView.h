#pragma once
#include "cComponentBase.h"

class cCameraComponentView : public cComponentBase
{
private:
	ENGINE::GameObject* m_pSelectObj;
	ENGINE::cCameraComponent* m_pSelectCamera;

	HWND m_hButton;

	HWND m_hDlgCamera;

	BOOL EditByCode;

	enum
	{
		ID_BUTTON = 0
	};

public:
	cCameraComponentView();
	~cCameraComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


	void drawText(HDC hdc);
	void SetCameraValue(ENGINE::GameObject* pSelectObj);
	void CreateDialogCamera(HWND hWnd);
};

