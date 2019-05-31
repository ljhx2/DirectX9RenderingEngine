#pragma once
#include "cComponentBase.h"
class cStaticMeshComponentView : public cComponentBase
{
public:
	cStaticMeshComponentView();
	~cStaticMeshComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	void SetTransformValue(std::wstring name, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
};

