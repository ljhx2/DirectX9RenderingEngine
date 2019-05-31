#pragma once
#include "cComponentBase.h"
#include <stdlib.h>



class cAniComponentView : public cComponentBase
{
public:
	//선택된 게임 오브젝트의 애니 컴포넌트
	ENGINE::cAnimComponent* m_pAniComponent;
	
	//추가하는 데이터를 넣는 자식 윈도우 핸들
	HWND m_hClildAddWin;
	HWND m_hNameEdit;
	HWND m_hIndexEdit;
	HWND m_hStartEdit;
	HWND m_hEndEdit;
	HWND m_hIsLoopEdit;
	HWND m_hOkButton;
	//리스트 뷰 핸들
	HWND m_hListView;
	//애니클립 추가 버튼 핸들,ID
	HWND m_hAddAniClipButton;
	int addAniClipButtonID = 1000;
	//애니클립 삭제 버튼 핸들,ID
	HWND m_hDeleteAniClipButton;
	int deleteAniClipButtonID = 1001;
	//애니클립 플레이 버튼 핸들,ID
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
	//list에 추가 삭제 함수
	void AddClipList(std::string name, int index, int start, int end, bool isLoop);
	void DeleteClipList();

	//게임 오브젝트 애니 데이터에 추가 삭제 플레이 함수
	void Addbutton();
	void AddAniClip(std::string name, int index, int start, int end, bool isLoop);
	void DeleteAniClip(TCHAR* name);
	void PlayClip(TCHAR* name);
};

