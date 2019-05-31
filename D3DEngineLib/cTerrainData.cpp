#include "Common.h"
#include "include\D3DRenderer.h"
#include "cTerrainData.h"

const DWORD ST_PNTT_VERTEX::FVF = { D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 };
const DWORD ST_BURSH_PC_VERTEX::FVF = { D3DFVF_XYZ | D3DFVF_DIFFUSE };
namespace ENGINE
{
	cTerrainData::cTerrainData()
	{
		testSize = 8;
		m_nBrushType = BRUSH_STATE_TYPE::STATE_NONE;
		m_eMouseState = ST_MOUSE_STATE::MOUSE_MOVE;
		m_pVB = NULL;
		m_pIB = NULL;
		m_pIndices = NULL;
		m_pTexHeight = NULL;
		m_pVertices = NULL;
		m_pFogEffect = NULL;

		m_nRowMax = 0;
		m_nColMax = 0;

		m_nVtRowMax = 0;
		m_nVtColMax = 0;

		m_nAllVt = 0;
		m_nAllIndex = 0;

		m_fWidth = 0.0f;
		m_fDepth = 0.0f;
		m_fCellSpacing = 0.0f;

		m_nStartRow = 0;
		m_nStartCol = 0;
		bIsAlphaTex = true;

		m_fRadius = 1.0f;
		m_fPaintRadius = 0.5f;
		m_fRowFileHeight = 5.0f;
		bIsWireFrame = false;
		m_dwAlphaColor = 0x00;

		m_bIsPressing = false;
		m_bIsWireFrameMode = false;

		m_bIsFog = false;
		//브러쉬 관련 어쩔까 
		ZeroMemory(&m_HeightBrushMtl, sizeof(m_HeightBrushMtl));
		m_HeightBrushMtl.Ambient.r = 0.0;
		m_HeightBrushMtl.Ambient.g = 0.0;
		m_HeightBrushMtl.Ambient.b = 1.0f;
		m_HeightBrushMtl.Ambient.a = 1.0;

		m_HeightBrushMtl.Diffuse.r = 0.0;
		m_HeightBrushMtl.Diffuse.g = 0.0;
		m_HeightBrushMtl.Diffuse.b = 1.0f;
		m_HeightBrushMtl.Diffuse.a = 1.0f;

		ZeroMemory(&m_PaintBrushMtl, sizeof(m_PaintBrushMtl));
		m_PaintBrushMtl.Ambient.r = 1.0f;
		m_PaintBrushMtl.Ambient.g = 0.0;
		m_PaintBrushMtl.Ambient.b = 0.0f;
		m_PaintBrushMtl.Ambient.a = 1.0;

		m_PaintBrushMtl.Diffuse.r = 1.0f;
		m_PaintBrushMtl.Diffuse.g = 0.0;
		m_PaintBrushMtl.Diffuse.b = 0.0f;
		m_PaintBrushMtl.Diffuse.a = 1.0f;



	}

	cTerrainData::~cTerrainData()
	{
		SAFE_RELEASE(m_pVB);
		SAFE_RELEASE(m_pIB);
		SAFE_RELEASE(m_pTexHeight);
		SAFE_DELETE(m_pIndices);
		SAFE_DELETE(m_pVertices);
		//SAFE_RELEASE(m_pFVF);
		//SAFE_RELEASE(m_pFogFVF);
	}
	/*변경 되는 정보들*/
	void cTerrainData::ChangePriority(int nFirst, int nSecond)
	{
		ST_TEX_ALPHA temp;

		temp = m_SplattingTex[nFirst];
		m_SplattingTex[nFirst] = m_SplattingTex[nSecond];
		m_SplattingTex[nSecond] = temp;
	}
	void cTerrainData::SetGroundTexture(const char * szKeyName)
	{
		std::string strKey((szKeyName));
		if (m_nCurrentIndex == -1)
			m_pTex = g_pTextureManager->GetTextureTest(strKey);
		else
			m_SplattingTex[m_nCurrentIndex].tex = g_pTextureManager->GetTextureTest(strKey);
	}

	/*
	정점 정보들
	*/
	void cTerrainData::SettingInfo(int row, int col, float * pH)
	{
		m_nRowMax = row;				// 셀의 칸 
		m_nColMax = col;				// 셀의 칸 

		m_nVtRowMax = m_nRowMax + 1;	//버텍스 수 
		m_nVtColMax = m_nColMax + 1;	//버테스 수 

		m_nAllVt = (m_nVtRowMax  * m_nVtColMax);	//전제 버텍스 숫자 

		m_nAllIndex = (m_nRowMax * m_nColMax) * 6; // * 2 * 3 삼각형* 점3 

		m_fWidth = m_nRowMax * m_fCellSpacing;
		m_fDepth = m_nColMax * m_fCellSpacing;

		m_nStartRow = (int)(-m_nRowMax / 2 * (m_fCellSpacing));
		m_nStartCol = (int)(m_nColMax / 2 * (m_fCellSpacing));

		m_pVertices = new ST_PNTT_VERTEX[m_nAllVt];
		m_pIndices = new UINT[m_nAllIndex];

		int index = 0;
		//버텍스 
		for (int col = 0; col < m_nVtColMax; ++col)
		{
			for (int row = 0; row < m_nVtRowMax; ++row)
			{
				float _y = pH[row * (m_nVtColMax)+col];
				m_pVertices[index].p = D3DXVECTOR3(m_nStartRow + row * m_fCellSpacing, _y, m_nStartCol + col * -m_fCellSpacing);
				m_pVertices[index].n = D3DXVECTOR3(0, 1, 0);
				m_pVertices[index].t = D3DXVECTOR2((float)(row * m_fCellSpacing) / m_fWidth, 1.0f - (float)(col*m_fCellSpacing) / m_fDepth);
				m_pVertices[index].t2 = D3DXVECTOR2((float)(row * m_fCellSpacing / testSize) / m_fWidth, 1.0f - (float)(col*m_fCellSpacing / testSize) / m_fDepth);
				index++;
			}
		}
	}

