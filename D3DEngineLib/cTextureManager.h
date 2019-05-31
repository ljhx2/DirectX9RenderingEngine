#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	cTextureManager();
	~cTextureManager();
public:
	static cTextureManager* GetInstance() { static cTextureManager instance; return &instance; }

private:
	std::map<std::string, LPDIRECT3DTEXTURE9>	m_mapTexture;
	std::map<std::string, D3DXIMAGE_INFO>		m_mapImageInfo;

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFilename, D3DXIMAGE_INFO* pImageInfo = NULL);
	LPDIRECT3DTEXTURE9 GetTexture(std::string& sFilename, D3DXIMAGE_INFO* pImageInfo = NULL);



	LPDIRECT3DTEXTURE9 GetTextureTest(char* szFilename, D3DXIMAGE_INFO* pImageInfo = NULL);
	LPDIRECT3DTEXTURE9 GetTextureTest(std::string& sFilename, D3DXIMAGE_INFO* pImageInfo = NULL);

	void SaveTexture(const TCHAR* fileName, LPDIRECT3DTEXTURE9 pTex, _D3DXIMAGE_FILEFORMAT format = D3DXIFF_PNG);
	void Destroy();
};

