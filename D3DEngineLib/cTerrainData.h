#pragma once
#include "cQuadTree.h"

#define BRUSH_LINE_MAX 36						//�귯�� ���� ����
#define TEX_ALPHA_SIZE 512						//���ĸ� ������
#define TEX_MAX 7								//�ؽ��� ���� 


struct ST_BURSH_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	static const DWORD FVF;
};


//ALPHA MAP
typedef struct ST_TEX_ALPHA
{
	LPDIRECT3DTEXTURE9 tex;							//�ؽ���
	LPDIRECT3DTEXTURE9 amap;						//���ĸ�
	ST_TEX_ALPHA()	{ tex = NULL; amap = NULL; }	//�ʱ�ȭ
}SplattingTex;

//���콺 ����
enum ST_MOUSE_STATE
{
	MOUSE_LCLICK,
	MOUSE_RCLICK,
	MOUSE_MOVE,
	MOUSE_LCLICKUP,
};
//�귯�� Ÿ�� 
enum BRUSH_STATE_TYPE
{
	STATE_CURVED,
	STATE_FLATTER,
	STATE_HILL,
	STATE_POLYGON,
	STATE_SLOPE,
	STATE_SMOOTH,
	STATE_PAINT,
	STATE_OBJECT_PICK,			//������Ʈ PICK
	STATE_NONE,
};

namespace ENGINE
{
	//*����*//
	#define SYNTHESIZE(varType, varName, funName)\
	protected: varType varName;\
	public: inline varType Get##funName(void) const { return varName; }\
	public: inline void Set##funName(varType var){ varName = var; }


	class cTerrainData
	{
		///-------------------------------------------------------------------
		int testSize = 8;
		///-------------------------------------------------------------------

		friend class cTerrainComponent;


	public :
		cTerrainData();
		~cTerrainData();
	public :
		//--------------------------------------------------------------
		//�⺻ ����
		//--------------------------------------------------------------
		BOOL SettingSize(int row, int col, float cellSpacing);

		////--------------------------------------------------------------
		////����
		////--------------------------------------------------------------
		void Render(D3DXMATRIXA16* matWorld);
		void PrevRender(D3DXMATRIXA16* matWorld);
		void AlphaTexRender(D3DXMATRIXA16* matWorld);
		void BrushRender();
		////--------------------------------------------------------------
		////���콺 ��ŷ 
		////--------------------------------------------------------------
		void MousePicking(OUT D3DXVECTOR3& pos);
		void PickCircle();
		void PickPoint();

		bool GetHeightmapPos(OUT D3DXVECTOR3& pos);
	public:
		////--------------------------------------------------------------
		////Load & Save
		////--------------------------------------------------------------
		bool LoadHeightMap(const char* szFilename);
		BOOL SaveHeightMap(const char * strPath);

		bool LoadTexture(const char* szFilename); //all load
		BOOL LoadAlphaMap(const char * strPath); //Alphamap load
		BOOL SaveAlphaMap(const char * strPath);//Alphamap save
		////--------------------------------------------------------------
		////������ �귯�� & �ؽ���
		////--------------------------------------------------------------
		////�귯�� ���ĸ� ->���ĸ� ���忡 4�� 
		////RGBA - 4���� ���� ����
		void DrawAlphaMap();
		void ChangePriority(int nFirst, int nSecond);
		void SetGroundTexture(const char * szKeyName);
		//--------------------------------------------------------------
		//���̸� �귯�� 
		//--------------------------------------------------------------
		void UpdateBrush(); //�귯��
		void Hill();
		void Flatter();
		void Curved();
		void Polygon();
		void Smooth();
		void Slope();

	
	private:
		//--------------------------------------------------------------
		//�⺻ ����
		//--------------------------------------------------------------
		void SettingInfo(int row, int col, float *pH);
		BOOL SettingVertex();
		BOOL SettingIndex();
		void SettingNormal();
		BOOL SettingSplatting();
		float GetHeightmapCell(int row, int col);

		////--------------------------------------------------------------
		////���� Ʈ�� + LOD + �÷����� 
		//--------------------------------------------------------------
		//void IndexBufferSetting(cFrustum *  frustum);


		//void SetWireFrame(bool flag) { bIsWireFrame = flag; }
		//void SetGroundTexture(const char * strKey);

		//float GetHeightmapCell(int row, int col);
		//void  GetHeightmapPos(OUT D3DXVECTOR3 & pos);
		//float GetCellSpacing() { return m_fCellSpacing; }

