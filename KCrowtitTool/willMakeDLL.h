#pragma once

struct ST_VERTEX
{
	D3DXVECTOR4 p;
	D3DXVECTOR2	t;
	static const DWORD FVF;
};

//3D - point , texture 
struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2	t;
	static const DWORD FVF;
};

// point, nomalVector , texture
struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;
	static const DWORD FVF;
};

// point & color 
struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	static const DWORD FVF;
};

//구와 구 충돌 
inline BOOL ColBetweenSpheres(D3DXVECTOR3 s1, float s1Radius, D3DXVECTOR3 s2, float s2Radius)
{
	return sqrt(powf(s2.x - s1.x, 2.0f) + powf(s2.y - s1.y, 2.0f) + powf(s2.z - s1.z, 2.0f)) < (s1Radius + s2Radius);
	//return (powf(s2.x - s1.x, 2.0f) + powf(s2.y - s1.y, 2.0f) + powf(s2.z - s1.z, 2.0f)) < powf(s1Radius + s2Radius, 2.0f);
}

//선형 보간 
inline float Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

//두점 사이의 거리 vector 3 
inline BOOL GetDistacne(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
{
	return D3DXVec3Length(&(*pv2 - *pv1));
}



// Char → TCHAR
inline TCHAR* CharToTCHAR(const char* pstrSrc)
{
	size_t nLen = strlen(pstrSrc) + 1;
	TCHAR * pwstr = (LPWSTR)malloc(sizeof(TCHAR)*nLen);
	size_t con = 0;
	memset(pwstr, 0, sizeof(pwstr));

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pstrSrc, nLen, pwstr, nLen);
	return pwstr;
}

// TCHAR → Char
inline char * TCHARToChar(const TCHAR* pstrSrc)
{
	size_t nLen = lstrlen(pstrSrc) + 1;
	char * pstr = new char[sizeof(char)* nLen];
	WideCharToMultiByte(CP_ACP, 0, pstrSrc, nLen, pstr, nLen, NULL, NULL);

	return pstr;
}