#pragma once
#include "cBaseCustomWindow.h"

class cProjectView : public cBaseCustomWindow
{
public:
	cProjectView();
	~cProjectView();

	virtual	BOOL Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	void Setup();


	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


	enum
	{
		ID_LIST = 0,
		ID_BUTTON_LOADFILE,
		ID_BUTTON_INSTANTIATE
	};
};