	private :
		RECT getRange(int middlePointX, int middlePointZ, int maxX, int maxZ, float radius, float divide = 1.0f)
		{
			float halfRadius = (radius / divide);
			RECT rt;
			rt.left = int(((middlePointX - halfRadius) < 0) ? 0 : middlePointX - halfRadius);
			rt.top = int(((middlePointZ - halfRadius) < 0) ? 0 : middlePointZ - halfRadius);
			rt.right = int(((middlePointX + halfRadius) >= maxX) ? maxX - 1 : middlePointX + halfRadius);
			rt.bottom = int(((middlePointZ + halfRadius) >= maxZ) ? maxZ - 1 : middlePointZ + halfRadius);

			return rt;
		}
		float GetDistacne(int selectedX, int selectedZ, int targetX, int targetZ)
		{
			D3DXVECTOR3 distance = D3DXVECTOR3((float)targetX, 0.0f, (float)targetZ) - D3DXVECTOR3((float)selectedX, 0.0f, (float)selectedZ);
			return D3DXVec3Length(&distance);
		}
		float GetGaussian(float fX, float fZ, float fRho)
		{
			float g = 1.0f / sqrt(2.0f * D3DX_PI * fRho * fRho);
			return g * exp(-(fX * fX + fZ * fZ) / (2 * fRho * fRho));
		}
		//���� ���� 
		float Lerp(float a, float b, float t)
		{
			return a - (a*t) + (b*t);
		}
		LPDIRECT3DTEXTURE9 m_pTex;							//�ؽ���
		bool bIsWireFrame = false; // ���̾� ������ 

		LPDIRECT3DVERTEXBUFFER9 m_pVB;			// ������ ������ ��������
		LPDIRECT3DINDEXBUFFER9	m_pIB;			// �ε����� ������ �ε�������

		UINT					*m_pIndices;	//�ε��� 
		ST_PNTT_VERTEX			*m_pVertices;	//���ؽ�

		LPDIRECT3DTEXTURE9		m_pTexHeight;	// Texture ���̸�
		D3DMATERIAL9			m_TerrainMtl;	//�ͷ��� mtl 

		int m_nVtRowMax;
		int m_nVtColMax;

		int m_nSelectedRow = 0;
		int m_nSelectedCol = 0;

		int m_nAllVt;
		int m_nAllIndex;

		bool bIsAlphaTex; 
		//�귯�� 
		D3DMATERIAL9 m_HeightBrushMtl;
		D3DMATERIAL9 m_PaintBrushMtl;
		ST_BURSH_PC_VERTEX HeightLine[BRUSH_LINE_MAX];
		ST_BURSH_PC_VERTEX HeightLineEx[2];
		ST_BURSH_PC_VERTEX PaintLine[BRUSH_LINE_MAX];
		ST_BURSH_PC_VERTEX PaintLineEx[2];

		//����Ʈ + ���̴� 
		D3DXMATRIX m_WorldViewProj;		//���� �� ������
		D3DXMATRIX m_WorldView;		//���� ��
		LPD3DXEFFECT	 m_pTexEffect;		//splatting
		LPD3DXEFFECT	 m_pFogEffect;		//fog
		LPDIRECT3DVERTEXDECLARATION9 m_pFogFVF;
		LPDIRECT3DVERTEXDECLARATION9  m_pFVF;
		SplattingTex	 m_SplattingTex[TEX_MAX];//���÷��� �ؽ��� & ���ĸ�
		int m_nTexAlphaSize;

		D3DLIGHT9 m_Light;

		//����Ʈ�� ���� 
		int		m_nTriangles;	/// ����� �ﰢ���� ����
//		cQuadTree * m_pQuadTree;
		float m_fLODRatio;


	public:
		//������/ width 
		SYNTHESIZE(int, m_nStartRow, StartRow);
		SYNTHESIZE(int, m_nStartCol, StartCol);
		SYNTHESIZE(float, m_fWidth, Width);
		SYNTHESIZE(float, m_fDepth, Depth);
		SYNTHESIZE(float, m_fCellSpacing, CellSpacing);
		SYNTHESIZE(int, m_nRowMax, Row);
		SYNTHESIZE(int, m_nColMax, Col);

		//paint 
		SYNTHESIZE(float, m_fRadius, Radius);
		SYNTHESIZE(float, m_fPaintRadius, PaintRadius);
		SYNTHESIZE(POINT, m_Mouse, Mouse);
		SYNTHESIZE(BYTE, m_nBrushType, BrushType);
		SYNTHESIZE(float, m_fRowFileHeight, RawFileHeight);
		SYNTHESIZE(int, m_nCurrentIndex, TexIndex);//���İ� �ε��� ���÷��� 
		SYNTHESIZE(BYTE, m_SlopeState, SlopeState);
		SYNTHESIZE(BOOL, m_bIsPressing, Pressing);
		SYNTHESIZE(BOOL, m_bIsQuadTree, QuadTree);
		SYNTHESIZE(BOOL, m_bIsWireFrameMode, WireFrameMode);
		SYNTHESIZE(BOOL, m_bIsFog, Fog);
		SYNTHESIZE(DWORD, m_dwAlphaColor, AlphaBrush);

		//���콺 
		SYNTHESIZE(D3DXVECTOR3, m_vMousePos, vMousePos);
		SYNTHESIZE(D3DXVECTOR3, m_vMousedir, vMouseDir);
		SYNTHESIZE(ST_MOUSE_STATE, m_eMouseState, MouseState);
	};
}