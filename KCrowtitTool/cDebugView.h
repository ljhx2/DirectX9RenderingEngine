#pragma once
#include "cBaseCustomWindow.h"


class cDebugView : public cBaseCustomWindow
{
public:
	cDebugView();
	~cDebugView();

	virtual	BOOL Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
public :
	void InitList(HWND hWnd, WPARAM wParam, LPARAM lParam);
private :
	
};