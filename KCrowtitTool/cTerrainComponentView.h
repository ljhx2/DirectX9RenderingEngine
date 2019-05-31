#pragma once
#include "cComponentBase.h"
#include <hash_map>

#define MAX_TILE_NUM 27
class cTerrainComponentView : public cComponentBase
{
private:
	enum E_EDIT_BRUSH_OPTION
	{
		TEX_EDIT,
		HEIGHT_EDIT,
		OBJECT_EDIT
	};
	typedef struct tagImageInfo
	{
		DWORD			 resID;
		HDC				 hMemDC;
		HBITMAP			 hBit;
		HBITMAP			 hOBit;
		int				 width;
		int				 height;

		tagImageInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			width = 0;
			height = 0;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

public:
	cTerrainComponentView();
	~cTerrainComponentView();

	virtual	BOOL Initialize(LPCTSTR name, HINSTANCE hInstance);
	virtual VOID Uninitialize(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	//view 
	static BOOL CALLBACK Dlg1Proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); //다이얼로그

	void InitTerrainComponentView(ENGINE::GameObject* pSelectObj);
private:
	std::vector<std::wstring> m_vecDirectory;
	stdext::hash_map<std::wstring, IMAGE_INFO*>m_mapTextureList;
private:
	void SettingTexEdit();
	void SettingSize(int row, int col, int cell);
	void SettingListView(HWND hlist, const TCHAR * str);
	void PaintDlg(HWND hWnd);
	void HScroll(HWND hWnd, LPARAM lParam);
	void Notify(HWND hWnd, LPARAM lParam);

	void ChangeBrushOption(E_EDIT_BRUSH_OPTION opt);

	/*다이얼로그 시작*/
	void drawText(HDC hdc);
	void CreateDlg(HWND hWnd);
	void InitDlg(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void CommandDlg(HWND hWnd, WPARAM wParam, LPARAM lParam);
private :
	IMAGE_INFO * m_pImageInfo;
	ENGINE::GameObject* m_pSelectObj;
	ENGINE::cTerrainData * m_pTerrain;
	HWND m_dlghWnd;

	bool IsDlgEnd;

	int m_nX;
	int m_nY;
	int m_nCell;

	float m_fVertexRadius;
	float m_fHeight;
	float m_fPaintRadius;
	int	 m_nPriodFirst;
	int	 m_nPriodSecond;

	int m_nCurrentTexIndex;
	TCHAR m_szSelectedTex[8][18];
};