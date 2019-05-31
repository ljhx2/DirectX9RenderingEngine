#pragma once
#include "cBaseCustomWindow.h"


class cHierarchyView : public cBaseCustomWindow
{
public:
	cHierarchyView();
	~cHierarchyView();
	virtual	BOOL Initialize(LPCTSTR name, HWND hwnd, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	void ResetTreeNode(ENGINE::GameObject* obj);
	void InsertTreeNode(HTREEITEM pNode, ENGINE::GameObject* obj);//����߰�
	HTREEITEM GetTreeNode(ENGINE::GameObject* obj);//obj�� ���ã��
	void SelectTreeNode(HTREEITEM Node);//Node���õȰ����� ǥ��
	void SelectTreeNode(ENGINE::GameObject* obj);//obj ���õȰ����� ǥ��
	HTREEITEM NowSelectedTreeNode(); //���� ���õ��ִ� ���(���õ� ���� ������ NULL����)
	void DeleteTreeNode(HTREEITEM Node);//������
	ENGINE::GameObject* PickGameObjectFromTreeNode(HTREEITEM Node);//��忡�� GameObject��������
	void Setup();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	enum
	{
		ID_TREE = 0,
		ID_BUTTON_DELETE,
		ID_BUTTON_SAVE,
	};

	std::map<ENGINE::GameObject*, HTREEITEM> m_mapNode;

};