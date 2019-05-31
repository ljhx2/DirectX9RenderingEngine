// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// KCrowtitTool.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.

const DWORD ST_VERTEX::FVF = (D3DFVF_XYZRHW | D3DFVF_TEX1);
const DWORD ST_PT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_TEX1);
const DWORD ST_PNT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
const DWORD ST_PC_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);