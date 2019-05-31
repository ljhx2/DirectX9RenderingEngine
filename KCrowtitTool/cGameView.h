#pragma once
#include "cBaseCustomWindow.h"


class cGameView : public cBaseCustomWindow
{
public :
	cGameView();
	~cGameView();

	virtual	BOOL Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	void Setup();

private:
	static LRESULT CALLBACK cGameView::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

};