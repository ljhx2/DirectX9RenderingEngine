#include "Common.h"
#include "cTextureManager.h"
#include "include\DeviceMgr.h"
#include <string>

cTextureManager::cTextureManager(void)
{
}


cTextureManager::~cTextureManager(void)
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( char* szFilename, D3DXIMAGE_INFO* pImageInfo /*= NULL*/ )
{
	return GetTexture(std::string(szFilename), pImageInfo);
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( std::string& sFilename, D3DXIMAGE_INFO* pImageInfo /*= NULL*/ )
{
	
	std::wstring wFileName = L"";
	wFileName.assign(sFilename.begin(), sFilename.end());

	if(pImageInfo)
	{
		

		if( m_mapImageInfo.find(sFilename) == m_mapImageInfo.end() ||
			m_mapTexture.find(sFilename) == m_mapTexture.end())
		{
			SAFE_RELEASE(m_mapTexture[sFilename]);

			

			D3DXCreateTextureFromFileEx(
				g_pD3DDevice,
				wFileName.c_str(),
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT,
				0,
				D3DFMT_UNKNOWN, 
				D3DPOOL_MANAGED, 
				D3DX_FILTER_NONE, 
				D3DX_DEFAULT, 
				0,
				&m_mapImageInfo[sFilename],
				NULL,
				&m_mapTexture[sFilename] );
		}
	}
	else if(m_mapTexture.find(sFilename) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFile(g_pD3DDevice, wFileName.c_str(), &m_mapTexture[sFilename]);
	}

	if(pImageInfo)
		*pImageInfo = m_mapImageInfo[sFilename];
	
	return m_mapTexture[sFilename];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTextureTest(char* szFilename, D3DXIMAGE_INFO* pImageInfo /*= NULL*/)
{
	return GetTextureTest(std::string(szFilename), pImageInfo);
}
LPDIRECT3DTEXTURE9 cTextureManager::GetTextureTest(std::string& sFilename, D3DXIMAGE_INFO* pImageInfo /*= NULL*/)
{

	std::wstring wFileName = L"";
	wFileName.assign(sFilename.begin(), sFilename.end());

	if (D3DXCreateTextureFromFileEx(
		g_pD3DDevice,
		wFileName.c_str(),					//파일명 
		D3DX_DEFAULT,		//width  -D3DX_DEFAULT 기본 그림 값
		D3DX_DEFAULT,		//height
		D3DX_DEFAULT,						//mipLevels
		NULL,						//
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		(D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR),				//Filter
		(D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR),				//MipFilter
		D3DCOLOR_XRGB(0, 0, 0),		//투명이 되는 D3DCOLOR - 이걸 해주면 etRenderState(D3DRS_ALPHAREF, D3DCOLOR_XRGB(0, 0, 0)) 안해도 됨
		NULL,						//D3DXIMAGE_INFO의 구조체 포인트
		NULL,						//256색 팔레트를 나타내는 구조체 포인터 or NULL
		&m_mapTexture[sFilename]))						//텍스쳐 포인터 주소 
	{
	}

	return m_mapTexture[sFilename];
}
void cTextureManager::SaveTexture(const TCHAR* fileName, LPDIRECT3DTEXTURE9 pTex, _D3DXIMAGE_FILEFORMAT format)
{
	D3DXSaveTextureToFile(fileName, D3DXIFF_PNG, pTex, NULL);
}
void cTextureManager::Destroy()
{
	for each(auto p in m_mapTexture)
	{
		SAFE_RELEASE(p.second);
	}
}