	BOOL cTerrainData::SettingSize(int row, int col, float cellSpacing)
	{
		int maxSize = (row + 1) * (col + 1);
		float *pHeight = new float[maxSize];
		ZeroMemory(pHeight, row*col);
		for (int index = 0; index <maxSize; ++index)
		{
			pHeight[index] = 0.0f;
		}
		m_fCellSpacing = cellSpacing;
		SettingInfo(row, col, pHeight);
		SAFE_DELETE(pHeight);
		////노멀값 셋팅 
		SettingNormal();

		//버텍스 버퍼 
		g_pD3DDevice->CreateVertexBuffer(sizeof(ST_PNTT_VERTEX)* m_nAllVt, D3DUSAGE_DYNAMIC,
			ST_PNTT_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL);

		SettingVertex();

		if (!SettingIndex())
			return FALSE;

		if (!SettingSplatting())
		{
			TCHAR strMessage[255] = { 0 };
			wsprintf(strMessage, L"Terrain _스플레팅 세팅 오류");
			::MessageBox(NULL, strMessage, L"Error", MB_OK);
			return FALSE;
		}

		//////fog
		//m_pFogEffect = g_pShaderManager->GetShader("Terrain/Shader/", "Fog.fx");
		//D3DVERTEXELEMENT9 decl[] =
		//{
		//	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		//	{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		//	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		//	D3DDECL_END()
		//};


		//if (FAILED(g_pD3DDevice->CreateVertexDeclaration(decl, &m_pFogFVF)))
		//	return FALSE;

		////g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);

		//return TRUE;

	}
	bool cTerrainData::LoadHeightMap(const char* szHeightFileName)
	{
		FILE * pfile;

		fopen_s(&pfile, szHeightFileName, "rb");
		//파일 없으면 에러 
		if (!pfile) {
			char strMessage[255] = { 0 };
			strcpy_s(strMessage, szHeightFileName);
			strcat_s(strMessage, "__Terrain Tex File Can't Not Found");
			::MessageBox(NULL, UTIL::CharToTCHAR(strMessage), L"Error", MB_OK);
			return false;
		}

		fseek(pfile, 0, SEEK_END);

		int lSize = ftell(pfile);

		fseek(pfile, 0, SEEK_SET);
		float * pH = new float[lSize];
		fread(pH, sizeof(float) * lSize, 1, pfile);
		fclose(pfile);

		/*
		std::string cell(szHeightFileName);
		cell.replace(cell.size() - 3, 3, "txt");
		pfile = NULL;
		fopen_s(&pfile, cell.c_str(), "rb");

		if (!pfile)
			return FALSE;
		fread(&m_fCellSpacing, sizeof(float), 1, pfile);

		fclose(pfile);
		*/

		///m_fCellSpacing = 1.0f;//셀 세팅은 꼭 미리 해줘야함
		SettingInfo((int)((sqrt(lSize) / 2) - 1), (int)((sqrt(lSize) / 2) - 1), pH);	//기본 정보 셋팅 

		SAFE_DELETE(pH);

		////노멀값 셋팅 
		SettingNormal();

		//버텍스 버퍼 
		g_pD3DDevice->CreateVertexBuffer(sizeof(ST_PNTT_VERTEX)* m_nAllVt, D3DUSAGE_DYNAMIC,
			ST_PNTT_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL);

		SettingVertex();

		if (!SettingIndex())
			return FALSE;

		return TRUE;
	}

