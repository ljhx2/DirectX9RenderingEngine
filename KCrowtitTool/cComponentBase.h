#pragma once

class cComponentBase
{
public:
	LPCTSTR WC_COMPONENTWINDOW;

	HWND		m_hWnd = NULL;
	HINSTANCE	m_hInstance = NULL;

	int m_winWidth;
	int m_winHeight;
public:
	cComponentBase();
	~cComponentBase();

	HWND CreateCustomWindow(LPCTSTR sName, HWND hWndHost, INT startX, INT startY);
	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance) = 0;
	virtual VOID Uninitialize(HINSTANCE hInstance) = 0;
public:
	void SetHWND(HWND hWnd){ m_hWnd = hWnd; }
	void ChangeWindowSize(int x, int y, int width, int height);
};

