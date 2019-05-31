#pragma once

#include "cComponentBase.h"

class cScriptComponentView : public cComponentBase
{
private:
	ENGINE::GameObject* m_pSelectObj;
	ENGINE::cScriptComponent* m_pSelectScript;

	HWND m_hEditButton;		//스크립트 편집 버튼
	HWND m_hLoadButton;		//스크립트 로드 버튼
	HWND m_hPathEditBox;	//스크립트 경로 출력 에디트박스

	HWND m_hScriptEditWin;	//스크립트 편집 윈도우
	HWND m_hScriptEditBox;	//스크립트 편집 하는 editBox
	enum
	{
		ID_EDIT_BUTTON = 0,
		ID_LOAD_BUTTON,
		ID_PATH_EDITBOX,
		ID_SCRIPT_EDIT_WINDOW,
		ID_SCRIPT_EDIT_BOX
	};
public:
	cScriptComponentView();
	~cScriptComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK EditWindowProc(HWND, UINT, WPARAM, LPARAM);
public:
	void InitScriptComponentView(ENGINE::GameObject* pSelectObj);
	void drawText(HDC hdc);
	void GetRelativePath(std::wstring inPath, char* outPath);
public:
	void EditButtonEvent();
	void LoadButtonEvent();
	void LoadScriptData();
	void SaveScriptData();
};

