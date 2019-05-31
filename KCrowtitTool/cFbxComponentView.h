#pragma once
#include "cComponentBase.h"

class cFbxComponentView : public cComponentBase
{
private:
	ENGINE::GameObject* m_pSelectObj;
public:
	HWND m_hHierarchyButton;
	int m_idHierarchyButton = 4001;

	HWND m_hMeterialListView;
	int m_idMeterialListView = 4004;
	HWND m_hMeterialSearchButton;
	int m_idMeterialSearchButton = 4005;

	HWND m_hMeterialDiffuseEditBox[4];
	int m_idMeterialDiffuseEditBox[4];//4006~4009

	HWND m_hMeterialDiffuseUpdateButton;
	int m_idMeterialDiffuseUpdateButton = 4010;


	// ----���̶�Ű â ������
	HWND m_hHierarchyWin;
	int m_idHierarchyWin = 4000;

	HWND m_hBipListBox;
	int m_idBipListBox = 4002;

	HWND m_hOkButton;
	int m_idOkButton = 4003;
	HWND m_hCencleButton;
	int m_idCencleButton = 4004;

public:
	cFbxComponentView();
	~cFbxComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK HierarchyProc(HWND, UINT, WPARAM, LPARAM);

	void InitFbxComponentView(ENGINE::GameObject* pSelectObj);
	void drawText(HDC hdc);

	void CreateHierarchyWindow();
	void AddMeterialName(std::string name);
	void AddMeterialPath();
public:
	//���� ���� ��θ� ����η� �ٲپ��ش�
	void GetRelativePath(std::wstring inPath, char* outPath);
	//���õ� ��������Ʈ diffuse���� ����ϴ� �Լ�
	void SelectMeterialDiffuseUpdate();
};

