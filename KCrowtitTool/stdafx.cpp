// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// KCrowtitTool.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

// TODO: �ʿ��� �߰� �����
// �� ������ �ƴ� STDAFX.H���� �����մϴ�.

const DWORD ST_VERTEX::FVF = (D3DFVF_XYZRHW | D3DFVF_TEX1);
const DWORD ST_PT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_TEX1);
const DWORD ST_PNT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
const DWORD ST_PC_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);