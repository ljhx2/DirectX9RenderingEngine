#pragma once
#include "fbxsdk.h"
#include "FbxUtil.h"

typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;


struct FBX_SKIN_WEIGHT
{
	float weight[4];
	int index[4];
	int weightNum;
};

struct FBX_MESH_DATA_PNTi
{
	D3DXVECTOR4		p;
	D3DXVECTOR3		n;
	D3DXVECTOR2		t;
	D3DXVECTOR4		weight;
	D3DXVECTOR4		index;
	D3DXVECTOR3		tangent;
	D3DXVECTOR3		biNormal;
	static const DWORD FVF;
};


struct FBX_SKIN_DATA
{
	int				type;//스킨이 노말인지 토탈인지 여부 저장
	/*
	FbxCluster::eAdditive	= 0;
	FbxCluster::eNormalize	= 1;
	FbxCluster::eTotalOne	= 2;
	else					= 1;//그 외 일경우 걍 노말로 하자
	*/
	D3DXMATRIX*	pMat;//행렬 배열 저장
	int				matSize;
};

struct FBX_DATA_GROUP
{
	FbxNode*				pNode;//해당 그룹정보의 노드 포인터
	int						nNumTri;
	char					sMtlName[256];
	D3DMATERIAL9			stMtl;
	LPDIRECT3DVERTEXBUFFER9	pVB;
	LPDIRECT3DVERTEXBUFFER9	bufVB;
	FBX_SKIN_DATA			skinData;
	//해당 그룹의 애니 행렬
	
	FBX_DATA_GROUP() : nNumTri(0), pVB(NULL) {
		memset(sMtlName, 0, 256);
		ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));
		stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		stMtl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		skinData.pMat = NULL;
		skinData.matSize = 0;
		bufVB = NULL;
	}
	~FBX_DATA_GROUP()
	{
		if (pVB){
			pVB->Release();
			pVB = NULL;
		}
		if (bufVB){
			bufVB->Release();
			bufVB = NULL;
		}
		if (skinData.pMat){
			delete[] skinData.pMat;
			skinData.pMat = NULL;
		}
	}
};

struct SubMesh
{
	SubMesh() : IndexOffset(0), TriangleCount(0) {}

	int IndexOffset;
	int TriangleCount;
};

struct FBX_MTL_TEX
{
	char				path[256];
	D3DMATERIAL9		stMtl;
	LPDIRECT3DTEXTURE9	DiffuseTex;
	LPDIRECT3DTEXTURE9	NormalMapTex;
	LPDIRECT3DTEXTURE9	SpecularMapTex;
	//LPDIRECT3DBASETEXTURE9
	FBX_MTL_TEX()
		: DiffuseTex(NULL),
		NormalMapTex(NULL),
		SpecularMapTex(NULL)
	{
		memset(path, 0, 256);
		ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));
		stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		stMtl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	~FBX_MTL_TEX()
	{
		if (DiffuseTex){
			DiffuseTex->Release();
			DiffuseTex = NULL;
		}
		if (NormalMapTex){
			NormalMapTex->Release();
			NormalMapTex = NULL;
		}
		if (SpecularMapTex){
			SpecularMapTex->Release();
			SpecularMapTex = NULL;
		}
	}
};
namespace ENGINE
{
	class cFbxData{
		friend class cFbxComponent;
	public:
		cFbxData();
		~cFbxData();

		void Update(FbxTime* curTime);
		void PrevRender(FbxTime* curTime, D3DXMATRIXA16& worldMat);
		void Render(FbxTime* curTime, D3DXMATRIXA16& worldMat, std::map<std::string, FBX_MTL_TEX*>* pMeterialMap);
		void Destroy();
		void Load(FbxManager* pFbxManager, char* szFolder, const char* szFilename);
	private:
		FbxScene*				pFbxScene;

		FbxAnimLayer*			mCurrentAnimLayer;
		FbxArray<FbxString*>	mAnimStackNameArray;
		FbxArray<FbxNode*>		mCameraArray;
		FbxArray<FbxPose*>		mPoseArray;

		//경과 시간, 현재 애니 시간
		mutable FbxTime mFrameTime, mCurrentTime;// , mStart, mStop, mCurrentTime;
		mutable FbxTime mCache_Start, mCache_Stop;

		int mPoseIndex;

