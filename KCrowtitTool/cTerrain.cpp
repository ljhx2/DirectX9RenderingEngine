#include "stdafx.h"
#include "cTerrain.h"

cTerrain::cTerrain()
{
	m_pVB = NULL;
	m_pIB = NULL;
	m_Indices = NULL;
	m_ErrorMessage = NULL;
//	m_pTex = NULL;
	m_pTexHeight = NULL;
	m_Vertices = NULL;
	m_RowNumMax = 0;
	m_ColNumMax = 0;

	m_VtRowNumMax = 0;
	m_VtColNumMax = 0;

	m_AllVtNum = 0;
	m_AllIndexNum = 0;
	m_RowSpace = 0;
	m_ColSpace = 0;

	m_StartROW = 0;
	m_StartCOL = 0;
	m_fRowFileHeight = 1.0f;
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vAngle = D3DXVECTOR3(0, 0, 0);
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMatrixScaling(&m_mScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&m_mRotation, m_vAngle.y, m_vAngle.x, m_vAngle.z);
	D3DXMatrixTranslation(&m_mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_World = m_mScale * m_mRotation * m_mTranslation;
}
cTerrain::~cTerrain()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pTexHeight);
	//SAFE_DELETE(m_pTex);
	SAFE_DELETE(m_Indices);
	SAFE_DELETE(m_ErrorMessage);
	SAFE_DELETE(m_Vertices);
	m_Heightmap.clear();

}

void cTerrain::Setup(char * szHeightFileName, char * szTextureFileName, float fRowFileHeight, float fScaleX, float fScaleY, float fScaleZ)
{

	char * fileName = szHeightFileName;
	m_fRowFileHeight = fRowFileHeight;
	m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);

	char fileExtension[12];
	memset(fileExtension, 0, sizeof(fileExtension));

	if (!szHeightFileName) return;

	do
	{
		if (fileName[0] == '.')
		{
			fileName++;
			strcpy(fileExtension, fileName);
			break;
		}
		fileName++;
	} while (strlen(fileName) > 0);

	if ((!strcmp("bmp", _strlwr(fileExtension))) || (!strcmp("png", _strlwr(fileExtension))))
	{
		if (!ReadBMPFile(szHeightFileName))
		{
			::MessageBox(0, m_ErrorMessage, 0, 0);
			::PostQuitMessage(0);
		}
	}
	else if (!strcmp("raw", _strlwr(fileExtension)))
	{
		ReadRawFile(szHeightFileName);
	}


	//TCHAR * wsTextureFileName = CharToTCHAR(szTextureFileName);
	//m_pTex = D3DTEXTURE->AddTexture(szTextureFileName, wsTextureFileName);

	//index
	int index = 0;
	for (int col = 0; col < m_ColNumMax; ++col)
	{
		for (int row = 0; row < m_RowNumMax; ++row)
		{
			//사각형 1개  ex) 5
			m_Indices[index++] = (m_VtRowNumMax * col) + row;						//0
			m_Indices[index++] = ((m_VtRowNumMax *col) + m_VtRowNumMax + 1) + row;	//6 
			m_Indices[index++] = ((m_VtRowNumMax *col) + m_VtRowNumMax) + row;		//5 
			m_Indices[index++] = (m_VtRowNumMax * col) + row;						//0
			m_Indices[index++] = ((m_VtRowNumMax *col) + 1) + row;					//1
			m_Indices[index++] = ((m_VtRowNumMax *col) + m_VtRowNumMax + 1) + row;	//6
		}
	}

	//풀스크린 모드 할땐 인덱스 & 버텍스 버퍼를 D3DPOOL_MANAGED 로 바꿔야 한다. 
	//D3DPOOL_DEFAULT로 하면 안됨
	g_pD3DDevice->CreateIndexBuffer(sizeof(WORD)* m_AllIndexNum, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL);

	LPVOID pIndices;
	m_pIB->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, m_Indices, sizeof(WORD)* m_AllIndexNum);
	m_pIB->Unlock();

}

