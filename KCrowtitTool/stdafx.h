// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS			
#define _CRT_NON_CONFORMING_SWPRINTFS		
//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//======================================================
//������� ���� 
//======================================================
#include <d3dx9.h>					//d3d�� radian -> degree �Լ� �����Ƿ� �������� �ʰڴ�!

/*
//���ǵ� �Լ���
#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DX_1BYPI ((FLOAT)  0.318309886f)

#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))
*/

#include <assert.h>
#include <map>			//��
#include <vector>		//���� ��
#include <mmsystem.h>	//TimeGetTime �Լ��� ����ϱ� ���� ���̺귯��
#include <time.h>		//srand �Լ��� ����ϱ� ���� 
#include <algorithm>
#include <fstream>
#include <string>
#include <CommCtrl.h>	//Ʈ����

#include <D3DRenderer.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#include "willMakeDLL.h"



//======================================================
// Define ����
//======================================================
#define TITLENAME (LPTSTR)(TEXT("ENGINE Project"))

#define FRAME_STANDARD_RATE 60		//1�ʵ��� 60 �������� �������� �Ѵ�. 
#define WINSIZE_X 1000
#define WINSIZE_Y 700
/*
#define WINSIZE_X 1440
#define WINSIZE_Y 900

#define WINSIZE_X 1000
#define WINSIZE_Y 700
--------------------------------------
Resolution		Pixels		Widescreen
--------------------------------------
800 x 600		480,000			No
1024 x 768		786,432			No
1152 x 864		995,328			No
1280 x 1024		1,310,720		No
1600 x 1200		1,920,000		No
1440 x 900		1,296,000		Yes
1680 x 1050		1,764,000		Yes
1920 x 1080		2,073,600		Yes
1920 x 1200		2,304,000		Yes

*/

//���� ��ũ�� ���� 
#define SCREENSIZE_X GetSystemMetrics(SM_CXSCREEN)
#define SCREENSIZE_Y GetSystemMetrics(SM_CYSCREEN)

/*
SM_CLEANBOOT : �ý����� ��� ���õǾ������� �����Ѵ�. 0�̸� ���� ���� ���õ� ���̰� 1�̸� ������ ���� ������ ���̰� 2�̸� ��Ʈ�� ������ �ִ� ������ ���

SM_CMONITORS : ����ũž�� ����� ����

SM_CMOUSEBUTTONS : ���콺�� ��ư ����. 0�̸� ���콺�� ��ġ�Ǿ� ���� �ʴ�

SM_CXBORDER, SM_CYBORDER : ������ ��輱�� ���� ���̸� �ȼ� ������ �����Ѵ�. 3D ��Ÿ���� SM_CXEDGE�� �����ϴ�.

SM_CXDRAG, SM_CYDRAG : ���� ���콺 ��ư�� ���� �� �巡�׸� �����ϱ� ���� �������� �ϴ� �Ÿ��� �����Ѵ�.

SM_CXHSCROLL, SM_CYHSCROLL : ��ũ�� ���� ȭ��ǥ ��Ʈ���� ���� ����.

SM_CXICON, SM_CYICON : �������� ���� ����. LoadIcon�� �� ũ���� �����ܸ� ���� �� �ִ�.
*/

//�̱��� 
#define SINGLETONE(class_name) private: class_name(void); ~class_name(void);\
public: static class_name* GetInstance() { static class_name instance; return &instance; }


#define SAFE_DELETE(p)			{ if(p) {delete (p);	p = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{ if(p) {delete [] p;	p = NULL;}}
#define SAFE_RELEASE(p)			{ if(p) {p->Release();	p = NULL;}}

#define SAFE_DESTROY_WINDOW(hWnd)	{ if(hWnd) { DestroyWindow(hWnd); hWnd = NULL; } }

#define SAFE_ADD_REF(p) {if(p){p->AddRef();}}
//get & set �޾��ִ� �Լ��� �ڵ����� ����
#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName; \
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

//���� �Ƚ���� ����. 
/*
#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){
\
if (varName != var) {
	\
	SAFE_ADD_REF(var); \
	SAFE_RELEASE(varName); \
	varName = var; \
}\
}
*/
//���콺 ��ǥ 
//extern POINT g_ptMouse;
//extern BOOL g_bUseMouseWheel;
extern short g_WheelDeltaWPARAM;

//�ڵ鰪 ��ü 
extern HWND	g_hWnd;
extern HWND	g_hwndGame;
extern HWND g_hwndHierarchy;
extern HWND g_hwndProject;
extern HWND g_hwndDebug;
extern HWND g_hwndInspector;

//��� View���� ��ü ������
#include "cHierarchyView.h"
#include "cGameView.h"
#include "cProjectView.h"
#include "cInspectorView.h"
#include "cDebugView.h"
extern cHierarchyView * HierarchyView;
extern cGameView * GameView;
extern cProjectView * ProjectView;
extern cDebugView * DebugView;
extern cInspectorView * InspectorView;

#include"cMainGame.h"
extern cMainGame * g_MainGame;

#include "cWorkingCamera.h"

//������ ������ ����
//3�������� ���ǵ� ���ϱ��� ������
//2���� ȭ�� ���ۿ� ������ �ϱ� ���� �ʿ��� ��� ��� / ����Ʈ ���� ���� ����

//FVF = Flexible Vertex Format
//FVF�� D3D���� ������ �ִ� ������ ���� �߿��� �ڽ��� �����ϴ� ���α׷��� �˸��� �͵��� ��Ƽ� ���ο� ���� ������ ���ϴ� ����̴�.
//
//D3DFVF_XYZ		-> 3���� ���������� ������ ��ǥ
//D3DFVF_XYZRHW	-> 2���� ���������� ���� ��ǥ D3D�� �̿��� 2D�� ǥ���� �� ���
//D3DFVF_DIFFUSE ������ ��
//D3DFVF_SPECULAR ������ �ݻ簪
//
////������ �Ӽ�
//const DWORD ST_VERTEX::FVF = ...; ����ƽ�� �����ϰ� �ʱ�ȭ�ؾ��Ѵ�.
//������Ͽ��� �ʱ�ȭ�ϸ� ������ �ʱ�ȭ �ϱ� ������ ������ ������ ����.
//���� ������ ȣ����� �ʴ� ��(cpp)���� �ʱ�ȭ�ؾ� �Ѵ�.

inline LONG RectHeight(const RECT& r) { return r.bottom - r.top; }
inline LONG RectWidth(const RECT& r) { return r.right - r.left; }
inline POINT RectCenter(const RECT& r)
{
	POINT pt; pt.x = r.left + RectWidth(r) / 2; pt.y = r.top + RectHeight(r) / 2; return pt;
}

//���콺 ��ǥ 
inline POINT GetCursorPos() { POINT pt; GetCursorPos(&pt); return pt; }




//===================================
// �̱���
//===================================
#include "cSceneManager.h"
#include <DeviceMgr.h>

//����׿�
#include "cDebugLog.h"