		//-----------------------------
		IDirect3DVertexDeclaration9*		m_pDecl;
		LPD3DXEFFECT						m_ToonEffect;
		D3DXVECTOR4							MeshColors;
		IDirect3DTexture9*					ShadeTex = 0;
		D3DXHANDLE							WorldViewHandle = 0;
		D3DXHANDLE							WorldViewProjHandle = 0;
		D3DXHANDLE							ColorHandle = 0;
		D3DXHANDLE							LightDirHandle = 0;
		D3DXHANDLE							DiffuseTexHandle = 0;
		D3DXHANDLE							NormalMapTexHandle = 0;
		D3DXHANDLE							SpecularMapTexHandle = 0;
		D3DXHANDLE							ToonTechHandle = 0;
		D3DXHANDLE							ControlMatrixArray = 0;
		D3DXHANDLE							Model_Type = 0;
		//------------------------------
		//----------- 그림자 -----------------
		LPD3DXEFFECT			m_pApplyShadowShader = NULL;
		LPD3DXEFFECT			m_pCreateShadowShader = NULL;



		//파일 경로 저장
		char								m_FilePath[256];
		//파일 이름 저장
		char								m_FileName[128];
		//오브젝트 정점 데이터
		std::vector<FBX_DATA_GROUP*>		m_Groups;
		//오브젝트 재질,텍스쳐 데이터
		std::vector<FBX_MTL_TEX*>			m_MtlTexMap;
		FbxArray<SubMesh*>					mSubMeshes;
		bool mHasNormal;
		bool mHasUV;
		bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.

		// 바운딩스피어
		D3DXVECTOR3					m_vMin;
		D3DXVECTOR3					m_vMax;

	public://오브젝트 로드 함수
		void FillCameraArray(FbxScene* pScene, FbxArray<FbxNode*>& pCameraArray);
		void FillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray);
		void FillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray);
		void LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName);
		void LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer);

		void PreparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop);

		bool CreateDataGroup(FBX_DATA_GROUP* pGroup, const FbxMesh *pMesh);
		//각 정점마다 weight값과 해당 
		void InitVertexAndIndex(FBX_DATA_GROUP* pGroup);
		void GetMaterialValue(const FbxSurfaceMaterial* pMaterial, D3DMATERIAL9* mtl);
		FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName);
		void UpdateBipMatrixMap(std::map<std::string, D3DXMATRIXA16>* vpBipMap, D3DXMATRIXA16& worldMat);
		void UpdateBipNodeSearch(FbxNode * pNode, std::map<std::string, D3DXMATRIXA16>* vpBipMap, FbxPose* pPose, D3DXMATRIXA16& worldMat);
	public://update함수
		void UpdateGroup(FBX_DATA_GROUP* pGroup, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose);
	public://render함수
		void RenderGroup(FBX_DATA_GROUP* pGroup, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, LPD3DXEFFECT effect);
		void UpdateVertexPosition(FBX_DATA_GROUP* pGroup, FbxVector4 * pVertices);
		void ReadVertexCacheData(FbxMesh* pMesh, FbxTime& pTime);
		void ComputeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer);
		void ComputeSkinDeformation(FBX_DATA_GROUP* pGroup, FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxPose* pPose);
		void ComputeLinearDeformation(FBX_DATA_GROUP* pGroup, FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxPose* pPose);
		void ComputeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxPose* pPose);
		void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose);

		void MatrixScale(FbxAMatrix& pMatrix, double pValue);
		void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue);
		void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix);
	public://기능 함수
		//전체 경로+이름+확장자 에서 이름만 추출
		void getName(const char* filePath, char* fileName);
		bool SetCurrentAnimStack(int pIndex);
		FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose = NULL, FbxAMatrix* pParentGlobalPosition = NULL);
		FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex);
		FbxAMatrix GetGeometry(FbxNode* pNode);
		void ConvertMatrixD3dToFbx(D3DXMATRIX* d3dMat, FbxAMatrix* fbxMat);
		D3DXVECTOR3 GetMatVectorMulT(D3DXMATRIXA16* d3dMat, D3DXVECTOR3* d3dVet);
		bool IsZeroMatrix(D3DXMATRIXA16* d3dMat);
		D3DXVECTOR4 MulMatrixVector(D3DXMATRIX* pMat, D3DXVECTOR4* pVec);
	public:
		//애니메이션 셋
		void StartAnim(int nIndex, int nStart, int nEnd, FbxTime* pStart, FbxTime* pEnd, FbxTime* pCurrent);		//AnimComponent에서 사용
		//충돌박스 크기 구하는 함수
		void SetCollisionSphere(OUT D3DXVECTOR3* _vMin, OUT D3DXVECTOR3* _vMax);
		void GetMinMaxVertex(FbxNode* pNode, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, bool* pIsInitCollisionBox);
	public:
		int GetGroupDataNum(){ return m_Groups.size(); }
		const char* GetGroupDataName(int i){ return m_Groups[i]->pNode->GetName(); }
	};
}