	//textrue 또는 Alphamap
	bool cTerrainData::LoadTexture(const char* szTextureFileName)
	{
		std::string fullPath("Terrain/Texture/");
		std::string fileName(szTextureFileName);
		fileName.replace(fileName.size() - 3, 3, "png");
		std::string LoadFile = fullPath + fileName;

		LPDIRECT3DTEXTURE9  pTempTexture = NULL;
		if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, UTIL::CharToTCHAR(LoadFile.c_str()), &pTempTexture)))
		{
			bIsAlphaTex = false;
			SAFE_DELETE(pTempTexture);
			TCHAR strMessage[255] = { 0 };
			wsprintf(strMessage, L"해당 Terrain은 텍스쳐 없음");
			::MessageBox(NULL, strMessage, L"No Textrue", MB_OK);
			return false;
		}
		if (!SettingSplatting())
		{
			TCHAR strMessage[255] = { 0 };
			wsprintf(strMessage, L"Terrain _스플레팅 세팅 오류");
			::MessageBox(NULL, strMessage, L"Error", MB_OK);
			return false;
		}
		if (!LoadAlphaMap(LoadFile.c_str()))
		{
			TCHAR strMessage[255] = { 0 };
			wsprintf(strMessage, L"Terrain _알파맵 세팅 오류");
			::MessageBox(NULL, strMessage, L"Error", MB_OK);
			return false;
		}
		return true;
	}
	BOOL cTerrainData::SaveHeightMap(const char * strPath)
	{
		std::string str(strPath);
		FILE *pfile;
		fopen_s(&pfile, str.c_str(), "wb");

		if (!pfile)
			return FALSE;

		//세이브 문제 
		int index = 0;
		for (int col = 0; col < m_nVtColMax; ++col)
		{
			for (int row = 0; row < m_nVtRowMax; ++row)
			{
				float height = m_pVertices[row * (m_nVtColMax)+col].p.y;
				fwrite(&height, sizeof(float), 1, pfile);
				index++;
			}
		}

		fclose(pfile);

		/* XML 로 변환할것
		std::string cell(strPath);
		cell.replace(str.size() - 3, 3, "txt");
		pfile = NULL;
		fopen_s(&pfile, cell.c_str(), "wb");

		if (!pfile)
			return FALSE;
		fwrite(&m_fCellSpacing, sizeof(float), 1, pfile);

		fclose(pfile);
		*/
		return TRUE;
	}
	BOOL cTerrainData::SettingVertex()
	{
		VOID * pVertices = NULL;
		if (m_pVB->Lock(0, sizeof(ST_PNTT_VERTEX)* m_nAllVt, (void**)&pVertices, D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE))
		{
			return FALSE;
		}
		memcpy(pVertices, m_pVertices, sizeof(ST_PNTT_VERTEX)* m_nAllVt);
		if (m_pVB->Unlock())
		{
			return FALSE;
		}
		return TRUE;
	}
	BOOL cTerrainData::SettingIndex()
	{
		int index = 0;
		for (int col = 0; col < m_nColMax; ++col)
		{
			for (int row = 0; row < m_nRowMax; ++row)
			{
				//사각형 1개  ex) 5
				m_pIndices[index++] = col * m_nVtRowMax + row;//0
				m_pIndices[index++] = col * m_nVtRowMax + (row + 1);//1
				m_pIndices[index++] = (col + 1) * m_nVtRowMax + row;//5 
				m_pIndices[index++] = (col + 1) * m_nVtRowMax + row;//5 
				m_pIndices[index++] = col * m_nVtRowMax + (row + 1);//1
				m_pIndices[index++] = (col + 1) * m_nVtRowMax + (row + 1);//6
			}
		}
		//풀스크린 모드 할땐 인덱스 & 버텍스 버퍼를 D3DPOOL_MANAGED 로 바꿔야 한다. 
		//D3DPOOL_DEFAULT로 하면 안됨
		g_pD3DDevice->CreateIndexBuffer(sizeof(UINT)* m_nAllIndex, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL);

		LPVOID pIndices = NULL;
		if (m_pIB->Lock(0, 0, &pIndices, 0))
			return FALSE;
	/*	if (m_pIB->Lock(0, sizeof(UINT)* m_nAllIndex, &pIndices, D3DLOCK_DISCARD))
			return FALSE;*/
		memcpy(pIndices, m_pIndices, sizeof(UINT)* m_nAllIndex);
		if (m_pIB->Unlock())
			return FALSE;

		return TRUE;
	}
	//void cTerrainData::IndexBufferSetting(cFrustum *  frustum)
	//{
	//	LPVOID pIndices = NULL;
	//	m_pIB->Lock(0, sizeof(UINT)* m_nAllIndex, &pIndices, D3DLOCK_DISCARD);
	//	memcpy(pIndices, m_pIndices, sizeof(UINT)* m_nAllIndex);
	//	m_nTriangles = m_pQuadTree->GenerateIndex(pIndices, m_pVertices, frustum, m_fLODRatio);
	//	m_pIB->Unlock();
	//}
	BOOL cTerrainData::SettingSplatting()
	{
		//텍스쳐 로딩 --- 으아아아아아 ;
		m_pTex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");
		m_SplattingTex[0].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;
		m_SplattingTex[1].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;
		m_SplattingTex[2].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;
		m_SplattingTex[3].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;
		m_SplattingTex[4].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;
		m_SplattingTex[5].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;
		m_SplattingTex[6].tex = g_pTextureManager->GetTextureTest("Terrain/TexBrush/Grass_001.jpg");;

		//알파맵 만들기
		for (int i = 0; i<TEX_MAX; ++i)
		{
			if (FAILED(g_pD3DDevice->CreateTexture(TEX_ALPHA_SIZE, TEX_ALPHA_SIZE, 1, 0, D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED, &m_SplattingTex[i].amap, NULL)))
			{
				return FALSE;
			}

			D3DLOCKED_RECT alphaLock;
			memset(&alphaLock, 0, sizeof(D3DLOCKED_RECT));

			if (FAILED(m_SplattingTex[i].amap->LockRect(0, &alphaLock, NULL, 0))) return FALSE;

			LPBYTE pDataDST = (LPBYTE)alphaLock.pBits;

			//알파맵 사이즈 셋팅 
			for (int j = 0; j<TEX_ALPHA_SIZE; ++j)
			{
				LPDWORD pwDST = (LPDWORD)(pDataDST + j * alphaLock.Pitch);
				for (int i = 0; i<TEX_ALPHA_SIZE; ++i)	*(pwDST + i) = 0x00000000;
			}
			if (FAILED(m_SplattingTex[i].amap->UnlockRect(0))) return FALSE;
		}

		//------------------------------
		//이펙트 생성 나중에 함수로 뺄것 
		//------------------------------
		//LPD3DXBUFFER buff = NULL;
		//if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice, L"Terrain/Shader/Splatting.fx", NULL, NULL, 0, NULL, &m_pTexEffect, &buff)))
		//{
		//	LPVOID comErr = buff->GetBufferPointer();
		//	MessageBox(NULL, L"Splatting Shader 에러", NULL, MB_OK);
		//	return FALSE;
		//}

		m_pTexEffect = g_pShaderManager->GetShader("Terrain/Shader/", "Splatting.fx");

		//셰이더 FVF -----아래 참조  
		D3DVERTEXELEMENT9 decl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
			D3DDECL_END()
		};

		if (FAILED(g_pD3DDevice->CreateVertexDeclaration(decl, &m_pFVF))) return FALSE;
		 
		return TRUE;
	}


	void cTerrainData::DrawAlphaMap()
	{
		//알파맵 그리기 draw 값 
		float pixsize = (float)(m_fWidth) / (float)TEX_ALPHA_SIZE;

		float editsize = m_fRadius / pixsize;

		//가운데 정렬을 시켜서 Texture 좌표에 맞추려면 이동시켜줘야함. --이제 여기만 고치면됨..
		//float PosU = ((m_vMousePos.x / m_fWidth) + ((m_fWidth*0.5f) / m_fWidth));
		//float PosV = ((m_vMousePos.z / m_fDepth) + ((m_fDepth*0.5f) / m_fDepth));
		float PosU = ((m_vMousePos.x / m_fWidth) + ((m_fWidth* 0.5f) / m_fWidth));
		float PosV = ((m_vMousePos.z / m_fDepth) + ((m_fDepth* 0.5f) / m_fDepth));

		int m_nTexPosX = int(PosU * TEX_ALPHA_SIZE);
		int m_nTexPosY = int(PosV * TEX_ALPHA_SIZE);

		int startx = int(((m_nTexPosX - editsize) < 0) ? 0 : m_nTexPosX - editsize);
		int starty = int(((m_nTexPosY - editsize) < 0) ? 0 : m_nTexPosY - editsize);
		int endx = int(((m_nTexPosX + editsize) >= TEX_ALPHA_SIZE) ? TEX_ALPHA_SIZE - 1 : m_nTexPosX + editsize);
		int endy = int(((m_nTexPosY + editsize) >= TEX_ALPHA_SIZE) ? TEX_ALPHA_SIZE - 1 : m_nTexPosY + editsize);

		DWORD changeColor = 0x00;
		float smooth = 0.0f;
		DWORD dwA = 0x00;

		D3DLOCKED_RECT alphamap;
		memset(&alphamap, 0, sizeof(D3DLOCKED_RECT));


		if (m_nCurrentIndex < 0) return;
		if (FAILED(m_SplattingTex[m_nCurrentIndex].amap->LockRect(0, &alphamap, NULL, 0)))	return;

		LPBYTE dataDST = (LPBYTE)alphamap.pBits;
 
		for (int j = starty; j <= endy; ++j)
		{
			LPDWORD wordDST = (LPDWORD)(dataDST + j * alphamap.Pitch);
			for (int i = startx; i <= endx; ++i)
			{
				D3DXVECTOR3 pix = D3DXVECTOR3(i * pixsize, 0.0f, j * pixsize)
					- D3DXVECTOR3(m_nTexPosX * pixsize, 0.0f, m_nTexPosY * pixsize);
				//픽셀이 현위치에서 떨어져 있는 거리
				float len = D3DXVec3Length(&pix);
				//0x3f3f3f3f
				if (len <= m_fPaintRadius)
				{
					changeColor = (m_dwAlphaColor << 24) | (m_dwAlphaColor << 16) | (m_dwAlphaColor << 8) | m_dwAlphaColor;
				}
				else if (len <= m_fRadius)
				{
					// 최대사이즈에 포함될 우 최대사이즈까지의
					// 선영보간한 알파값을 구한다.
					len -= m_fPaintRadius;
					smooth = m_fRadius - m_fPaintRadius;
					changeColor = DWORD((smooth - len) / smooth * (m_dwAlphaColor << 24));
					dwA = (changeColor & 0xff000000) >> 24;
					changeColor = (dwA << 24) | (dwA << 16) | (dwA << 8) | dwA;
				}
				else
				{
					continue;
				}
				*(wordDST + i) = (*(wordDST + i) < changeColor) ? changeColor : *(wordDST + i);
			}
		}
		if (FAILED(m_SplattingTex[m_nCurrentIndex].amap->UnlockRect(0)))	return;
	}
	BOOL cTerrainData::LoadAlphaMap(const char * strPath)
	{
		std::string pullPath("Terrain/Texture/");
		std::string fileName(strPath);
		fileName.replace(fileName.size() - 3, 3, "png");

		LPDIRECT3DTEXTURE9 temp;
		D3DXCreateTexture(g_pD3DDevice, TEX_ALPHA_SIZE, TEX_ALPHA_SIZE, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &temp);

		LPDIRECT3DTEXTURE9  pTempTexture = NULL;
		if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, UTIL::CharToTCHAR(fileName.c_str()), &pTempTexture)))
			return false;

		LPDIRECT3DSURFACE9  pTargetSurf = NULL;
		pTempTexture->GetSurfaceLevel(0, &pTargetSurf);

		RECT rc;
		char buf[4];
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < TEX_MAX; ++j)
			{
				buf[0] = 0;
				LPDIRECT3DSURFACE9  pSrcSurf = NULL;
				temp->GetSurfaceLevel(0, &pSrcSurf);

				rc.left = TEX_ALPHA_SIZE * j;
				rc.right = TEX_ALPHA_SIZE * (j + 1);
				rc.top = TEX_ALPHA_SIZE * i;
				rc.bottom = TEX_ALPHA_SIZE * (i + 1);

				D3DXLoadSurfaceFromSurface(pSrcSurf, NULL, NULL, pTargetSurf, NULL, &rc, D3DX_FILTER_NONE, 0);
				std::string num = _itoa(j, buf, 10);
				num += (".png");
				std::string strTemp = pullPath + num;
				g_pTextureManager->SaveTexture(UTIL::CharToTCHAR(strTemp.c_str()), temp);

				if (i == 0)
					m_SplattingTex[j].tex = g_pTextureManager->GetTextureTest(strTemp);
				else if (i == 1)
					m_SplattingTex[j].amap = g_pTextureManager->GetTextureTest(strTemp);
				pSrcSurf->Release();
				::DeleteFile(UTIL::CharToTCHAR(strTemp.c_str()));
			}
		}

		LPDIRECT3DSURFACE9  pSrcSurf = NULL;
		temp->GetSurfaceLevel(0, &pSrcSurf);
		rc.left = TEX_ALPHA_SIZE * TEX_MAX;
		rc.right = TEX_ALPHA_SIZE * (TEX_MAX + 1);
		rc.top = 0;
		rc.bottom = TEX_ALPHA_SIZE;
		D3DXLoadSurfaceFromSurface(pSrcSurf, NULL, NULL, pTargetSurf, NULL, &rc, D3DX_FILTER_NONE, 0);
		g_pTextureManager->SaveTexture(L"Terrain/Texture/temp.png", temp);
		m_pTex = g_pTextureManager->GetTextureTest("Terrain/Texture/temp.png");
		pSrcSurf->Release();
		::DeleteFile(L"Terrain/Texture/temp.png");
		pTargetSurf->Release();

		return TRUE;


	}

	BOOL cTerrainData::SaveAlphaMap(const char * strPath)
	{
		std::string Path(strPath);

		LPDIRECT3DTEXTURE9  pTempTexture = NULL;

		D3DXCreateTexture(g_pD3DDevice, TEX_ALPHA_SIZE* (TEX_MAX + 1), TEX_ALPHA_SIZE * 2, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &pTempTexture);

		LPDIRECT3DSURFACE9  pTargetSurf = NULL;
		pTempTexture->GetSurfaceLevel(0, &pTargetSurf);

		RECT rc;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < TEX_MAX; ++j)
			{
				LPDIRECT3DSURFACE9  pSrcSurf = NULL;

				if (i == 0)
					m_SplattingTex[j].tex->GetSurfaceLevel(0, &pSrcSurf);
				else if (i == 1)
					m_SplattingTex[j].amap->GetSurfaceLevel(0, &pSrcSurf);

				rc.left = TEX_ALPHA_SIZE * j;
				rc.right = TEX_ALPHA_SIZE * (j + 1);
				rc.top = TEX_ALPHA_SIZE * i;
				rc.bottom = TEX_ALPHA_SIZE * (i + 1);

				D3DXLoadSurfaceFromSurface(pTargetSurf, NULL, &rc, pSrcSurf, NULL, NULL, D3DX_FILTER_NONE, 0);
				pSrcSurf->Release();
			}
		}

		LPDIRECT3DSURFACE9  pSrcSurf = NULL;
		m_pTex->GetSurfaceLevel(0, &pSrcSurf);

		rc.left = TEX_ALPHA_SIZE * TEX_MAX;
		rc.right = TEX_ALPHA_SIZE * (TEX_MAX + 1);
		rc.top = 0;
		rc.bottom = TEX_ALPHA_SIZE;

		D3DXLoadSurfaceFromSurface(pTargetSurf, NULL, &rc, pSrcSurf, NULL, NULL, D3DX_FILTER_NONE, 0);
		pSrcSurf->Release();

		D3DXSaveTextureToFile(UTIL::CharToTCHAR(strPath), D3DXIFF_PNG, pTempTexture, NULL);

		pTargetSurf->Release();

		return TRUE;
	}

	void cTerrainData::SettingNormal()
	{
		int index = 0;

		D3DXVECTOR3	Normal(0, 0, 0);
		D3DXVECTOR3	v0, v1, v2;
		D3DXVECTOR3	Nor;

		for (int col = 0; col < m_nColMax; ++col)
		{
			for (int row = 0; row < m_nRowMax; ++row)
			{
				int index = (row * m_nVtColMax) + col;

				float heightA = GetHeightmapCell(row, col);
				float heightB = GetHeightmapCell(row, col + 1);
				float heightC = GetHeightmapCell(row + 1, col);

				//UV 으음.. 
				D3DXVECTOR3 u(m_fCellSpacing, heightB - heightA, 0.0f);
				D3DXVECTOR3 v(0.0f, heightC - heightA, -m_fCellSpacing);

				D3DXVECTOR3 Nor;
				D3DXVec3Cross(&Nor, &u, &v);
				D3DXVec3Normalize(&Nor, &Nor);
				m_pVertices[index].n = Nor;

			}
		}
	}
	/*
	브러쉬 관련 
	*/

	//높이맵 
	void cTerrainData::UpdateBrush()
	{
		if (m_eMouseState == ST_MOUSE_STATE::MOUSE_LCLICK)
		{
			switch (m_nBrushType)
			{
			case BRUSH_STATE_TYPE::STATE_NONE:
				break;
			case BRUSH_STATE_TYPE::STATE_PAINT:
				DrawAlphaMap();
				break;
			case BRUSH_STATE_TYPE::STATE_CURVED:
				Curved();
				break;
			case BRUSH_STATE_TYPE::STATE_FLATTER:
				Flatter();
				break;
			case BRUSH_STATE_TYPE::STATE_HILL:
				Hill();
				break;
				//case BRUSH_STATE_TYPE::STATE_SMOOTH:
				//	Smooth();
				//	break;
				//case BRUSH_STATE_TYPE::STATE_SLOPE:
				//	Slope();
				//	break;
				//case BRUSH_STATE_TYPE::STATE_POLYGON:
				//	Polygon();
				break;
			}
		}
	}
	void cTerrainData::Hill()
	{
		//원
		RECT rt = getRange(m_nSelectedRow, m_nSelectedCol, m_nRowMax, m_nColMax, m_fRadius);

		for (int col = rt.top; col <= rt.bottom; ++col)
		{
			for (int row = rt.left; row <= rt.right; ++row)
			{
				float len = GetDistacne(m_nSelectedRow, m_nSelectedCol, row, col);
				if (m_fRadius> len)
				{
					int A = (row * m_nVtColMax) + col;
					m_pVertices[A].p.y = m_fRowFileHeight;
					if (col + 1 < m_nColMax)
					{
						int B = (row * m_nVtColMax) + (col + 1);
						m_pVertices[B].p.y = m_fRowFileHeight;
					}
					if (row + 1 < m_nRowMax)
					{
						int C = ((row + 1) * m_nVtColMax) + col;
						m_pVertices[C].p.y = m_fRowFileHeight;
					}
					if ((col + 1) < m_nColMax && (row + 1) < m_nRowMax)
					{
						int D = ((row + 1) * m_nVtColMax) + (col + 1);
						m_pVertices[D].p.y = m_fRowFileHeight;
					}
				}
			}
		}
		SettingNormal();
		SettingVertex();

	}
	void cTerrainData::Flatter()
	{
		//원
		RECT rt = getRange(m_nSelectedRow, m_nSelectedCol, m_nRowMax, m_nColMax, m_fRadius);

		float height = m_pVertices[(m_nSelectedRow * m_nVtColMax) + m_nSelectedCol].p.y;
		for (int col = rt.top; col <= rt.bottom; ++col)
		{
			for (int row = rt.left; row <= rt.right; ++row)
			{
				float len = GetDistacne(m_nSelectedRow, m_nSelectedCol, row, col);
				if (m_fRadius> len)
				{
					int A = (row * m_nVtColMax) + col;
					if (height < m_pVertices[A].p.y)
						m_pVertices[A].p.y = height;
					if (col + 1 < m_nColMax)
					{
						int B = (row * m_nVtColMax) + (col + 1);
						if (height < m_pVertices[B].p.y)
							m_pVertices[B].p.y = height;
					}
					if (row + 1 < m_nRowMax)
					{
						int C = ((row + 1) * m_nVtColMax) + col;
						if (height < m_pVertices[C].p.y)
							m_pVertices[C].p.y = height;
					}
					if ((col + 1) < m_nColMax && (row + 1) < m_nRowMax)
					{
						int D = ((row + 1) * m_nVtColMax) + (col + 1);
						if (height < m_pVertices[D].p.y)
							m_pVertices[D].p.y = height;
					}
				}
			}
		}
		SettingNormal();
		SettingVertex();
	}
	void cTerrainData::Curved()
	{
		float fRho = m_fRadius;

		int Colrange = m_fRadius * 4;
		int Rowrange = m_fRadius * 4;
		int countCol = Colrange * (-1);
		int countRow = Rowrange * (-1);

		for (int col = m_nSelectedCol - Colrange; col <= m_nSelectedCol + Colrange; ++col)
		{
			for (int row = m_nSelectedRow - Rowrange; row <= m_nSelectedRow + Rowrange; ++row)
			{
				if (col < 0) { countRow++; continue; }
				if (row < 0) { countRow++; continue; }
				if (row > m_nRowMax) continue;
				if (col > m_nColMax) continue;
				int A = (row * m_nVtColMax) + col;
				m_pVertices[A].p.y += m_fRowFileHeight*GetGaussian(countRow, countCol, fRho);
				if (col + 1 < m_nColMax)
				{
					int B = (row * m_nVtColMax) + (col + 1);
					if (col + 1> m_nColMax) continue;
					m_pVertices[B].p.y += m_fRowFileHeight* GetGaussian(countRow, countCol, fRho);
				}
				if (row + 1 < m_nRowMax)
				{
					int C = ((row + 1) * m_nVtColMax) + col;
					if (row + 1> m_nRowMax) continue;
					m_pVertices[C].p.y += m_fRowFileHeight * GetGaussian(countRow, countCol, fRho);
				}
				if ((col + 1) < m_nColMax && (row + 1) < m_nRowMax)
				{
					int D = ((row + 1) * m_nVtColMax) + (col + 1);
					if (row + 1> m_nRowMax) continue;
					if (col + 1> m_nColMax) continue;
					m_pVertices[D].p.y += m_fRowFileHeight *GetGaussian(countRow, countCol, fRho);
				}
				countRow++;
			}
			countCol++;
			countRow = Rowrange * (-1);
		}
		SettingNormal();
		SettingVertex();

	}
	void cTerrainData::Polygon()
	{

	}
	void cTerrainData::Smooth()
	{

	}
	void cTerrainData::Slope()
	{

	}

	/*
	픽킹
	*/

	void cTerrainData::MousePicking(OUT D3DXVECTOR3& pos)
	{
		D3DXMATRIXA16 m_mProjection;
		D3DXVECTOR3 dir;

		//마우스 좌표 받아오고 
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &m_mProjection);
		FLOAT w = m_mProjection._11;
		FLOAT h = m_mProjection._22;

		D3DVIEWPORT9 vp;	//Viewport
		g_pD3DDevice->GetViewport(&vp);

		//스크린의 Width / Height
		dir.x = ((2.0f * m_Mouse.x) / vp.Width - 1.0f) / w;
		dir.y = ((-2.0f * m_Mouse.y) / vp.Height + 1.0f) / h;
		dir.z = 1.0f;

		D3DXVec3Normalize(&dir, &dir);

		D3DXMATRIXA16 m_mView, m_mInView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &m_mView);
		D3DXMatrixInverse(&m_mInView, 0, &m_mView);
		D3DXVec3TransformNormal(&dir, &dir, &m_mInView);
		m_vMousedir = dir;

		D3DXVECTOR3 vPos = D3DXVECTOR3((float)m_Mouse.x, (float)m_Mouse.y, 0);
		D3DXVec3Unproject(&vPos, &vPos, &vp, &m_mProjection, &m_mView, NULL);

		float fShortDistance;
		bool bIsFristCheck = true;

		for (int col = 0; col < m_nColMax; ++col)
		{
			for (int row = 0; row < m_nRowMax; ++row)
			{
				float	fU, fV, fDistance;
				// A - C
				// | / |
				// B - D
				int A = (row * m_nVtColMax) + col;
				m_pVertices[A].p;
				int B = (row * m_nVtColMax) + (col + 1);
				m_pVertices[B].p;
				int C = ((row + 1) * m_nVtColMax) + col;
				m_pVertices[C].p;
				int D = ((row + 1) * m_nVtColMax) + (col + 1);
				m_pVertices[D].p;
				//위 
				if (D3DXIntersectTri(&m_pVertices[A].p, &m_pVertices[C].p, &m_pVertices[D].p, &vPos, &dir, &fU, &fV, &fDistance))
				{
					if (bIsFristCheck)
					{
						fShortDistance = fDistance;
						bIsFristCheck = false;
					}
					float fcos = D3DXVec3Dot(&m_pVertices[A].n, &dir);
					float radian = acosf(fcos);
					float angle = D3DXToDegree(radian);
					if ((fShortDistance >= fDistance) && angle > 90)
					{
						fShortDistance = fShortDistance;
						m_vMousePos = m_pVertices[A].p + fU * (m_pVertices[C].p - m_pVertices[A].p) + fV * (m_pVertices[B].p - m_pVertices[A].p);
						m_nSelectedRow = row; m_nSelectedCol = col;
						pos = m_vMousePos;

						//브러쉬 상태
						if (m_nBrushType != BRUSH_STATE_TYPE::STATE_SLOPE)
							PickCircle();
						else
							PickPoint();
						//::MessageBox(0, L"DD", 0, 0);

					}
				}
				//아래 
				if (D3DXIntersectTri(&m_pVertices[D].p, &m_pVertices[B].p, &m_pVertices[C].p, &vPos, &dir, &fU, &fV, &fDistance))
				{
					if (bIsFristCheck)
					{
						fShortDistance = fDistance;
						bIsFristCheck = false;
					}
					float fcos = D3DXVec3Dot(&m_pVertices[D].n, &dir);
					float radian = acosf(fcos);
					float angle = D3DXToDegree(radian);
					if ((fShortDistance >= fDistance) && angle > 90)
					{
						fShortDistance = fShortDistance;
						m_vMousePos = m_pVertices[D].p + fU * (m_pVertices[B].p - m_pVertices[D].p) + fV * (m_pVertices[C].p - m_pVertices[D].p);
						m_nSelectedRow = row; m_nSelectedCol = col;
						pos = m_vMousePos;
						//브러쉬 상태
						if (m_nBrushType != BRUSH_STATE_TYPE::STATE_SLOPE)
							PickCircle();
						else
							PickPoint();
					}
				}
			}
		}

	}

	void cTerrainData::PickCircle()
	{
		const float angle = D3DX_PI*2.0f / (BRUSH_LINE_MAX - 1);
		D3DXVECTOR3 iniPos = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXVECTOR3 rotatedPos;

		D3DXMATRIX mat;
		D3DXMatrixRotationY(&mat, angle * 0);
		D3DXVec3TransformCoord(&rotatedPos, &iniPos, &mat);
		D3DXVec3Normalize(&rotatedPos, &rotatedPos);
		HeightLine[0].p = m_fRadius * rotatedPos + m_vMousePos;
		HeightLine[0].p.y += 0.01f;
		HeightLine[0].c = D3DCOLOR_XRGB(0, 0, 255); // Blue  

		for (int i = 0; i< BRUSH_LINE_MAX; ++i)
		{
			D3DXMatrixRotationY(&mat, angle*i);
			D3DXVec3TransformCoord(&rotatedPos, &iniPos, &mat);
			D3DXVec3Normalize(&rotatedPos, &rotatedPos);
			HeightLine[i].p = m_fRadius * rotatedPos + m_vMousePos;
			GetHeightmapPos(HeightLine[i].p);
			HeightLine[i].p.y += 0.1f;
			HeightLine[i].c = D3DCOLOR_XRGB(0, 0, 255);
		}

		PaintLine[0].p = m_fRadius * rotatedPos + m_vMousePos;
		PaintLine[0].p.y += 0.01f;
		PaintLine[0].c = D3DCOLOR_XRGB(255, 0, 0);

		for (int i = 0; i< BRUSH_LINE_MAX; ++i)
		{
			D3DXMatrixRotationY(&mat, angle*i);
			D3DXVec3TransformCoord(&rotatedPos, &iniPos, &mat);
			D3DXVec3Normalize(&rotatedPos, &rotatedPos);
			PaintLine[i].p = m_fPaintRadius * rotatedPos + m_vMousePos;
			GetHeightmapPos(PaintLine[i].p);
			PaintLine[i].p.y += 0.1f;
			PaintLine[i].c = D3DCOLOR_XRGB(255, 0, 0);
		}
	}
	void cTerrainData::PickPoint()
	{
		//switch (m_SlopeState)
		//{
		//case SLOPE_STATE::STATE_FIRST_CLICK:
		//	HeightLineEx[0].p = m_vMousePos;
		//	HeightLine[0].p.y += 0.01f;
		//	HeightLine[0].c = D3DCOLOR_XRGB(0, 0, 255); // Blue  
		//	m_SlopeState = SLOPE_STATE::STATE_FIRST_CLICKED;
		//	break;
		//case SLOPE_STATE::STATE_FIRST_CLICKED:
		//	HeightLineEx[1].p = m_vMousePos;
		//	HeightLine[1].p.y += 0.01f;
		//	HeightLine[1].c = D3DCOLOR_XRGB(0, 0, 255); // Blue  
		//	break;
		//}
	}
	/*
	높이 값
	*/
	float cTerrainData::GetHeightmapCell(int row, int col)
	{
		if (row < 0 || row > m_nRowMax) return 0;
		if (col < 0 || col > m_nColMax) return 0;
		int index = (row * m_nVtColMax) + col;
		return m_pVertices[index].p.y;
	}

	bool cTerrainData::GetHeightmapPos(OUT D3DXVECTOR3& pos)
	{
		D3DXVECTOR3 oldPos = pos;

		// 맵 밖! 
		if (pos.z > m_fDepth / 2.0f || pos.z < -m_fDepth / 2.0f || pos.x > m_fWidth / 2.0f || pos.x < -m_fWidth / 2.0f)
		{
			pos.y = 0.0f;
			return true;
		}

		float x = ((m_fWidth / 2.0f) + pos.x) / m_fCellSpacing;
		float z = ((m_fDepth / 2.0f) - pos.z) / m_fCellSpacing;

		// 소수점 이하 버리기 
		int col = floorf(x);
		int row = floorf(z);

		float A = GetHeightmapCell(row, col);
		float B = GetHeightmapCell(row, col + 1);
		float C = GetHeightmapCell(row + 1, col);
		float D = GetHeightmapCell(row + 1, col + 1);

		// A - C
		// | / |
		// B - D

		float dx = x - col;
		float dz = z - row;

		if (dz < 1.0f - dx)
		{
			//위 
			float uy = B - A; // B높이값 - A높이값
			float vy = C - A; // C높이값 - A높이값

			pos.y = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
		}
		else
		{
			//아래 
			float uy = C - D;
			float vy = B - D;

			pos.y = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
		}

		//처음 들어온 pos 값이 변경 안되었으면 
		if (oldPos == pos) return false;
		
		//처음 들어온 pos 값이 변경 되었으면 
		return true;
	}
	/*
	Render 
	*/

	void cTerrainData::PrevRender(D3DXMATRIXA16* matWorld)
	{
		if (m_pTexEffect)
		{
			g_pD3DDevice->SetVertexDeclaration(m_pFVF);
		
			m_pTexEffect->SetTechnique("CreateShadow");
			m_pTexEffect->SetMatrix("gWorldMatrix", matWorld);
		
			//m_pTexEffect->SetMatrix("Mat_LightViewProj",); //shaderMgr에서 set해줌
		
			g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNTT_VERTEX));
			g_pD3DDevice->SetIndices(m_pIB);
		
			m_pTexEffect->Begin(NULL, 0);
			m_pTexEffect->BeginPass(0);
		
			g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nAllVt, 0, m_nAllIndex / 3);
		
			m_pTexEffect->EndPass();
			m_pTexEffect->End();
		}
	}
	
	void cTerrainData::AlphaTexRender(D3DXMATRIXA16* matWorld)
	{

		//이펙트 적용.
		m_pTexEffect->SetTechnique("Tech");

		m_pTexEffect->SetMatrix("gWorldMatrix", matWorld);

		//D3DXCOLOR color(1.0f, 1.0f, 1.0f, 0);
		//셰이더 변수 맵핑
		m_pTexEffect->SetMatrix("worldViewProj", &m_WorldViewProj);
		//m_pTexEffect->SetMatrix("matWorldView", &m_WorldView);
		//D3DXVECTOR4 fFog = D3DXVECTOR4(0.66f, 1800.0f, 0.0f, 0.0f);
		//m_pTexEffect->SetVector("fFog", &fFog);

		//D3DXVECTOR4 dir(m_Light.Direction.x, m_Light.Direction.y, m_Light.Direction.z, 0);
		//m_pTexEffect->SetVector("m_LgtDir", &dir);
		//m_pTexEffect->SetVector("m_LgtDif", (D3DXVECTOR4*)&color);

		m_pTexEffect->SetTexture("BaseTexture", m_pTex);
		m_pTexEffect->SetTexture("AlphaTex0", m_SplattingTex[0].amap);
		m_pTexEffect->SetTexture("AlphaTex1", m_SplattingTex[1].amap);
		m_pTexEffect->SetTexture("AlphaTex2", m_SplattingTex[2].amap);
		m_pTexEffect->SetTexture("AlphaTex3", m_SplattingTex[3].amap);
		m_pTexEffect->SetTexture("AlphaTex4", m_SplattingTex[4].amap);
		m_pTexEffect->SetTexture("AlphaTex5", m_SplattingTex[5].amap);
//		m_pTexEffect->SetTexture("AlphaTex6", m_SplattingTex[6].amap);


		m_pTexEffect->SetTexture("SplatTex0", m_SplattingTex[0].tex);
		m_pTexEffect->SetTexture("SplatTex1", m_SplattingTex[1].tex);
		m_pTexEffect->SetTexture("SplatTex2", m_SplattingTex[2].tex);
		m_pTexEffect->SetTexture("SplatTex3", m_SplattingTex[3].tex);
		m_pTexEffect->SetTexture("SplatTex4", m_SplattingTex[4].tex);
		m_pTexEffect->SetTexture("SplatTex5", m_SplattingTex[5].tex);
//		m_pTexEffect->SetTexture("SplatTex6", m_SplattingTex[6].tex);

		m_pTexEffect->SetFloat("TileWidth", (float)m_nRowMax);
		m_pTexEffect->SetFloat("TileHeight", (float)m_nColMax);

		//픽셀 쉐이더 FVF설정.
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNTT_VERTEX));
		g_pD3DDevice->SetVertexDeclaration(m_pFVF);
		g_pD3DDevice->SetIndices(m_pIB);

		m_pTexEffect->Begin(NULL, 0);
		m_pTexEffect->BeginPass(0);

		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nAllVt, 0, m_nAllIndex / 3);

		m_pTexEffect->EndPass();
		m_pTexEffect->End();

		// 쉐이더 사용 해제
		g_pD3DDevice->SetVertexDeclaration(NULL);
		g_pD3DDevice->SetVertexShader(NULL);
		g_pD3DDevice->SetPixelShader(NULL);

		//FOG 사용 해제
