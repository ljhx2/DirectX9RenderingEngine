#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#include <NiBuildConfiguration.h>
//#include <windows.h>
//#include <NiMain.h>
//#include <NIAnimation.h>
//#include <NiInputSystem.h>
//#include <NiDI8InputSystem.h>
//#include <NiParticleSystem.h>
//#include <NiMeshParticleSystem.h>
//#include <NiInputGamePad.h>
//
//#include <NiDirectXVersion.h>
//#include <NiDX9Renderer.h>
#include <assert.h>
//#include "../../SharedSource/StringUtil.h"
//#include "../../SharedSource/ErrorCode.h"
//#include "../../SharedSource/Exception.h"
//#include "../../SharedSource/LogCatalogList.h"
//#include "../../SharedSource/LogManager.h"


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here


#ifndef _INC_MMSYSTEM
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma message("Automatically linking with winmm.lib")    
#endif

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3dx9.h>
#include <assert.h>
#include <vector>
#include <map>
#include <hash_map>
#include <math.h>
#include <set>
#include <list>
#include <string>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define WINSIZE_X 1024
#define WINSIZE_Y 768

#define SAFE_RELEASE(p) {if(p)p->Release(); p = NULL;}
#define SAFE_DELETE(p) {if(p) delete p; p = NULL;}


#define SAFE_DELETE_ARRAY(p) {if(p){delete [] p; p = NULL;}}

#define SAFE_ADD_REF(p) {if(p){p->AddRef();}}


/** @def    _SAFE_DELETE(p)
*  @brief  ��ü�� �����, �� ���� NULL�� �Ҵ��Ͽ�, �������� �ο��Ѵ�.
*/
#define _SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

/** @def   _SAFE_DELETE_ARRAY(p)
*  @brief �迭�� �޸� �Ҵ��� ��ü�� �����, �� ���� NULL�� �Ҵ��Ͽ�, ������
*         �� �ο��Ѵ�.
*/
#define _SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

/** @def    _SAFE_RELEASE(p)
*  @brief  Release()�� ���� ���� �Ǵ� �������̽���(�ַ� COM��ü)
*          �����ϰ� ������Ų��.
*/
#define _SAFE_RELEASE(p)      { if(p) { p->Release();   (p)=NULL; } }


#define SINGLETONE(class_name) private: class_name(void); ~class_name(void);\
public:	static class_name* GetInstance() {	static class_name instance;	return &instance; } 

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
		}\
}

//�Ŵ���(�̱���)
#include "include/DeviceMgr.h"
#include "cTextureManager.h"
#include "cSkinnedMeshManager.h"
#include "cStaticMeshManager.h"
#include "cTimeManager.h"
#include "cFbxManager.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "cCameraManager.h"
#include "cCollisionManager.h"
#include "cGameObjectManager.h"
#include "UniqueIdManager.h"
#include "cShadowManager.h"
#include "cWorldMapManager.h"
#include "cDamageNumberManager.h"
//#include "cDebugLog.h"

//��ŷ�� ���̴� ������
struct ST_PickingRay
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};
