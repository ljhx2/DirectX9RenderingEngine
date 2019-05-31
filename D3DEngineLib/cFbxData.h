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
	int				type;//��Ų�� �븻���� ��Ż���� ���� ����
	/*
	FbxCluster::eAdditive	= 0;
	FbxCluster::eNormalize	= 1;
	FbxCluster::eTotalOne	= 2;
	else					= 1;//�� �� �ϰ�� �� �븻�� ����
	*/
	D3DXMATRIX*	pMat;//��� �迭 ����
	int				matSize;
};

struct FBX_DATA_GROUP
{
	FbxNode*				pNode;//�ش� �׷������� ��� ������
	int						nNumTri;
	char					sMtlName[256];
	D3DMATERIAL9			stMtl;
	LPDIRECT3DVERTEXBUFFER9	pVB;
	LPDIRECT3DVERTEXBUFFER9	bufVB;
	FBX_SKIN_DATA			skinData;
	//�ش� �׷��� �ִ� ���
	
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

		//��� �ð�, ���� �ִ� �ð�
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
		//----------- �׸��� -----------------
		LPD3DXEFFECT			m_pApplyShadowShader = NULL;
		LPD3DXEFFECT			m_pCreateShadowShader = NULL;



		//���� ��� ����
		char								m_FilePath[256];
		//���� �̸� ����
		char								m_FileName[128];
		//������Ʈ ���� ������
		std::vector<FBX_DATA_GROUP*>		m_Groups;
		//������Ʈ ����,�ؽ��� ������
		std::vector<FBX_MTL_TEX*>			m_MtlTexMap;
		FbxArray<SubMesh*>					mSubMeshes;
		bool mHasNormal;
		bool mHasUV;
		bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.

		// �ٿ�����Ǿ�
		D3DXVECTOR3					m_vMin;
		D3DXVECTOR3					m_vMax;

	public://������Ʈ �ε� �Լ�
		void FillCameraArray(FbxScene* pScene, FbxArray<FbxNode*>& pCameraArray);
		void FillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray);
		void FillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray);
		void LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName);
		void LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer);

		void PreparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop);

		bool CreateDataGroup(FBX_DATA_GROUP* pGroup, const FbxMesh *pMesh);
		//�� �������� weight���� �ش� 
		void InitVertexAndIndex(FBX_DATA_GROUP* pGroup);
		void GetMaterialValue(const FbxSurfaceMaterial* pMaterial, D3DMATERIAL9* mtl);
		FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName);
		void UpdateBipMatrixMap(std::map<std::string, D3DXMATRIXA16>* vpBipMap, D3DXMATRIXA16& worldMat);
		void UpdateBipNodeSearch(FbxNode * pNode, std::map<std::string, D3DXMATRIXA16>* vpBipMap, FbxPose* pPose, D3DXMATRIXA16& worldMat);
	public://update�Լ�
		void UpdateGroup(FBX_DATA_GROUP* pGroup, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose);
	public://render�Լ�
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
	public://��� �Լ�
		//��ü ���+�̸�+Ȯ���� ���� �̸��� ����
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
		//�ִϸ��̼� ��
		void StartAnim(int nIndex, int nStart, int nEnd, FbxTime* pStart, FbxTime* pEnd, FbxTime* pCurrent);		//AnimComponent���� ���
		//�浹�ڽ� ũ�� ���ϴ� �Լ�
		void SetCollisionSphere(OUT D3DXVECTOR3* _vMin, OUT D3DXVECTOR3* _vMax);
		void GetMinMaxVertex(FbxNode* pNode, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, bool* pIsInitCollisionBox);
	public:
		int GetGroupDataNum(){ return m_Groups.size(); }
		const char* GetGroupDataName(int i){ return m_Groups[i]->pNode->GetName(); }
	};
}
