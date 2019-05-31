#pragma once
#include "cBaseCustomWindow.h"
#include "cComponentBase.h"
#include "cTransformView.h"
#include "cStaticMeshComponentView.h"
#include "cFbxComponentView.h"
#include "cAniComponentView.h"
#include "cColliderComponentView.h"
#include "cCameraComponentView.h"
#include "cTerrainComponentView.h"
#include "cScriptComponentView.h"
#include "cSkyBoxComponentView.h"

class cInspectorView : public cBaseCustomWindow
{
public:
	HWND m_hPrefabSaveButton;
	int m_idPrefabButton = 101;
public:
	cInspectorView();
	~cInspectorView();

	virtual	BOOL Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);
	virtual HWND CreateCustomWindow(LPCTSTR sName, HWND hWndHost, INT width, INT height, int resizability);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	void SetSelectGameObject(ENGINE::GameObject* pSelectGameObject);

	ENGINE::GameObject* m_pSelectGameObject;
	std::map<std::string, cComponentBase*> m_vCompontView;
	//std::vector<cComponentBase*>			m_vCompontView;
public:
	void SavePrefab();
};