#pragma once
#include "cComponentBase.h"
#include <stdlib.h>



class cAniComponentView : public cComponentBase
{
public:
	//���õ� ���� ������Ʈ�� �ִ� ������Ʈ
	ENGINE::cAnimComponent* m_pAniComponent;
	
	//�߰��ϴ� �����͸� �ִ� �ڽ� ������ �ڵ�
	HWND m_hClildAddWin;
	HWND m_hNameEdit;
	HWND m_hIndexEdit;
	HWND m_hStartEdit;
	HWND m_hEndEdit;
	HWND m_hIsLoopEdit;
	HWND m_hOkButton;
	//����Ʈ �� �ڵ�
	HWND m_hListView;
	//�ִ�Ŭ�� �߰� ��ư �ڵ�,ID
	HWND m_hAddAniClipButton;
	int addAniClipButtonID = 1000;
	//�ִ�Ŭ�� ���� ��ư �ڵ�,ID
	HWND m_hDeleteAniClipButton;
	int deleteAniClipButtonID = 1001;
	//�ִ�Ŭ�� �÷��� ��ư �ڵ�,ID
	HWND m_hPlayAniClipButton;
	int playAniClipButtonID = 1002;
public:
	cAniComponentView();
	~cAniComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

	void SetAniValue(ENGINE::cAnimComponent* pAniComponent);

	void drawText(HDC hdc);
public:
	//list�� �߰� ���� �Լ�
	void AddClipList(std::string name, int index, int start, int end, bool isLoop);
	void DeleteClipList();

	//���� ������Ʈ �ִ� �����Ϳ� �߰� ���� �÷��� �Լ�
	void Addbutton();
	void AddAniClip(std::string name, int index, int start, int end, bool isLoop);
	void DeleteAniClip(TCHAR* name);
	void PlayClip(TCHAR* name);
};

