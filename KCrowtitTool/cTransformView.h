#pragma once
#include "cComponentBase.h"

class cTransformView : public cComponentBase
{
private:
	ENGINE::GameObject* m_pSelectObj;

	HWND m_hNameListBox;
	HWND m_hTranslateListBox[3];
	//HWND m_hRotationListBox[3];

	HWND m_hRotationTrackBar[3];

	HWND m_hScaleListBox[3];
	HWND m_hUpdateButton;

	HWND m_tagComboBox;
	enum{
		TAG_COMBOBOX = 0
	};
public:
	cTransformView();
	~cTransformView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	void drawText(HDC hdc);
	//void SetTransformValue(std::wstring name, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	void SetTransformValue(ENGINE::GameObject* pSelectObj);
	void UpdateTranslateButton();
	//게임 오브젝트 태그 값을 int로 리턴해준다
	int GetGameObjectTagNumber();
	void SetGameObjectTag(int tagNumber);
};