void cTerrain::SettingNormal()
{
	int index = 0;

	D3DXVECTOR3	Normal(0, 0, 0);
	D3DXVECTOR3	v0, v1, v2;
	D3DXVECTOR3	Nor;

	for (int col = 0; col < m_ColNumMax; ++col)
	{
		for (int row = 0; row < m_RowNumMax; ++row)
		{
			//int index = (col * m_ColNumMax) + row;
			int index = (col * m_VtColNumMax) + row;

			float heightA = GetHeightmapCell(row, col);
			float heightB = GetHeightmapCell(row, col + 1);
			float heightC = GetHeightmapCell(row + 1, col);

			//UV 으음.. 
			D3DXVECTOR3 u(m_RowSpace, heightB - heightA, 0.0f);
			D3DXVECTOR3 v(0.0f, heightC - heightA, -m_RowSpace);

			D3DXVECTOR3 Nor;
			D3DXVec3Cross(&Nor, &u, &v);
			D3DXVec3Normalize(&Nor, &Nor);
			m_Vertices[index].n = Nor;

		}
	}

}
void cTerrain::GetHeightmapPos(OUT D3DXVECTOR3& pos)
{

	float x = m_RowNumMax + pos.x;
	float z = m_ColNumMax - pos.z;

	int row = x / m_RowSpace;
	int col = z / m_ColSpace;

	if (row < 0 || row > m_RowNumMax || col < 0 || col > m_ColNumMax)
	{
		pos.y = 0.0f;
		return;
	}


	float v0 = GetHeightmapCell(row, col);		//0
	float v1 = GetHeightmapCell(row, col + 1);	//5
	float v2 = GetHeightmapCell(row + 1, col);	//1
	float v3 = GetHeightmapCell(row + 1, col + 1);//6

	//1 - 6
	//| / |
	//0 - 5

	float dx = x - row*m_RowSpace;
	float dz = z - col*m_ColSpace;

//	DEBUG->Log("%d %d %f %f", row, col, dx, dz);

	//위쪽 삼각형 
	if (dz < 1.0f - dx)
	{
		float uy = v1 - v0;
		float vy = v2 - v0;

		//이부분 완벽하지 못하다.. 어렵다. .선형보간  공부 할 것 
		pos.y = v0 + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else
	{
		//아래쪽 삼각형 

		float uy = v2 - v3;
		float vy = v1 - v3;

		pos.y = v3 + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	//	DEBUG->Log("%d %d %f", row, col , height);
}
float cTerrain::GetHeightmapCell(int row, int col)
{
	int index = (col * m_VtColNumMax) + row;
	return m_Vertices[index].p.y;
}
BOOL cTerrain::ReadRawFile(char * szHeightFileName)
{
	FILE * filePointer;

	fopen_s(&filePointer, szHeightFileName, "rb");
	//파일 없으면 에러 
	if (!filePointer) {
		char strMessage[255] = { 0 };
		strcpy_s(strMessage, szHeightFileName);
		strcat_s(strMessage, "__Terrain Tex File Can't Not Found");
		::MessageBox(NULL, CharToTCHAR(strMessage), L"Error", MB_OK);
		return false;
	}


	fseek(filePointer, 0, SEEK_END);

	long lSize = ftell(filePointer);

	fseek(filePointer, 0, SEEK_SET);
	BYTE * pH = new BYTE[lSize];

	fread(pH, lSize, 1, filePointer);

	fclose(filePointer);


	SettingInfo((int)sqrt(lSize) - 1, (int)sqrt(lSize) - 1);	//기본 정보 셋팅 

	///-------------------------------------------------------------------------
	m_Vertices = new ST_PNT_VERTEX[m_AllVtNum];
	m_Indices = new WORD[m_AllIndexNum];

	int index = 0;
	//버텍스 
	for (int col = 0; col < m_VtColNumMax; ++col)
	{
		for (int row = 0; row < m_VtRowNumMax; ++row)
		{
			//int index = (col * m_VtColNumMax) + row;
			FLOAT _y = pH[col * (m_VtColNumMax)+row] * m_fRowFileHeight;

			m_Vertices[index].p = D3DXVECTOR3(m_StartROW + row * m_RowSpace, _y, m_StartCOL + col * -m_ColSpace);
			m_Vertices[index].n = m_Vertices[index].p;
			D3DXVec3Normalize(&m_Vertices[index].n, &m_Vertices[index].n);
			m_Vertices[index].t = D3DXVECTOR2(row * (1.0f / (float)m_VtRowNumMax), col* (1.0f / (float)m_VtColNumMax));
			index++;
			//	DEBUG->Log("X : %f  Y : %f  Z : %f || Index : %d", m_Vertices[index].p.x, m_Vertices[index].p.y, m_Vertices[index].p.z, index);
		}
	}

	SettingNormal();

	g_pD3DDevice->CreateVertexBuffer(sizeof(ST_PNT_VERTEX)* m_AllVtNum, 0,
		ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	VOID * pVertices;
	if (m_pVB->Lock(0, sizeof(ST_PNT_VERTEX)* m_AllVtNum, (void**)&pVertices, 0))
	{
		lstrcat(m_ErrorMessage, _T("m_pVB->lock() - FAILED"));
		return FALSE;
	}
	memcpy(pVertices, m_Vertices, sizeof(ST_PNT_VERTEX)* m_AllVtNum);
	if (m_pVB->Unlock())
	{
		lstrcat(m_ErrorMessage, _T("m_pVB->Unlock() - FAILED"));
		return FALSE;
	}
	SAFE_DELETE(pH);

	return TRUE;
}
BOOL cTerrain::ReadBMPFile(char * szHeightFileName)
{

	TCHAR * wsFileName = CharToTCHAR(szHeightFileName);
	//높이맵 
	if ((D3DXCreateTextureFromFileEx(g_pD3DDevice, wsFileName,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexHeight)))
	{

		lstrcat(wsFileName, _T("___Terrain-D3DXCreateTextureFromFileEx() - FAILED"));
		m_ErrorMessage = wsFileName;
		return FALSE;
	}

	/*
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	*/
	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;

	m_pTexHeight->GetLevelDesc(0, &ddsd);	// 텍스처의 정보
	SettingInfo(ddsd.Width, ddsd.Height);	//기본 정보 셋팅 

	//텍스쳐 메모리 락
	if (m_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY))
	{
		lstrcat(m_ErrorMessage, _T("m_pTexHeight->lock() - FAILED"));
		return FALSE;
	}
	///-------------------------------------------------------------------------
	m_Vertices = new ST_PNT_VERTEX[m_AllVtNum];
	m_Indices = new WORD[m_AllIndexNum];

	int index = 0;
	//버텍스 
	for (int col = 0; col < m_VtColNumMax; ++col)
	{
		for (int row = 0; row < m_VtRowNumMax; ++row)
		{
			m_Vertices[index].p = D3DXVECTOR3(m_StartROW + row * m_RowSpace, ((float)(*((LPDWORD)d3drc.pBits + row + col*(d3drc.Pitch / 4)) & 0x000000ff)) / 10.0f, m_StartCOL + col * -m_ColSpace);
			m_Vertices[index].n = m_Vertices[index].p;
			D3DXVec3Normalize(&m_Vertices[index].n, &m_Vertices[index].n);
			m_Vertices[index].t = D3DXVECTOR2(row * (1.0f / (float)m_VtRowNumMax), col* (1.0f / (float)m_VtColNumMax));
			index++;
		}
	}

	SettingNormal();
	//텍스쳐 메모리 언락
	m_pTexHeight->UnlockRect(0);

	g_pD3DDevice->CreateVertexBuffer(sizeof(ST_PNT_VERTEX)* m_AllVtNum, 0,
		ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	LPVOID pVertices;
	if (m_pVB->Lock(0, sizeof(ST_PNT_VERTEX)* m_AllVtNum, (void**)&pVertices, 0))
	{
		lstrcat(m_ErrorMessage, _T("m_pVB->lock() - FAILED"));
		return FALSE;
	}
	memcpy(pVertices, m_Vertices, sizeof(ST_PNT_VERTEX)* m_AllVtNum);
	if (m_pVB->Unlock())
	{
		lstrcat(m_ErrorMessage, _T("m_pVB->Unlock() - FAILED"));
		return FALSE;
	}

	return TRUE;
}
void cTerrain::SettingInfo(int row, int col)
{
	m_RowNumMax = row;				// 텍스처의 가로크기
	m_ColNumMax = col;				// 텍스처의 세로크기

	m_VtRowNumMax = m_RowNumMax + 1;
	m_VtColNumMax = m_ColNumMax + 1;

	m_AllVtNum = (m_VtRowNumMax  * m_VtColNumMax);
	m_AllIndexNum = (m_RowNumMax * m_ColNumMax) * 6; // * 2 * 3

	m_RowSpace = 2;
	m_ColSpace = 2;

	m_StartROW = -m_RowNumMax / 2 * (m_RowSpace);
	m_StartCOL = m_ColNumMax / 2 * (m_ColSpace);
}

void cTerrain::Update()
{
	D3DXMatrixScaling(&m_mScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&m_mRotation, m_vAngle.y, m_vAngle.x, m_vAngle.z);
	D3DXMatrixTranslation(&m_mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_World = m_mScale * m_mRotation * m_mTranslation;
}
void cTerrain::Render()
{
	//텍스쳐 
	//m_pTex->SetTexture(0);// 0번 텍스쳐 스테이지에 텍스쳐 고정
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	/// 0번 텍스처 스테이지의 확대 필터
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		/// 0번 텍스처 : 0번 텍스처 인덱스 사용
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//매쉬 ----------------------------------
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_World);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetIndices(m_pIB);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_AllVtNum, 0, m_AllIndexNum / 3);
	g_pD3DDevice->SetTexture(0, NULL);

}