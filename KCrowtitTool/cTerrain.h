#pragma once 

class cTerrain
{
	LPDIRECT3DVERTEXBUFFER9 m_pVB; // 정점을 보관할 정점버퍼
	LPDIRECT3DINDEXBUFFER9	m_pIB; // 인덱스를 보관할 인덱스버퍼

	LPDIRECT3DTEXTURE9		m_pTexHeight; // Texture 높이맵
//	cTexture				*m_pTex; // Texture MAP

	WORD					* m_Indices;
	TCHAR					* m_ErrorMessage;

	ST_PNT_VERTEX			* m_Vertices;
	std::vector<char> m_Heightmap;

	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vAngle;
	D3DXVECTOR3		m_vScale;

	D3DXMATRIXA16	m_mScale;
	D3DXMATRIXA16	m_mRotation;
	D3DXMATRIXA16	m_mTranslation;

	D3DXMATRIXA16	m_World;

	int m_RowNumMax;
	int m_ColNumMax;

	int m_VtRowNumMax;
	int m_VtColNumMax;

	int m_AllVtNum;
	int m_AllIndexNum;
	int m_RowSpace;
	int m_ColSpace;

	int m_StartROW;
	int m_StartCOL;

	float m_fRowFileHeight;

public:
	cTerrain();
	~cTerrain();

	void Setup(char * szHeightFileName, char * szTextureFileName, float fRowFileHeight = 0, float fScaleX = 1.0f, float fScaleY = 1.0f, float fScaleZ = 1.0f);
	void Update();
	void Render();

	void SettingInfo(int row, int col);
	void SettingNormal();

	float GetHeightmapCell(int row, int col);
	void GetHeightmapPos(OUT D3DXVECTOR3& pos);

	BOOL ReadRawFile(char * szHeightFileName);
	BOOL ReadBMPFile(char * szHeightFileName);
};