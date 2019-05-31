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
	void InsertTreeNode(HTREEITEM pNode, ENGINE::GameObject* obj);//노드추가
	HTREEITEM GetTreeNode(ENGINE::GameObject* obj);//obj로 노드찾기
	void SelectTreeNode(HTREEITEM Node);//Node선택된것으로 표시
	void SelectTreeNode(ENGINE::GameObject* obj);//obj 선택된것으로 표시
	HTREEITEM NowSelectedTreeNode(); //현재 선택되있는 노드(선택된 것이 없으면 NULL리턴)
	void DeleteTreeNode(HTREEITEM Node);//노드삭제
	ENGINE::GameObject* PickGameObjectFromTreeNode(HTREEITEM Node);//노드에서 GameObject가져오기
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