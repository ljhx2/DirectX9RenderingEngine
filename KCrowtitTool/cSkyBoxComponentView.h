#pragma once
#include "cComponentBase.h"

class cSkyBoxComponentView : public cComponentBase
{
public :
	cSkyBoxComponentView();
	~cSkyBoxComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	//view 
	void InitSkyBoxComponentView(ENGINE::GameObject* pSelectObj);

private :
	ENGINE::GameObject* m_pSelectObj;
	HWND m_hLoadButton;
	HWND m_hSaveButton;

	void InitSkyBox();
	void drawText(HDC hdc);
	void LoadSkyBox();
	void SaveSkyBox();
	enum
	{
		ID_SKYBOX_LOAD_BUTTON = 0,
		ID_SKYBOX_SAVE_BUTTON,
	};
};