//		g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		//행렬 초기화.
		D3DXMATRIXA16 mIdWorld;
		D3DXMatrixIdentity(&mIdWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &mIdWorld);
	}
	//브러쉬 
	void cTerrainData::BrushRender()
	{
		g_pD3DDevice->SetFVF(ST_BURSH_PC_VERTEX::FVF);
		if (m_nBrushType != BRUSH_STATE_TYPE::STATE_SLOPE)
		{
			for (int i = 1; i<BRUSH_LINE_MAX; ++i)
			{
				HeightLineEx[0] = HeightLine[i - 1];
				HeightLineEx[1] = HeightLine[i];
				g_pD3DDevice->SetMaterial(&m_HeightBrushMtl);
				g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, HeightLineEx, sizeof(ST_BURSH_PC_VERTEX));

				PaintLineEx[0] = PaintLine[i - 1];
				PaintLineEx[1] = PaintLine[i];
				g_pD3DDevice->SetMaterial(&m_PaintBrushMtl);
				g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, PaintLineEx, sizeof(ST_BURSH_PC_VERTEX));
			}
		}
		else
		{
			//if (m_SlopeState != STATE_SLOPE::STATE_NONE_CLICK)
			//{
			//	g_pD3DDevice->SetMaterial(&m_HeightBrushMtl);
			//	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, HeightLineEx, sizeof(ST_BURSH_PC_VERTEX));
			//}
		}
	}
	void cTerrainData::Render(D3DXMATRIXA16* matWorld)
	{
		// SY to 진호  - 책갈피 Light 관련 있으니 꼭 고쳐주세영 . 
		//light 
		g_pD3DDevice->LightEnable(0, TRUE);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		g_pD3DDevice->GetLight(0, &m_Light);


		D3DXMatrixIdentity(&m_WorldViewProj);

		//셰이더에서 쓰일 행렬 계산.
	//	D3DXMATRIX mView = g_pCameraManager->GetCurrentCamera()->GetMatView();
		D3DXMATRIX mProj = g_pCameraManager->GetCurrentCamera()->GetMatProj();
		D3DXMATRIX mView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		//g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mProj);
		D3DXMATRIX mWorld = *matWorld;
		m_WorldView = mWorld* mView;
		m_WorldViewProj = m_WorldView * mProj;

		if (m_bIsWireFrameMode)
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		else
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if (bIsAlphaTex)
		{
			AlphaTexRender(matWorld);
		} 
		else
		{
			g_pD3DDevice->LightEnable(0, FALSE);
			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			g_pD3DDevice->SetTransform(D3DTS_WORLD, matWorld);
			g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNTT_VERTEX));
			g_pD3DDevice->SetFVF(ST_PNTT_VERTEX::FVF);
			g_pD3DDevice->SetIndices(m_pIB);

			g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nAllVt, 0, m_nAllIndex / 3);


		}

		if ((m_nBrushType != BRUSH_STATE_TYPE::STATE_NONE) && (m_nBrushType != BRUSH_STATE_TYPE::STATE_OBJECT_PICK))
			BrushRender();

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		D3DXMATRIXA16 world; D3DXMatrixIdentity(&world);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);
	}
	
}