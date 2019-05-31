// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS			
#define _CRT_NON_CONFORMING_SWPRINTFS		
//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//======================================================
//헤더파일 정의 
//======================================================
#include <d3dx9.h>					//d3d에 radian -> degree 함수 있으므로 정의하지 않겠다!

/*
//정의된 함수들
#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DX_1BYPI ((FLOAT)  0.318309886f)

#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))
*/

#include <assert.h>
#include <map>			//맵
#include <vector>		//벡터 ㄴ
#include <mmsystem.h>	//TimeGetTime 함수를 사용하기 위한 라이브러리
#include <time.h>		//srand 함수를 사용하기 위해 
#include <algorithm>
#include <fstream>
#include <string>
#include <CommCtrl.h>	//트리뷰

#include <D3DRenderer.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#include "willMakeDLL.h"



//======================================================
// Define 정의
//======================================================
#define TITLENAME (LPTSTR)(TEXT("ENGINE Project"))

#define FRAME_STANDARD_RATE 60		//1초동안 60 프레임을 기준으로 한다. 
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

//실제 스크린 길이 
#define SCREENSIZE_X GetSystemMetrics(SM_CXSCREEN)
#define SCREENSIZE_Y GetSystemMetrics(SM_CYSCREEN)

/*
SM_CLEANBOOT : 시스템이 어떻게 부팅되었는지를 조사한다. 0이면 보통 모드로 부팅된 것이고 1이면 세이프 모드로 부팅한 것이고 2이면 네트웍 지원이 있는 세이프 모드

SM_CMONITORS : 데스크탑의 모니터 개수

SM_CMOUSEBUTTONS : 마우스의 버튼 개수. 0이면 마우스가 설치되어 있지 않다

SM_CXBORDER, SM_CYBORDER : 윈도우 경계선의 폭과 높이를 픽셀 단위로 조사한다. 3D 스타일의 SM_CXEDGE와 동일하다.

SM_CXDRAG, SM_CYDRAG : 최초 마우스 버튼을 누른 후 드래그를 시작하기 위해 움직여야 하는 거리를 조사한다.

SM_CXHSCROLL, SM_CYHSCROLL : 스크롤 바의 화살표 비트맵의 폭과 높이.

SM_CXICON, SM_CYICON : 아이콘의 폭과 높이. LoadIcon은 이 크기의 아이콘만 읽을 수 있다.
*/

//싱글톤 
#define SINGLETONE(class_name) private: class_name(void); ~class_name(void);\
public: static class_name* GetInstance() { static class_name instance; return &instance; }


#define SAFE_DELETE(p)			{ if(p) {delete (p);	p = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{ if(p) {delete [] p;	p = NULL;}}
#define SAFE_RELEASE(p)			{ if(p) {p->Release();	p = NULL;}}

#define SAFE_DESTROY_WINDOW(hWnd)	{ if(hWnd) { DestroyWindow(hWnd); hWnd = NULL; } }

#define SAFE_ADD_REF(p) {if(p){p->AddRef();}}
//get & set 받아주는 함수를 자동으로 제작
#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName; \
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

//아직 안써봐서 봉인. 
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
//마우스 좌표 
//extern POINT g_ptMouse;
//extern BOOL g_bUseMouseWheel;
extern short g_WheelDeltaWPARAM;

//핸들값 전체 
extern HWND	g_hWnd;
extern HWND	g_hwndGame;
extern HWND g_hwndHierarchy;
extern HWND g_hwndProject;
extern HWND g_hwndDebug;
extern HWND g_hwndInspector;

//모든 View들의 객체 포인터
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

//렌더링 파이프 라인
//3차원으로 정의된 기하구조 정보를
//2차원 화면 버퍼에 렌더링 하기 위해 필요한 모든 기계 / 소프트 웨어 적인 절차

//FVF = Flexible Vertex Format
//FVF는 D3D에서 정해져 있는 정점의 포맷 중에서 자신이 제작하는 프로그램에 알맞은 것들을 모아서 새로운 정점 포맷을 정하는 방법이다.
//
//D3DFVF_XYZ		-> 3차원 공간에서의 정점의 좌표
//D3DFVF_XYZRHW	-> 2차원 공간에서의 정점 좌표 D3D를 이용해 2D를 표현할 때 사용
//D3DFVF_DIFFUSE 정점의 색
//D3DFVF_SPECULAR 정점의 반사값
//
////정점의 속성
//const DWORD ST_VERTEX::FVF = ...; 스태틱을 제외하고 초기화해야한다.
//헤더파일에서 초기화하면 여러번 초기화 하기 때문에 컴파일 에러가 난다.
//따라서 여러번 호출되지 않는 곳(cpp)에서 초기화해야 한다.

inline LONG RectHeight(const RECT& r) { return r.bottom - r.top; }
inline LONG RectWidth(const RECT& r) { return r.right - r.left; }
inline POINT RectCenter(const RECT& r)
{
	POINT pt; pt.x = r.left + RectWidth(r) / 2; pt.y = r.top + RectHeight(r) / 2; return pt;
}

//마우스 좌표 
inline POINT GetCursorPos() { POINT pt; GetCursorPos(&pt); return pt; }




//===================================
// 싱글톤
//===================================
#include "cSceneManager.h"
#include <DeviceMgr.h>

//디버그용
#include "cDebugLog.h"
