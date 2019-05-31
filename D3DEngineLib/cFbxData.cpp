#include "Common.h"
#include "include\D3DRenderer.h"
#include "cFbxData.h"

//const DWORD FBX_PNT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
const DWORD FBX_MESH_DATA_PNTi::FVF = (D3DFVF_XYZW | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_XYZW | D3DFVF_XYZW | D3DFVF_XYZ | D3DFVF_XYZ);
int num = 0;
/*
typedef enum _D3DDECLUSAGE{
D3DDECLUSAGE_POSITION = 0,		//위치
D3DDECLUSAGE_BLENDWEIGHTS = 1,	//블렌딩 가중치
D3DDECLUSAGE_BLENDINDICES = 2,	//블렌딩 인덱스
D3DDECLUSAGE_NORMAL = 3,		//법선 벡터
D3DDECLUSAGE_PSIZE = 4,			//버텍스 포인트 크기
D3DDECLUSAGE_TEXCOORD = 5,		//텍스처 좌표
D3DDECLUSAGE_TANGENT = 6,		//접선 벡터
D3DDECLUSAGE_BINORMAL = 7,		//이중법선 벡터
D3DDECLUSAGE_TESSFACTOR = 8,	//세분화 인수
D3DDECLUSAGE_POSITIONT = 9,		//변환된 위치
D3DDECLUSAGE_COLOR = 10,		//컬러
D3DDECLUSAGE_FOG = 11,			//안개 블렌드 값
D3DDECLUSAGE_DEPTH = 12,		//깊이 값
D3DDECLUSAGE_SAMPLE = 13		//샘플러 데이터
} D3DDECLUSAGE;
*/

D3DVERTEXELEMENT9 fbxDecl[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	{ 0, 52, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	{ 0, 68, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	{ 0, 80, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
	D3DDECL_END()
};

int TRIANGLE_VERTEX_COUNT = 3;
// Four floats for every position.
int VERTEX_STRIDE = 4;
// Three floats for every normal.
int NORMAL_STRIDE = 3;
// Two floats for every UV.
int UV_STRIDE = 2;

// char to LPCWSTR
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs(pwstrDest, pstrSrc, nLen);
}

// LPCWSTR to char
void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	int nLen = (int)wcslen(pwstrSrc);
	wcstombs(pstrDest, pwstrSrc, nLen + 1);
}
namespace ENGINE
{
	cFbxData::cFbxData()
	{
		mCurrentAnimLayer = NULL;
		mPoseIndex = -1;
		mFrameTime = 0.0f;
		pFbxScene = NULL;
		//m_pDecl

		HRESULT hr = g_pD3DDevice->CreateVertexDeclaration(fbxDecl, &m_pDecl);

		if (FAILED(hr))		::MessageBox(0, L"CreateVertexDeclaration() - FAILED", 0, 0);	
	}
	cFbxData::~cFbxData()
	{
	}

	void cFbxData::Update(FbxTime* curTime)
	{
		//GetOneFrameValue
		*curTime += mFrameTime;
		/*mCurrentTime = *curTime;

		FbxPose * lPose = NULL;
		if (mPoseIndex != -1)
		{
			lPose = pFbxScene->GetPose(mPoseIndex);
		}

		FbxAMatrix lDummyGlobalPosition;

		for (size_t i = 0; i < m_Groups.size(); ++i){

			if (m_Groups[i]->pVB == NULL)		continue;

			UpdateGroup(m_Groups[i], mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose);
		}*/

	}

	void cFbxData::PrevRender(FbxTime* curTime, D3DXMATRIXA16& worldMat)
	{
		//방향성 광원
		//D3DXVECTOR3 lightPos = D3DXVECTOR3(100, 100, -100);	//빛의 위치
		//D3DXVECTOR3 lightDir = D3DXVECTOR3(-1, -1, 1);	//빛의 방향
		//
		//D3DXMATRIXA16 matDirLightView;	//방향성 광원의 뷰 행렬
		//D3DXMatrixLookAtLH(&matDirLightView, &lightPos, &(lightPos + lightDir), &D3DXVECTOR3(0, 1, 0));
		//
		//D3DXMATRIXA16 matDirLightProj;	//방향성 광원의 투영 행렬
		//D3DXMatrixOrthoLH(&matDirLightProj, 1000.0f, 1000.0f, 0.1f, 1000.0f * 2.0f);
		////D3DXMatrixPerspectiveFovLH(&matDirLightProj, D3DX_PI / 4.0f, 1, 1, 1000);
		//
		//D3DXMATRIXA16 matDirLightViewProj = matDirLightView * matDirLightProj;

		if (m_pCreateShadowShader)
		{
			g_pD3DDevice->SetVertexDeclaration(m_pDecl);

			m_pCreateShadowShader->SetTechnique("CreateShadowShader");
			m_pCreateShadowShader->SetMatrix("Mat_World", &worldMat);
			
			mCurrentTime = *curTime;

			FbxPose * lPose = NULL;
			if (mPoseIndex != -1)
			{
				lPose = pFbxScene->GetPose(mPoseIndex);
			}

			FbxAMatrix lDummyGlobalPosition;

			for (size_t i = 0; i < m_Groups.size(); ++i)
			{

				if (m_Groups[i]->pVB == NULL)		continue;

				//스킨 타입 설정
				m_pCreateShadowShader->SetInt("Model_Type", m_Groups[i]->skinData.type);

				RenderGroup(m_Groups[i], mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, m_pCreateShadowShader);
			}

			
		}
	}

	void cFbxData::Render(FbxTime* curTime, D3DXMATRIXA16& worldMat, std::map<std::string, FBX_MTL_TEX*>* pMeterialMap)
	{
		//----------------진호
		//방향성 광원
		//D3DXVECTOR3 lightPos = D3DXVECTOR3(100, 100, -100);	//빛의 위치
		//D3DXVECTOR3 lightDir = D3DXVECTOR3(-1, -1, 1);	//빛의 방향
		//
		//D3DXMATRIXA16 matDirLightView;	//방향성 광원의 뷰 행렬
		//D3DXMatrixLookAtLH(&matDirLightView, &lightPos, &(lightPos + lightDir), &D3DXVECTOR3(0, 1, 0));
		//
		//D3DXMATRIXA16 matDirLightProj;	//방향성 광원의 투영 행렬
		//D3DXMatrixOrthoLH(&matDirLightProj, 1000.0f, 1000.0f, 0.1f, 1000.0f * 2.0f);
		//
		//D3DXMATRIXA16 matDirLightViewProj = matDirLightView * matDirLightProj;
		//
		//m_ToonEffect->SetMatrix("Mat_LightViewProj", &matDirLightViewProj);
		//
		//LPDIRECT3DTEXTURE9 shadowMap = ENGINE::GetShadowMap();
		//
		//m_ToonEffect->SetTexture("ShadowMap_Tex", shadowMap);
		//----------------------

		// 빛의 위치
		//D3DXVECTOR4				gWorldLightPosition(100.0f, 100.0f, -100.0f, 1.0f);

		// 빛의 색상
		//D3DXVECTOR4				gLightColor(1.0f, 1.0f, 1.0f, 1.0f);


		mCurrentTime = *curTime;

		FbxPose * lPose = NULL;
		if (mPoseIndex != -1)
		{
			lPose = pFbxScene->GetPose(mPoseIndex);
		}

		FbxAMatrix lDummyGlobalPosition;

		for (size_t i = 0; i < m_Groups.size(); ++i){

			if (m_Groups[i]->pVB == NULL)		continue;

			UpdateGroup(m_Groups[i], mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose);
		}

		g_pD3DDevice->SetVertexDeclaration(m_pDecl);


		D3DXMATRIXA16 mView, mProj, WorldViewProj;
	
		mView = g_pCameraManager->GetCurrentCamera()->GetMatView();
		mProj = g_pCameraManager->GetCurrentCamera()->GetMatProj();
		//g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		//g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mProj);

		WorldViewProj = worldMat * mView * mProj;

		
		m_ToonEffect->SetTechnique(ToonTechHandle);

		m_ToonEffect->SetMatrix("gWorldMatrix", &worldMat);
		m_ToonEffect->SetMatrix(WorldViewProjHandle, &WorldViewProj);

		//m_ToonEffect->SetVector("gWorldLightPosition", &gWorldLightPosition);
		//m_ToonEffect->SetVector("gWorldCameraPosition", &gWorldCameraPosition);
		//m_ToonEffect->SetVector("gLightColor", &gLightColor);

		//============================서영 추가 - Fog
		//D3DXMATRIXA16 mWorldView = worldMat* mView;
		//m_ToonEffect->SetMatrix("matWorldView", &mWorldView);
		//D3DXVECTOR4 fFog = D3DXVECTOR4(2500.0f, 900.0f, 0.0f, 0.0f);
		//m_ToonEffect->SetVector("fFog", &fFog);
		//g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(80, 80, 80));
		//g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		//g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		//============================서영 추가 - Fog

		for (size_t i = 0; i < m_Groups.size(); ++i){

			if (m_Groups[i]->pVB == NULL)		continue;

			//스킨 타입 설정
			m_ToonEffect->SetInt(Model_Type, m_Groups[i]->skinData.type);

			//텍스쳐 설정
			LPDIRECT3DTEXTURE9	pDiffuseTex = NULL;
			LPDIRECT3DTEXTURE9	pNormalMapTex = NULL;
			LPDIRECT3DTEXTURE9	pSpecularMapTex = NULL;
			D3DXVECTOR4 diffuse;
			std::map<std::string, FBX_MTL_TEX*>::iterator iter;
			for (iter = pMeterialMap->begin(); iter != pMeterialMap->end(); iter++)
			{
				if (strcmp(iter->first.c_str(), m_Groups[i]->pNode->GetName()) == 0){
					pDiffuseTex = iter->second->DiffuseTex;
					pNormalMapTex = iter->second->NormalMapTex;
					pSpecularMapTex = iter->second->SpecularMapTex;

					diffuse.x = iter->second->stMtl.Diffuse.r;
					diffuse.y = iter->second->stMtl.Diffuse.g;
					diffuse.z = iter->second->stMtl.Diffuse.b;
					diffuse.w = iter->second->stMtl.Diffuse.a;
					break;
				}
			}

			m_ToonEffect->SetTexture(DiffuseTexHandle, pDiffuseTex);
			m_ToonEffect->SetTexture(NormalMapTexHandle, pNormalMapTex);
			m_ToonEffect->SetTexture(SpecularMapTexHandle, pSpecularMapTex);

			m_ToonEffect->SetVector(ColorHandle, &diffuse);

			/*for (int j = 0; j < m_MtlTexMap.size(); j++){
				if (strcmp(m_MtlTexMap[j]->path, m_Groups[i]->sMtlName) == 0){
					m_ToonEffect->SetTexture(DiffuseTexHandle, m_MtlTexMap[i]->DiffuseTex);
					break;
				}
			}*/

			//g_pD3DDevice->SetMaterial(&m_Groups[i]->stMtl);

			RenderGroup(m_Groups[i], mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, m_ToonEffect);
		}
		//============================서영 추가 - Fog
		//g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	}

	void cFbxData::Destroy()	//버텍스 버퍼 릴리즈(staticmeshManager에서만 호출)
	{
		for (int j = 0; j < m_Groups.size(); j++){
			SAFE_DELETE(m_Groups[j]);
		}
		for (int i = 0; i < m_MtlTexMap.size(); i++){
			SAFE_RELEASE(m_MtlTexMap[i]->DiffuseTex);
			SAFE_DELETE(m_MtlTexMap[i]);
		}
		m_MtlTexMap.clear();

		for (int i = 0; i < mSubMeshes.Size(); i++){
			SAFE_DELETE(mSubMeshes[i]);
		}
		mSubMeshes.Clear();
		//-----------------------------

		FbxArrayDelete(mAnimStackNameArray);

		if (pFbxScene)
		{
			pFbxScene->Destroy(true);
			//UnloadCacheRecursive(pFbxScene);
		}

		SAFE_RELEASE(ShadeTex);
	//	SAFE_RELEASE(m_ToonEffect);
		SAFE_RELEASE(m_pDecl);
	}

	void cFbxData::Load(FbxManager* pFbxManager, char* szFolder, const char* szFilename)
	{
		//HRESULT hr = 0;
		//ID3DXBuffer* errorBuffer = 0;
		//hr = D3DXCreateEffectFromFile(
		//	g_pD3DDevice,           // associated device
		//	L"tooneffect.txt", // effect filename
		//	0,                // no preprocessor definitions
		//	0,                // no ID3DXInclude interface
		//	D3DXSHADER_DEBUG, // compile flags
		//	0,                // don't share parameters
		//	&m_ToonEffect,      // return effect
		//	&errorBuffer);    // return error messages
		//if (errorBuffer)
		//{
		//	std::string error = (char*)errorBuffer->GetBufferPointer();
		//	std::wstring wError = L"";
		//	wError.assign(error.begin(), error.end());
		//	::MessageBox(0, wError.c_str(), 0, 0);
		//	SAFE_RELEASE(errorBuffer);
		//}
		//if (FAILED(hr))
		//{
		//
		//	::MessageBox(0, L"D3DXCreateEffectFromFile() - FAILED", 0, 0);
		//}

		m_ToonEffect = g_pShaderManager->GetShader("Shader/", "tooneffect.txt");

		ToonTechHandle = m_ToonEffect->GetTechniqueByName("Toon");
		DiffuseTexHandle = m_ToonEffect->GetParameterByName(0, "DiffuseTex");
		NormalMapTexHandle = m_ToonEffect->GetParameterByName(0, "NormalMapTex");
		SpecularMapTexHandle = m_ToonEffect->GetParameterByName(0, "SpecularMapTex");

		WorldViewProjHandle = m_ToonEffect->GetParameterByName(0, "WorldViewProjMatrix");
		ColorHandle = m_ToonEffect->GetParameterByName(0, "Color");
		ControlMatrixArray = m_ToonEffect->GetParameterByName(0, "ControlMatrixArray");
		Model_Type = m_ToonEffect->GetParameterByName(0, "Model_Type");

		//---------------------------------------------------------------------------------------------------------------------------------

		m_pCreateShadowShader = g_pShaderManager->GetShader("Shader/", "CreateShadow.fx");
		m_pApplyShadowShader = g_pShaderManager->GetShader("Shader/", "ApplyShadow.fx");
		  
		std::string filepath(szFolder);
		filepath += std::string(szFilename);

		mCache_Start = FBXSDK_TIME_INFINITE;
		mCache_Stop = FBXSDK_TIME_MINUS_INFINITE;

		FbxImporter* pImporter = FbxImporter::Create(pFbxManager, "");
		pFbxScene = FbxScene::Create(pFbxManager, "");

		bool bSuccess = pImporter->Initialize(filepath.c_str(), -1, pFbxManager->GetIOSettings());
		if (!bSuccess) return;

		bSuccess = pImporter->Import(pFbxScene);
		if (!bSuccess) return;

		// Convert Axis System to what is used in this example, if needed
		FbxAxisSystem SceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if (SceneAxisSystem != OurAxisSystem)
		{
			OurAxisSystem.ConvertScene(pFbxScene);
		}

		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit SceneSystemUnit = pFbxScene->GetGlobalSettings().GetSystemUnit();
		if (SceneSystemUnit.GetScaleFactor() != 1.0)
		{
			//The unit in this example is centimeter.
			FbxSystemUnit::cm.ConvertScene(pFbxScene);
		}

		// Get the list of all the animation stack.
		pFbxScene->FillAnimStackNameArray(mAnimStackNameArray);

		// Get the list of all the cameras in the scene.
		//카메라 노드 찾아서 벡터에 저장???
		//FillCameraArray(pFbxScene, mCameraArray);

		// Convert mesh, NURBS and patch into triangle mesh
		// 곡선으로 매쉬 정보를 바꾸어주나???
		FbxGeometryConverter lGeomConverter(pFbxManager);
		lGeomConverter.Triangulate(pFbxScene, /*replace*/true);

		// Get the list of pose in the scene
		FillPoseArray(pFbxScene, mPoseArray);

		// Bake the scene for one frame
		LoadCacheRecursive(pFbxScene, mCurrentAnimLayer, filepath.c_str());

		// Convert any .PC2 point cache data into the .MC format for 
		// vertex cache deformer playback.
		// 무슨 기능을 하는 함수인교???
		//PreparePointCacheData(pFbxScene, mCache_Start, mCache_Stop);

		

		// Initialize the frame period.
		mFrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());

		pImporter->Destroy();
		pImporter = NULL;

		//SetCurrentAnimStack(0);
	}

	//---------------------------------------------------------------로드 함수들
	// Find all the cameras in this scene.
	void cFbxData::FillCameraArray(FbxScene* pScene, FbxArray<FbxNode*>& pCameraArray)
	{
		pCameraArray.Clear();

		FillCameraArrayRecursive(pScene->GetRootNode(), pCameraArray);
	}
	void cFbxData::FillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray)
	{
		if (pNode)
		{
			if (pNode->GetNodeAttribute())
			{
				if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eCamera)
				{
					pCameraArray.Add(pNode);
				}
			}

			const int lCount = pNode->GetChildCount();
			for (int i = 0; i < lCount; i++)
			{
				FillCameraArrayRecursive(pNode->GetChild(i), pCameraArray);
			}
		}
	}
	// Find all poses in this scene.
	void cFbxData::FillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray)
	{
		const int lPoseCount = pScene->GetPoseCount();

		for (int i = 0; i < lPoseCount; ++i)
		{
			pPoseArray.Add(pScene->GetPose(i));
		}
	}
	void cFbxData::LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName)
	{
		const int lTextureCount = pScene->GetTextureCount();
		char path[512];
		wchar_t path2[512];

		//툴에서 따로 이미지를 지정하여 fbxComponent에 추가하기에 사용하지 않는다
		/*
		//가지고 있는 텍스쳐 종류별로 데이터 로드 한다
		for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
		{
			FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
			FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
			
			if (lFileTexture && !lFileTexture->GetUserDataPtr())
			{
				//저장 된 텍스쳐 경로가 맞지 않으면 로드한 fbx파일 위치의 디렉토리를 찾는다
				const FbxString lFileName = lFileTexture->GetFileName();
				const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName);
				const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);


				bool lStatus = false; //이미지가 로드 되었는지 체크

				FBX_MTL_TEX* mtl = new FBX_MTL_TEX();

				if (!lStatus)
				{
					memset(path, 0, 512);
					strcpy(path, lFileName.Buffer());
					CharToWChar(path, path2);
					if (D3DXCreateTextureFromFile(g_pD3DDevice, path2, &mtl->DiffuseTex) == S_OK){
						//if (D3DXCreateTextureFromFile(g_pD3DDevice, , &mtl->DiffuseTex) == S_OK){
						lStatus = true;
						getName(lFileTexture->GetRelativeFileName(), mtl->path);

						m_MtlTexMap.push_back(mtl);
					}
				}
				if (!lStatus)
				{
					// Load texture from relative file name (relative to FBX file)
					const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
					memset(path, 0, 512);
					strcpy(path, lResolvedFileName.Buffer());
					CharToWChar(path, path2);

					if (D3DXCreateTextureFromFile(g_pD3DDevice, path2, &mtl->DiffuseTex) == S_OK){
						lStatus = true;
						getName(lFileTexture->GetRelativeFileName(), mtl->path);
						m_MtlTexMap.push_back(mtl);
					}
				}
				if (!lStatus)
				{
					// Load texture from file name only (relative to FBX file)
					const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
					const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);

					memset(path, 0, 512);
					strcpy(path, lResolvedFileName.Buffer());
					CharToWChar(path, path2);

					if (D3DXCreateTextureFromFile(g_pD3DDevice, path2, &mtl->DiffuseTex) == S_OK){
						lStatus = true;
						getName(lFileTexture->GetRelativeFileName(), mtl->path);

						m_MtlTexMap.push_back(mtl);
					}
				}

				//이미지 로드 실패일경우 동적할당 해제
				if (!lStatus)
				{
					delete mtl;
					continue;
				}
			}
		}
		*/

		LoadCacheRecursive(pScene->GetRootNode(), pAnimLayer);
	}
	void cFbxData::LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer)
	{
		// Bake material and hook as user data.
		const int lMaterialCount = pNode->GetMaterialCount();

		//해당 노드 머테리얼값이 있을 경우 머테리얼 값과 매쉬 정보를 저장한다
		if (lMaterialCount > 0){
			//머테리얼 정보와 매쉬 정보를 저장하는 데이터 클래스 생성
			FBX_DATA_GROUP* pGroup = new FBX_DATA_GROUP();
			//노트 포인터 저장
			pGroup->pNode = pNode;

			//머테리얼 정보 저장
			//머테리얼이 여러개 일때 어떻게 적용 해야 할까????
			for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
			{
				FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);

				strcpy(pGroup->sMtlName, lMaterial->GetName());
				GetMaterialValue(lMaterial, &pGroup->stMtl);
			}

			FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
			if (lNodeAttribute)
			{
				// Bake mesh as VBO(vertex buffer object) into GPU.
				if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
				{
					CreateDataGroup(pGroup, pNode->GetMesh());
				}

				// Bake light properties.
				else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
				{
					/*FbxLight * lLight = pNode->GetLight();
					if (lLight && !lLight->GetUserDataPtr())
					{
					FbxAutoPtr<LightCache> lLightCache(new LightCache);
					if (lLightCache->Initialize(lLight, pAnimLayer))
					{
					lLight->SetUserDataPtr(lLightCache.Release());
					}
					}*/
				}
			}
			m_Groups.push_back(pGroup);
		}

		//자식 노드 호출
		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			LoadCacheRecursive(pNode->GetChild(lChildIndex), pAnimLayer);
		}
	}
	void cFbxData::PreparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop)
	{
		// This function show how to cycle through scene elements in a linear way.
		const int lNodeCount = pScene->GetSrcObjectCount<FbxNode>();
		FbxStatus lStatus;

		for (int lIndex = 0; lIndex<lNodeCount; lIndex++)
		{
			FbxNode* lNode = pScene->GetSrcObject<FbxNode>(lIndex);

			if (lNode->GetGeometry())
			{
				int i, lVertexCacheDeformerCount = lNode->GetGeometry()->GetDeformerCount(FbxDeformer::eVertexCache);

				// There should be a maximum of 1 Vertex Cache Deformer for the moment
				lVertexCacheDeformerCount = lVertexCacheDeformerCount > 0 ? 1 : 0;

				for (i = 0; i<lVertexCacheDeformerCount; ++i)
				{
					// Get the Point Cache object
					FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(lNode->GetGeometry()->GetDeformer(i, FbxDeformer::eVertexCache));
					if (!lDeformer) continue;
					FbxCache* lCache = lDeformer->GetCache();
					if (!lCache) continue;

					// Process the point cache data only if the constraint is active
					if (lDeformer->Active.Get())
					{
						if (lCache->GetCacheFileFormat() == FbxCache::eMaxPointCacheV2)
						{
							// This code show how to convert from PC2 to MC point cache format
							// turn it on if you need it.
#if 0 
							if (!lCache->ConvertFromPC2ToMC(FbxCache::eMCOneFile,
								FbxTime::GetFrameRate(pScene->GetGlobalTimeSettings().GetTimeMode())))
							{
								// Conversion failed, retrieve the error here
								FbxString lTheErrorIs = lCache->GetStaus().GetErrorString();
							}
#endif
						}
						else if (lCache->GetCacheFileFormat() == FbxCache::eMayaCache)
						{
							// This code show how to convert from MC to PC2 point cache format
							// turn it on if you need it.
							//#if 0 
							if (!lCache->ConvertFromMCToPC2(FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode()), 0, &lStatus))
							{
								// Conversion failed, retrieve the error here
								FbxString lTheErrorIs = lStatus.GetErrorString();
							}
							//#endif
						}


						// Now open the cache file to read from it
						if (!lCache->OpenFileForRead(&lStatus))
						{
							// Cannot open file 
							FbxString lTheErrorIs = lStatus.GetErrorString();

							// Set the deformer inactive so we don't play it back
							lDeformer->Active = false;
						}
						else
						{
							// get the start and stop time of the cache
							FbxTime lChannel_Start;
							FbxTime lChannel_Stop;
							int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
							if (lCache->GetAnimationRange(lChannelIndex, lChannel_Start, lChannel_Stop))
							{
								// get the smallest start time
								if (lChannel_Start < pCache_Start) pCache_Start = lChannel_Start;

								// get the biggest stop time
								if (lChannel_Stop  > pCache_Stop)  pCache_Stop = lChannel_Stop;
							}
						}
					}
				}
			}
		}
	}

	//PNT값 저장
	bool cFbxData::CreateDataGroup(FBX_DATA_GROUP* pGroup, const FbxMesh *pMesh)
	{
		std::vector<FBX_MESH_DATA_PNTi>	Vertexes2;
		Vertexes2.clear();

		if (!pMesh->GetNode())
			return false;

		const int lPolygonCount = pMesh->GetPolygonCount();

		// Count the polygon count of each material
		FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
		FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;

		if (pMesh->GetElementMaterial())
		{
			lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
			lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
			
			if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
			{
				FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
				if (lMaterialIndice->GetCount() == lPolygonCount)
				{
					// Count the faces of each material
					for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
					{
						const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
						if (mSubMeshes.GetCount() < lMaterialIndex + 1)
						{
							mSubMeshes.Resize(lMaterialIndex + 1);
						}
						if (mSubMeshes[lMaterialIndex] == NULL)
						{
							mSubMeshes[lMaterialIndex] = new SubMesh;
						}
						mSubMeshes[lMaterialIndex]->TriangleCount += 1;
					}

					// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
					// if, in the loop above, we resized the mSubMeshes by more than one slot.
					for (int i = 0; i < mSubMeshes.GetCount(); i++)
					{
						if (mSubMeshes[i] == NULL)
							mSubMeshes[i] = new SubMesh;
					}

					// Record the offset (how many vertex)
					const int lMaterialCount = mSubMeshes.GetCount();
					int lOffset = 0;
					for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
					{
						mSubMeshes[lIndex]->IndexOffset = lOffset;
						lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
						// This will be used as counter in the following procedures, reset to zero
						mSubMeshes[lIndex]->TriangleCount = 0;
					}
					FBX_ASSERT(lOffset == lPolygonCount * 3);
				}
			}
		}

		// All faces will use the same material.
		if (mSubMeshes.GetCount() == 0)
		{
			mSubMeshes.Resize(1);
			mSubMeshes[0] = new SubMesh();
		}

		// Congregate all the data of a mesh to be cached in VBOs.
		// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
		mHasNormal = pMesh->GetElementNormalCount() > 0;
		mHasUV = pMesh->GetElementUVCount() > 0;
		FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
		FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
		if (mHasNormal)
		{
			lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
			if (lNormalMappingMode == FbxGeometryElement::eNone)
			{
				mHasNormal = false;
			}
			if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
			{
				mAllByControlPoint = false;
			}
		}
		if (mHasUV)
		{
			lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
			if (lUVMappingMode == FbxGeometryElement::eNone)
			{
				mHasUV = false;
			}
			if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
			{
				mAllByControlPoint = false;
			}
		}
		int lPolygonVertexCount = pMesh->GetControlPointsCount();
		if (!mAllByControlPoint)
		{
			lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
		}
		float * lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
		//unsigned int * lIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_COUNT];
		float * lNormals = NULL;
		if (mHasNormal)
		{
			lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
		}
		float * lUVs = NULL;
		FbxStringList lUVNames;
		pMesh->GetUVSetNames(lUVNames);
		const char * lUVName = NULL;
		if (mHasUV && lUVNames.GetCount())
		{
			lUVs = new float[lPolygonVertexCount * UV_STRIDE];
			lUVName = lUVNames[0];
		}

		// Populate the array with vertex attribute, if by control point.

		const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
		FbxVector4 lCurrentVertex;
		FbxVector4 lCurrentNormal;
		FbxVector2 lCurrentUV;
		if (mAllByControlPoint)
		{
			const FbxGeometryElementNormal * lNormalElement = NULL;
			const FbxGeometryElementUV * lUVElement = NULL;
			if (mHasNormal)
			{
				lNormalElement = pMesh->GetElementNormal(0);
			}
			if (mHasUV)
			{
				lUVElement = pMesh->GetElementUV(0);
			}
			for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
			{
				// Save the vertex position.
				lCurrentVertex = lControlPoints[lIndex];
				lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
				lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
				lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
				lVertices[lIndex * VERTEX_STRIDE + 3] = 1;

				// Save the normal.
				if (mHasNormal)
				{
					int lNormalIndex = lIndex;
					if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
					}
					lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
					lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
					lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
				}

				// Save the UV.
				if (mHasUV)
				{
					int lUVIndex = lIndex;
					if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
					}
					lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
					lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
					lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
				}

			}
		}

		int lVertexCount = 0;
		pGroup->nNumTri = lPolygonCount;
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
		{
			// The material for current face.
			int lMaterialIndex = 0;
			if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
			{
				lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
			}

			// Where should I save the vertex attribute index, according to the material
			const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset +
				mSubMeshes[lMaterialIndex]->TriangleCount * 3;

			//한 폴리곤의 정점,노말,uv값을 저장하는 변수 
			TempVertexType vertexType[3];
			//폴리곤 pnt로 구한 tangent벡터
			D3DXVECTOR3 TangentVector;
			//폴리곤 pnt로 구한 Binormal벡터
			D3DXVECTOR3 BinormalVector;

			FBX_MESH_DATA_PNTi v[3];
			for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
			{
				//FBX_MESH_DATA_PNTi v;
				const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

				if (mAllByControlPoint)
				{
				}
				else
				{
					//v.index = lControlPointIndex;
					for (int i = 0; i < 4; i++){
						v[lVerticeIndex].index[i] = 0.0f;
						v[lVerticeIndex].weight[i] = 0.0f;
					}

					lCurrentVertex = lControlPoints[lControlPointIndex];
					lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
					lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
					lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
					lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;

					v[lVerticeIndex].p.x = lVertices[lVertexCount * VERTEX_STRIDE];
					v[lVerticeIndex].p.y = lVertices[lVertexCount * VERTEX_STRIDE + 1];
					v[lVerticeIndex].p.z = lVertices[lVertexCount * VERTEX_STRIDE + 2];
					v[lVerticeIndex].p.w = 1.0f;

					vertexType[lVerticeIndex].p = (D3DXVECTOR3)v[lVerticeIndex].p;

					if (mHasNormal)
					{
						pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
						lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
						lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
						lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);

						v[lVerticeIndex].n.x = lNormals[lVertexCount * NORMAL_STRIDE];
						v[lVerticeIndex].n.y = lNormals[lVertexCount * NORMAL_STRIDE + 1];
						v[lVerticeIndex].n.z = lNormals[lVertexCount * NORMAL_STRIDE + 2];

						vertexType[lVerticeIndex].n = v[lVerticeIndex].n;
					}

					if (mHasUV)
					{
						bool lUnmappedUV;
						pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
						lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
						lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

						v[lVerticeIndex].t.x = lUVs[lVertexCount * UV_STRIDE];
						v[lVerticeIndex].t.y = 1 - lUVs[lVertexCount * UV_STRIDE + 1];

						vertexType[lVerticeIndex].t = v[lVerticeIndex].t;
					}
				}

				++lVertexCount;
			}

			//tangent, binormal을 구한다
			CalculateModelVectors(vertexType[0], vertexType[1], vertexType[2], &TangentVector, &BinormalVector);

			//모델 정보에 구한 tangent, binormal을 넣어준다
			for(int i = 0; i < TRIANGLE_VERTEX_COUNT; i++)
			{
				v[i].tangent = TangentVector;
				v[i].biNormal = BinormalVector;
				//최종적으로 정점데이터를 벡터에 넣어준다 
				Vertexes2.push_back(v[i]);
			}

			mSubMeshes[lMaterialIndex]->TriangleCount += 1;
		}
		if (Vertexes2.size() > 0){
			g_pD3DDevice->CreateVertexBuffer(Vertexes2.size() * sizeof(FBX_MESH_DATA_PNTi), 0, FBX_MESH_DATA_PNTi::FVF, D3DPOOL_MANAGED, &pGroup->pVB, 0);

			// 삼각형 개수 기록
			pGroup->nNumTri = Vertexes2.size() / 3;

			void* pVertex;
			pGroup->pVB->Lock(0, 0, &pVertex, 0);
			memcpy(pVertex, &Vertexes2[0], Vertexes2.size() * sizeof(FBX_MESH_DATA_PNTi));
			pGroup->pVB->Unlock();

			g_pD3DDevice->CreateVertexBuffer(Vertexes2.size() * sizeof(FBX_MESH_DATA_PNTi), 0, FBX_MESH_DATA_PNTi::FVF, D3DPOOL_MANAGED, &pGroup->bufVB, 0);
			pGroup->bufVB->Lock(0, 0, &pVertex, 0);
			memcpy(pVertex, &Vertexes2[0], Vertexes2.size() * sizeof(FBX_MESH_DATA_PNTi));
			pGroup->bufVB->Unlock();

			Vertexes2.clear();
		}
		delete[] lVertices;

		if (mHasNormal)
		{
			delete[] lNormals;
		}

		if (mHasUV)
		{
			delete[] lUVs;
		}

		InitVertexAndIndex(pGroup);

		return true;
	}
	void cFbxData::InitVertexAndIndex(FBX_DATA_GROUP* pGroup)
	{
		FbxMesh* lMesh = pGroup->pNode->GetMesh();

		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

		if (lHasDeformation){
			if (lHasVertexCache)
			{
				//예외처리 안 되어 있음 들어오면 주옥됨
			}
			else{
				if (lHasShape)
				{
					//예외처리 안 되어 있음 들어오면 주옥됨
				}

				FbxSkin * lSkinDeformer = (FbxSkin *)lMesh->GetDeformer(0, FbxDeformer::eSkin);
				FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

				if (lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid)
				{
					// All the links must have the same link mode.
					FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)lMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

					int lVertexCount = lMesh->GetControlPointsCount();

					//정점 갯수만큼 생성 
					FBX_SKIN_WEIGHT* weightData = new FBX_SKIN_WEIGHT[lVertexCount];
					for (int i = 0; i < lVertexCount; i++){
						for (int j = 0; j < 4; j++){
							weightData[i].weight[j] = 0.0f;
							weightData[i].index[j] = 0;
						}
						weightData[i].weightNum = 0;
					}

					if (lClusterMode == FbxCluster::eAdditive)			pGroup->skinData.type = 0;
					else if (lClusterMode == FbxCluster::eNormalize)	pGroup->skinData.type = 1;
					else if (lClusterMode == FbxCluster::eTotalOne)		pGroup->skinData.type = 2;
					else												pGroup->skinData.type = 1;//다른거 일경우 걍 노말로 하자

					int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
					int matCount = 0;
					for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
					{
						FbxSkin * lSkinDeformer = (FbxSkin *)lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
						
						int lClusterCount = lSkinDeformer->GetClusterCount();
						for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
						{
							FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
							if (!lCluster->GetLink())
								continue;

							int lVertexIndexCount = lCluster->GetControlPointIndicesCount();

							for (int k = 0; k < lVertexIndexCount; ++k)
							{
								int lIndex = lCluster->GetControlPointIndices()[k];

								if (lIndex >= lVertexCount)
									continue;

								double lWeight = lCluster->GetControlPointWeights()[k];

								if (lWeight == 0.0)
								{
									continue;
								}

								//weight값은 4개까지만 저장한다
								if (weightData[lIndex].weightNum >= 4) 
									continue;
								//무게값 저장 현재 무게값에 맞는 매트릭스 인덱스 저장 그리고 웨이트 카운트를 +1 해준다
								weightData[lIndex].weight [weightData[lIndex].weightNum ] = lWeight;
								weightData[lIndex].index[ weightData[lIndex].weightNum++ ] = matCount;

							}//For each vertex
							matCount++;
						}//lClusterCount
					}

					//이 그룹의 매트릭스 갯수
					pGroup->skinData.matSize = matCount;
					pGroup->skinData.pMat = new D3DXMATRIX[pGroup->skinData.matSize];

					if (mAllByControlPoint)
					{
						//예외처리 안 되어 있음 들어오면 주옥됨
					}
					else{
						FBX_MESH_DATA_PNTi * bufVB = NULL;
						int meshVectorCount = pGroup->nNumTri * TRIANGLE_VERTEX_COUNT;
						bufVB = new FBX_MESH_DATA_PNTi[meshVectorCount];

						//원본 매쉬 pnt 정보 복사
						void* pVertex;
						pGroup->pVB->Lock(0, 0, &pVertex, 0);
						memcpy(bufVB, pVertex, meshVectorCount * sizeof(FBX_MESH_DATA_PNTi));

						int lPolygonCount = pGroup->pNode->GetMesh()->GetPolygonCount();
						int vertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;

						int vertxCount = 0;
						for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
						{
							for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
							{
								const int lControlPointIndex = pGroup->pNode->GetMesh()->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
								
								//위에서넣은 weight값을 데이터에 넣어준다
								for (int i = 0; i < 4; i++){
									bufVB[vertxCount].weight[i] = weightData[lControlPointIndex].weight[i];
									bufVB[vertxCount].index[i] = (float)weightData[lControlPointIndex].index[i];
								}

								++vertxCount;
							}
						}
					
						memcpy(pVertex, bufVB, meshVectorCount * sizeof(FBX_MESH_DATA_PNTi));
						pGroup->pVB->Unlock();
					}
					//동적 할당 해제
					delete[] weightData;
				}
				else if (lSkinningType == FbxSkin::eDualQuaternion)
				{
					//예외처리 안 되어 있음 들어오면 주옥됨
				}
				else if (lSkinningType == FbxSkin::eBlend)
				{
					//예외처리 안 되어 있음 들어오면 주옥됨
				}
			}
		}
		else{

			// 스킨이 없음으로 -1을 해서 스킨 애니메이션을 계산 하지 않고 그리기만 한다
			pGroup->skinData.type = -1;
		}
	}
	//메테리얼 값 저장 
	void cFbxData::GetMaterialValue(const FbxSurfaceMaterial* pMaterial, D3DMATERIAL9* mtl)
	{
		const FbxDouble3 lEmissive = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
		mtl->Emissive.r = static_cast<GLfloat>(lEmissive[0]);
		mtl->Emissive.g = static_cast<GLfloat>(lEmissive[1]);
		mtl->Emissive.b = static_cast<GLfloat>(lEmissive[2]);

		const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
		mtl->Ambient.r = static_cast<GLfloat>(lAmbient[0]);
		mtl->Ambient.g = static_cast<GLfloat>(lAmbient[1]);
		mtl->Ambient.b = static_cast<GLfloat>(lAmbient[2]);

		const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
		mtl->Diffuse.r = static_cast<GLfloat>(lDiffuse[0]);
		mtl->Diffuse.g = static_cast<GLfloat>(lDiffuse[1]);
		mtl->Diffuse.b = static_cast<GLfloat>(lDiffuse[2]);

		const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
		mtl->Specular.r = static_cast<GLfloat>(lSpecular[0]);
		mtl->Specular.g = static_cast<GLfloat>(lSpecular[1]);
		mtl->Specular.b = static_cast<GLfloat>(lSpecular[2]);
	}
	FbxDouble3 cFbxData::GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName)
	{
		FbxDouble3 lResult(0, 0, 0);
		const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
		const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);

		if (lProperty.IsValid() && lFactorProperty.IsValid())
		{
			lResult = lProperty.Get<FbxDouble3>();
			double lFactor = lFactorProperty.Get<FbxDouble>();
			if (lFactor != 1)
			{
				lResult[0] *= lFactor;
				lResult[1] *= lFactor;
				lResult[2] *= lFactor;
			}
		}

		return lResult;
	}
	//---------------------------------------------------------------업데이트 함수들
	void cFbxData::UpdateGroup(FBX_DATA_GROUP* pGroup, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose)
	{
		FbxMesh* lMesh = pGroup->pNode->GetMesh();
		const int lVertexCount = lMesh->GetControlPointsCount();

		// No vertex to draw.
		if (lVertexCount == 0)		return;

		// If it has some defomer connection, update the vertices position
		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;


		if (lHasDeformation)
		{
			// Active vertex cache deformer will overwrite any other deformer
			if (lHasVertexCache)
			{
				ReadVertexCacheData(lMesh, pTime);
			}
			else
			{
				if (lHasShape)
				{
					// Deform the vertex array with the shapes.
					ComputeShapeDeformation(lMesh, pTime, pAnimLayer);
				}

				//we need to get the number of clusters
				const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
				int lClusterCount = 0;
				for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
				{
					lClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
				}
				if (lClusterCount)
				{
					// Deform the vertex array with the skin deformer.
					ComputeSkinDeformation(pGroup, pGlobalPosition, lMesh, pTime, pPose);
				}
			}
		}
	}
	void cFbxData::UpdateBipMatrixMap(std::map<std::string, D3DXMATRIXA16>* vpBipMap, D3DXMATRIXA16& worldMat)
	{
		FbxPose * lPose = NULL;
		if (mPoseIndex != -1)
		{
			
			lPose = pFbxScene->GetPose(mPoseIndex);
		}

		UpdateBipNodeSearch(pFbxScene->GetRootNode(), vpBipMap, lPose, worldMat);
	}
	void cFbxData::UpdateBipNodeSearch(FbxNode * pNode, std::map<std::string, D3DXMATRIXA16>* vpBipMap, FbxPose* pPose, D3DXMATRIXA16& worldMat)
	{
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
		if (lNodeAttribute && lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			FbxAMatrix fbxMat;
			D3DXMATRIXA16 dirMat;
			fbxMat.GetT();
			fbxMat.GetR();
			fbxMat.GetS();
			fbxMat = GetGlobalPosition(pNode, mCurrentTime, pPose);
		
			ConvertMatrixD3dToFbx(&dirMat, &fbxMat);
			(*vpBipMap)[pNode->GetName()] = dirMat * worldMat;
		}

		//자식 노드 호출
		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			UpdateBipNodeSearch(pNode->GetChild(lChildIndex), vpBipMap, pPose, worldMat);
		}
	}
	//---------------------------------------------------------------렌더 함수들
	void cFbxData::RenderGroup(FBX_DATA_GROUP* pGroup, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, LPD3DXEFFECT effect)
	{
		D3DXHANDLE constrolMatrixA = effect->GetParameterByName(0, "ControlMatrixArray");
		effect->SetMatrixArray(constrolMatrixA, pGroup->skinData.pMat, pGroup->skinData.matSize);
		//effect->SetMatrixArray("ControlMatrixArray", pGroup->skinData.pMat, pGroup->skinData.matSize);
		
		UINT uiPasses, uiPass;

		effect->Begin(&uiPasses, 0);
		effect->BeginPass(0);

		g_pD3DDevice->SetStreamSource(0, pGroup->pVB, 0, sizeof(FBX_MESH_DATA_PNTi));
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, pGroup->nNumTri);

		effect->EndPass();
		effect->End();
	}
	//vertexData update And Draw PNT in Group
	void cFbxData::UpdateVertexPosition(FBX_DATA_GROUP* pGroup, FbxVector4 * pVertices)
	{
		// Convert to the same sequence with data in GPU.
		float * lVertices = NULL;
		int lVertexCount = 0;

		if (mAllByControlPoint)
		{
			lVertexCount = pGroup->pNode->GetMesh()->GetControlPointsCount();
			lVertices = new float[lVertexCount * VERTEX_STRIDE];
			for (int lIndex = 0; lIndex < lVertexCount; ++lIndex)
			{
				lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(pVertices[lIndex][0]);
				lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(pVertices[lIndex][1]);
				lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(pVertices[lIndex][2]);
				lVertices[lIndex * VERTEX_STRIDE + 3] = 1;
			}
		}
		else
		{
			const int lPolygonCount = pGroup->pNode->GetMesh()->GetPolygonCount();
			lVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
			lVertices = new float[lVertexCount * VERTEX_STRIDE];

			int vertxCount = 0;
			for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
			{
				for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
				{
					const int lControlPointIndex = pGroup->pNode->GetMesh()->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
					lVertices[vertxCount * VERTEX_STRIDE] = static_cast<float>(pVertices[lControlPointIndex][0]);
					lVertices[vertxCount * VERTEX_STRIDE + 1] = static_cast<float>(pVertices[lControlPointIndex][1]);
					lVertices[vertxCount * VERTEX_STRIDE + 2] = static_cast<float>(pVertices[lControlPointIndex][2]);
					lVertices[vertxCount * VERTEX_STRIDE + 3] = 1;
					++vertxCount;
				}
			}
		}
		
		// Transfer into GPU.
		if (lVertices)
		{
			//바뀐거 임시 정하는 메모리 할당
			FBX_MESH_DATA_PNTi * bufVB = NULL;
			bufVB = new FBX_MESH_DATA_PNTi[lVertexCount];

			//원본 매쉬 pnt 정보 복사
			void* pVertex;
			pGroup->pVB->Lock(0, 0, &pVertex, 0);
			memcpy(bufVB, pVertex, lVertexCount * sizeof(FBX_MESH_DATA_PNTi));

			//바뀐 버텍스 정보 옮기기
			for (int i = 0; i < lVertexCount; i++){
				bufVB[i].p.x = lVertices[i * VERTEX_STRIDE];
				bufVB[i].p.y = lVertices[i * VERTEX_STRIDE + 1];
				bufVB[i].p.z = lVertices[i * VERTEX_STRIDE + 2];
			}

			memcpy(pVertex, bufVB, lVertexCount * sizeof(FBX_MESH_DATA_PNTi));
			pGroup->pVB->Unlock();

			delete[] bufVB;
			delete[] lVertices;
		}
	}
	// Draw the Vertex CacheData
	void cFbxData::ReadVertexCacheData(FbxMesh* pMesh, FbxTime& pTime)
	{
		FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(pMesh->GetDeformer(0, FbxDeformer::eVertexCache));
		FbxCache*               lCache = lDeformer->GetCache();
		int                     lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
		unsigned int            lVertexCount = (unsigned int)pMesh->GetControlPointsCount();
		bool                    lReadSucceed = false;
		float*                  lReadBuf = NULL;
		unsigned int			BufferSize = 0;

		if (lDeformer->Type.Get() != FbxVertexCacheDeformer::ePositions)
			// only process positions
			return;

		unsigned int Length = 0;
		lCache->Read(NULL, Length, FBXSDK_TIME_ZERO, lChannelIndex);
		if (Length != lVertexCount * 3)
			// the content of the cache is by vertex not by control points (we don't support it here)
			return;

		lReadSucceed = lCache->Read(&lReadBuf, BufferSize, pTime, lChannelIndex);
		if (lReadSucceed)
		{
			unsigned int lReadBufIndex = 0;

			while (lReadBufIndex < 3 * lVertexCount)
			{
				// In statements like "pVertexArray[lReadBufIndex/3].SetAt(2, lReadBuf[lReadBufIndex++])", 
				// on Mac platform, "lReadBufIndex++" is evaluated before "lReadBufIndex/3". 
				// So separate them.
				/*pVertexArray[lReadBufIndex / 3].mData[0] = lReadBuf[lReadBufIndex]; lReadBufIndex++;
				pVertexArray[lReadBufIndex / 3].mData[1] = lReadBuf[lReadBufIndex]; lReadBufIndex++;
				pVertexArray[lReadBufIndex / 3].mData[2] = lReadBuf[lReadBufIndex]; lReadBufIndex++;*/
			}
		}
	}
	// Deform the vertex array with the shapes contained in the mesh.
	void cFbxData::ComputeShapeDeformation(FbxMesh* pMesh, FbxTime& pTime, FbxAnimLayer * pAnimLayer)
	{
		/*
		int lVertexCount = pMesh->GetControlPointsCount();

		FbxVector4* lSrcVertexArray = pVertexArray;
		FbxVector4* lDstVertexArray = new FbxVector4[lVertexCount];
		memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));

		int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
		for (int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
		{
			FbxBlendShape* lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

			int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
			for (int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
			{
				FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
				if (lChannel)
				{
					// Get the percentage of influence on this channel.
					FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
					if (!lFCurve) continue;
					double lWeight = lFCurve->Evaluate(pTime);


					// Find the two shape indices for influence calculation according to the weight.
					// Consider index of base geometry as -1.

					int lShapeCount = lChannel->GetTargetShapeCount();
					double* lFullWeights = lChannel->GetTargetShapeFullWeights();

					// Find out which scope the lWeight falls in.
					int lStartIndex = -1;
					int lEndIndex = -1;
					for (int lShapeIndex = 0; lShapeIndex<lShapeCount; ++lShapeIndex)
					{
						if (lWeight > 0 && lWeight <= lFullWeights[0])
						{
							lEndIndex = 0;
							break;
						}
						if (lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex + 1])
						{
							lStartIndex = lShapeIndex;
							lEndIndex = lShapeIndex + 1;
							break;
						}
					}

					FbxShape* lStartShape = NULL;
					FbxShape* lEndShape = NULL;
					if (lStartIndex > -1)
					{
						lStartShape = lChannel->GetTargetShape(lStartIndex);
					}
					if (lEndIndex > -1)
					{
						lEndShape = lChannel->GetTargetShape(lEndIndex);
					}

					//The weight percentage falls between base geometry and the first target shape.
					if (lStartIndex == -1 && lEndShape)
					{
						double lEndWeight = lFullWeights[0];
						// Calculate the real weight.
						lWeight = (lWeight / lEndWeight) * 100;
						// Initialize the lDstVertexArray with vertex of base geometry.
						memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
						for (int j = 0; j < lVertexCount; j++)
						{
							// Add the influence of the shape vertex to the mesh vertex.
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight * 0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
					//The weight percentage falls between two target shapes.
					else if (lStartShape && lEndShape)
					{
						double lStartWeight = lFullWeights[lStartIndex];
						double lEndWeight = lFullWeights[lEndIndex];
						// Calculate the real weight.
						lWeight = ((lWeight - lStartWeight) / (lEndWeight - lStartWeight)) * 100;
						// Initialize the lDstVertexArray with vertex of the previous target shape geometry.
						memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
						for (int j = 0; j < lVertexCount; j++)
						{
							// Add the influence of the shape vertex to the previous shape vertex.
							FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j]) * lWeight * 0.01;
							lDstVertexArray[j] += lInfluence;
						}
					}
				}//If lChannel is valid
			}//For each blend shape channel
		}//For each blend shape deformer

		memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));

		delete[] lDstVertexArray;
		*/
	}
	// Deform the vertex array according to the links contained in the mesh and the skinning type.
	void cFbxData::ComputeSkinDeformation(FBX_DATA_GROUP* pGroup, FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxPose* pPose)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(0, FbxDeformer::eSkin);
		FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

		if (lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid)
		{
			ComputeLinearDeformation(pGroup, pGlobalPosition, pMesh, pTime, pPose);
		}
		else if (lSkinningType == FbxSkin::eDualQuaternion)
		{
			ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, pPose);
		}
		else if (lSkinningType == FbxSkin::eBlend)
		{
			/*
			int lVertexCount = pMesh->GetControlPointsCount();

			FbxVector4* lVertexArrayLinear = new FbxVector4[lVertexCount];
			memcpy(lVertexArrayLinear, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

			FbxVector4* lVertexArrayDQ = new FbxVector4[lVertexCount];
			memcpy(lVertexArrayDQ, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

			ComputeLinearDeformation(pGroup, pGlobalPosition, pMesh, pTime, lVertexArrayLinear, pPose);
			ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);

			int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
			for (int lBWIndex = 0; lBWIndex<lBlendWeightsCount; ++lBWIndex)
			{
				double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
				pVertexArray[lBWIndex] = lVertexArrayDQ[lBWIndex] * lBlendWeight + lVertexArrayLinear[lBWIndex] * (1 - lBlendWeight);
			}
			*/
		}
	}
	// Deform the vertex array in classic linear way.
	void cFbxData::ComputeLinearDeformation(FBX_DATA_GROUP* pGroup, FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxPose* pPose)
	{
		// All the links must have the same link mode.
		FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

		if (lClusterMode == FbxCluster::eAdditive)			pGroup->skinData.type = 0;
		else if (lClusterMode == FbxCluster::eNormalize)	pGroup->skinData.type = 1;
		else if (lClusterMode == FbxCluster::eTotalOne)		pGroup->skinData.type = 2;
		else												pGroup->skinData.type = 1;//다른거 일경우 걍 노말로 하자

		for (int i = 0; i < pGroup->skinData.matSize; i++){
			if (lClusterMode == FbxCluster::eAdditive)
				D3DXMatrixIdentity(&pGroup->skinData.pMat[i]);
			else{
				for (int j = 0; j < 4; j++){
					for (int k = 0; k < 4; k++){
						pGroup->skinData.pMat[i].m[j][k] = 0.0;
					}
				}
			}
		}

		// For all skins and all clusters, accumulate their deformation and weight
		// on each vertices and store them in lClusterDeformation and lClusterWeight.
		int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
		int matCount = 0;
		for (int lSkinIndex = 0; lSkinIndex<lSkinCount; ++lSkinIndex)
		{
			FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
			
			int lClusterCount = lSkinDeformer->GetClusterCount();
			for (int lClusterIndex = 0; lClusterIndex<lClusterCount; ++lClusterIndex)
			{
				FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);

				if (!lCluster->GetLink())
					continue;
				
				FbxAMatrix lVertexTransformMatrix;
				lVertexTransformMatrix.SetIdentity();
				ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);
				
				//매트릭스 저장
				ConvertMatrixD3dToFbx(&(pGroup->skinData.pMat[matCount++]), &lVertexTransformMatrix);
			}
		}
	}
	// Deform the vertex array in Dual Quaternion Skinning way.
	void cFbxData::ComputeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxPose* pPose)
	{
		/*
		// All the links must have the same link mode.
		FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

		int lVertexCount = pMesh->GetControlPointsCount();
		int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

		FbxDualQuaternion* lDQClusterDeformation = new FbxDualQuaternion[lVertexCount];
		memset(lDQClusterDeformation, 0, lVertexCount * sizeof(FbxDualQuaternion));

		double* lClusterWeight = new double[lVertexCount];
		memset(lClusterWeight, 0, lVertexCount * sizeof(double));

		// For all skins and all clusters, accumulate their deformation and weight
		// on each vertices and store them in lClusterDeformation and lClusterWeight.
		for (int lSkinIndex = 0; lSkinIndex<lSkinCount; ++lSkinIndex)
		{
			FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
			int lClusterCount = lSkinDeformer->GetClusterCount();
			for (int lClusterIndex = 0; lClusterIndex<lClusterCount; ++lClusterIndex)
			{
				FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
				if (!lCluster->GetLink())
					continue;

				FbxAMatrix lVertexTransformMatrix;
				ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

				FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
				FbxVector4 lT = lVertexTransformMatrix.GetT();
				FbxDualQuaternion lDualQuaternion(lQ, lT);

				int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
				for (int k = 0; k < lVertexIndexCount; ++k)
				{
					int lIndex = lCluster->GetControlPointIndices()[k];

					// Sometimes, the mesh can have less points than at the time of the skinning
					// because a smooth operator was active when skinning but has been deactivated during export.
					if (lIndex >= lVertexCount)
						continue;

					double lWeight = lCluster->GetControlPointWeights()[k];

					if (lWeight == 0.0)
						continue;

					// Compute the influence of the link on the vertex.
					FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
					if (lClusterMode == FbxCluster::eAdditive)
					{
						// Simply influenced by the dual quaternion.
						lDQClusterDeformation[lIndex] = lInfluence;

						// Set the link to 1.0 just to know this vertex is influenced by a link.
						lClusterWeight[lIndex] = 1.0;
					}
					else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
					{
						if (lClusterIndex == 0)
						{
							lDQClusterDeformation[lIndex] = lInfluence;
						}
						else
						{
							// Add to the sum of the deformations on the vertex.
							// Make sure the deformation is accumulated in the same rotation direction. 
							// Use dot product to judge the sign.
							double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
							if (lSign >= 0.0)
							{
								lDQClusterDeformation[lIndex] += lInfluence;
							}
							else
							{
								lDQClusterDeformation[lIndex] -= lInfluence;
							}
						}
						// Add to the sum of weights to either normalize or complete the vertex.
						lClusterWeight[lIndex] += lWeight;
					}
				}//For each vertex
			}//lClusterCount
		}

		//Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
		for (int i = 0; i < lVertexCount; i++)
		{
			FbxVector4 lSrcVertex = pVertexArray[i];
			FbxVector4& lDstVertex = pVertexArray[i];
			double lWeightSum = lClusterWeight[i];

			// Deform the vertex if there was at least a link with an influence on the vertex,
			if (lWeightSum != 0.0)
			{
				lDQClusterDeformation[i].Normalize();
				lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

				if (lClusterMode == FbxCluster::eNormalize)
				{
					// In the normalized link mode, a vertex is always totally influenced by the links. 
					lDstVertex /= lWeightSum;
				}
				else if (lClusterMode == FbxCluster::eTotalOne)
				{
					// In the total 1 link mode, a vertex can be partially influenced by the links. 
					lSrcVertex *= (1.0 - lWeightSum);
					lDstVertex += lSrcVertex;
				}
			}
		}

		delete[] lDQClusterDeformation;
		delete[] lClusterWeight;
		*/
	}
	//Compute the transform matrix that the cluster will transform the vertex.
	void cFbxData::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose)
	{
		FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

		FbxAMatrix lReferenceGlobalInitPosition;
		FbxAMatrix lReferenceGlobalCurrentPosition;
		FbxAMatrix lAssociateGlobalInitPosition;
		FbxAMatrix lAssociateGlobalCurrentPosition;
		FbxAMatrix lClusterGlobalInitPosition;
		FbxAMatrix lClusterGlobalCurrentPosition;

		FbxAMatrix lReferenceGeometry;
		FbxAMatrix lAssociateGeometry;
		FbxAMatrix lClusterGeometry;

		FbxAMatrix lClusterRelativeInitPosition;
		FbxAMatrix lClusterRelativeCurrentPositionInverse;

		if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
		{
			pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
			// Geometric transform of the model
			lAssociateGeometry = GetGeometry(pCluster->GetAssociateModel());
			lAssociateGlobalInitPosition *= lAssociateGeometry;
			lAssociateGlobalCurrentPosition = GetGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

			pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation
			lReferenceGeometry = GetGeometry(pMesh->GetNode());
			lReferenceGlobalInitPosition *= lReferenceGeometry;
			lReferenceGlobalCurrentPosition = pGlobalPosition;

			// Get the link initial global position and the link current global position.
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			// Multiply lClusterGlobalInitPosition by Geometric Transformation
			lClusterGeometry = GetGeometry(pCluster->GetLink());
			lClusterGlobalInitPosition *= lClusterGeometry;
			lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

			// Compute the shift of the link relative to the reference.
			//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
			pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
				lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
		}
		else
		{
			pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
			lReferenceGlobalCurrentPosition = pGlobalPosition;
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation
			lReferenceGeometry = GetGeometry(pMesh->GetNode());
			lReferenceGlobalInitPosition *= lReferenceGeometry;

			// Get the link initial global position and the link current global position.
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

			// Compute the initial position of the link relative to the reference.
			lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

			// Compute the current position of the link relative to the reference.
			lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

			// Compute the shift of the link relative to the reference.
			pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
		}
	}
	// Scale all the elements of a matrix.
	void cFbxData::MatrixScale(FbxAMatrix& pMatrix, double pValue)
	{
		int i, j;

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				pMatrix[i][j] *= pValue;
			}
		}
	}
	// Add a value to all the elements in the diagonal of the matrix.
	void cFbxData::MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
	{
		pMatrix[0][0] += pValue;
		pMatrix[1][1] += pValue;
		pMatrix[2][2] += pValue;
		pMatrix[3][3] += pValue;
	}
	// Sum two matrices element by element.
	void cFbxData::MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
	{
		int i, j;

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				pDstMatrix[i][j] += pSrcMatrix[i][j];
			}
		}
	}

	//---------------------------------------------------------------기능 함수들
	bool cFbxData::SetCurrentAnimStack(int pIndex)
	{
		const int lAnimStackCount = mAnimStackNameArray.GetCount();
		if (!lAnimStackCount || pIndex >= lAnimStackCount)
		{
			return false;
		}

		// select the base layer from the animation stack
		FbxAnimStack * lCurrentAnimationStack = pFbxScene->FindMember<FbxAnimStack>(mAnimStackNameArray[pIndex]->Buffer());
		if (lCurrentAnimationStack == NULL)
		{
			// this is a problem. The anim stack should be found in the scene!
			return false;
		}

		// we assume that the first animation layer connected to the animation stack is the base layer
		// (this is the assumption made in the FBXSDK)
		mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
		pFbxScene->SetCurrentAnimationStack(lCurrentAnimationStack);
		/*
		FbxTakeInfo* lCurrentTakeInfo = pFbxScene->GetTakeInfo(*(mAnimStackNameArray[pIndex]));
		if (lCurrentTakeInfo)
		{
		mStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		mStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
		}
		else
		{
		// Take the time line value
		FbxTimeSpan lTimeLineTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

		mStart = lTimeLineTimeSpan.GetStart();
		mStop = lTimeLineTimeSpan.GetStop();
		}

		// check for smallest start with cache start
		if (mCache_Start < mStart)
		mStart = mCache_Start;

		// check for biggest stop with cache stop
		if (mCache_Stop  > mStop)
		mStop = mCache_Stop;

		// move to beginning
		mCurrentTime = mStart;
		*/
		mCurrentTime = 0;
		return true;
	}
	void cFbxData::getName(const char* filePath, char* fileName)
	{
		int end = strlen(filePath) - 1;
		int start = 0;

		//이름 시작위치 알아내기
		for (int i = end; i >= 0; i--){
			if (filePath[i] == 92){
				start = i + 1;
				break;
			}
		}
		//이름 저장
		for (int i = start; i <= end; i++){
			if (filePath[i] == '.') break;

			fileName[i - start] = filePath[i];
		}
	}
	FbxAMatrix cFbxData::GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition)
	{
		FbxAMatrix lGlobalPosition;
		bool        lPositionFound = false;

		if (pPose)
		{
			int lNodeIndex = pPose->Find(pNode);
		
			if (lNodeIndex > -1)
			{
				// The bind pose is always a global matrix.
				// If we have a rest pose, we need to check if it is
				// stored in global or local space.
				if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
				{
					lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
				}
				else
				{
					// We have a local matrix, we need to convert it to
					// a global space matrix.
					FbxAMatrix lParentGlobalPosition;

					if (pParentGlobalPosition)
					{
						lParentGlobalPosition = *pParentGlobalPosition;
					}
					else
					{
						if (pNode->GetParent())
						{
							lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
						}
					}

					FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
					lGlobalPosition = lParentGlobalPosition * lLocalPosition;
				}

				lPositionFound = true;
			}
		}

		if (!lPositionFound)
		{
			// There is no pose entry for that node, get the current global position instead.

			// Ideally this would use parent global position and local position to compute the global position.
			// Unfortunately the equation 
			//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
			// does not hold when inheritance type is other than "Parent" (RSrs).
			// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
			lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
		}

		return lGlobalPosition;
	}
	// Get the matrix of the given pose
	FbxAMatrix cFbxData::GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
	{
		FbxAMatrix lPoseMatrix;
		FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

		memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

		return lPoseMatrix;
	}
	// Get the geometry offset to a node. It is never inherited by the children.
	FbxAMatrix cFbxData::GetGeometry(FbxNode* pNode)
	{
		const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(lT, lR, lS);
	}
	void cFbxData::ConvertMatrixD3dToFbx(D3DXMATRIX* d3dMat, FbxAMatrix* fbxMat)
	{
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++)
				d3dMat->m[i][j] = fbxMat->Get(i, j);
		}
	}
	void cFbxData::SetCollisionSphere(OUT D3DXVECTOR3* _vMin, OUT D3DXVECTOR3* _vMax)
	{
		D3DXVECTOR3 vMin, vMax;
		ST_BONE* pBone;
		bool isInitCollisionBox = false;

		//모든 메쉬를 확인하고 충돌박스 크기를 구한다.
		vMin = D3DXVECTOR3(0, 0, 0);
		vMax = D3DXVECTOR3(0, 0, 0);

		GetMinMaxVertex(pFbxScene->GetRootNode(), &vMin, &vMax, &isInitCollisionBox);

		*_vMin = vMin;
		*_vMax = vMax;
	}
	void cFbxData::GetMinMaxVertex(FbxNode* pNode, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, bool* pIsInitCollisionBox)
	{
		//매쉬 일경우에만 버텍스 검사
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
		if (lNodeAttribute && lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = pNode->GetMesh();
			int lVertexCount = lMesh->GetControlPointsCount();

			// No vertex to draw.
			if (lVertexCount == 0)		return;

			// If it has some defomer connection, update the vertices position
			/*const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
			const bool lHasShape = lMesh->GetShapeCount() > 0;
			const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
			const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;
			if (lHasDeformation)
			{
			}*/
			FbxVector4* lVertexArray = NULL;


			lVertexArray = new FbxVector4[lVertexCount];
			memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

			// Convert to the same sequence with data in GPU.D3DXVECTOR3 p;
			D3DXVECTOR3 temp;
			if (mAllByControlPoint)
			{
				lVertexCount = lMesh->GetControlPointsCount();
				for (int lIndex = 0; lIndex < lVertexCount; ++lIndex)
				{
					//y와 z값을 바꾸는 이유는 맥스와 directx 좌표계가 다르기 떄문임
					temp.x = static_cast<float>(lVertexArray[lIndex][0]);
					temp.z = static_cast<float>(lVertexArray[lIndex][1]);
					temp.y = static_cast<float>(lVertexArray[lIndex][2]);

					if (*pIsInitCollisionBox == false){
						*pIsInitCollisionBox = true;
						*pMin = temp;
						*pMax = temp;
					}
					else{
						if (pMax->x < temp.x) pMax->x = temp.x;
						if (pMax->y < temp.y) pMax->y = temp.y;
						if (pMax->z < temp.z) pMax->z = temp.z;

						if (pMin->x > temp.x) pMin->x = temp.x;
						if (pMin->y > temp.y) pMin->y = temp.y;
						if (pMin->z > temp.z) pMin->z = temp.z;
					}
				}
			}
			else
			{
				const int lPolygonCount = pNode->GetMesh()->GetPolygonCount();
				lVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;

				int vertxCount = 0;
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
				{
					for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
					{
						const int lControlPointIndex = pNode->GetMesh()->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

						temp.x = static_cast<float>(lVertexArray[lControlPointIndex][0]);
						temp.z = static_cast<float>(lVertexArray[lControlPointIndex][1]);
						temp.y = static_cast<float>(lVertexArray[lControlPointIndex][2]);

						if (*pIsInitCollisionBox == false){
							*pIsInitCollisionBox = true;
							*pMin = temp;
							*pMax = temp;
						}
						else{
							if (pMax->x < temp.x) pMax->x = temp.x;
							if (pMax->y < temp.y) pMax->y = temp.y;
							if (pMax->z < temp.z) pMax->z = temp.z;

							if (pMin->x > temp.x) pMin->x = temp.x;
							if (pMin->y > temp.y) pMin->y = temp.y;
							if (pMin->z > temp.z) pMin->z = temp.z;
						}
						++vertxCount;
					}
				}
			}

			if (lVertexArray)	delete[] lVertexArray;
		}
		//자식 노드 호출
		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
		{
			GetMinMaxVertex(pNode->GetChild(lChildIndex), pMin, pMax, pIsInitCollisionBox);
		}
	}
	void cFbxData::StartAnim(int nIndex, int nStart, int nEnd, FbxTime* pStart, FbxTime* pEnd, FbxTime* pCurrent)
	{
		const int lAnimStackCount = mAnimStackNameArray.GetCount();

		if (!lAnimStackCount || nIndex >= lAnimStackCount)		return;

		FbxAnimStack * lCurrentAnimationStack = pFbxScene->FindMember<FbxAnimStack>(mAnimStackNameArray[nIndex]->Buffer());
		if (lCurrentAnimationStack == NULL)
		{
			return;
		}

		mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
		pFbxScene->SetCurrentAnimationStack(lCurrentAnimationStack);

		FbxTakeInfo* lCurrentTakeInfo = pFbxScene->GetTakeInfo(*(mAnimStackNameArray[nIndex]));
		if (lCurrentTakeInfo)
		{
			*pStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
			*pEnd = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
		}
		else
		{
			// Take the time line value
			FbxTimeSpan lTimeLineTimeSpan;
			pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

			*pStart = lTimeLineTimeSpan.GetStart();
			*pEnd = lTimeLineTimeSpan.GetStop();
		}

		FbxTime start, end;
		start.SetFrame(nStart);

		end.SetFrame(nEnd);

		if (start >= *pStart)	*pStart = start;

		if (end <= *pEnd)		*pEnd = end;

		*pCurrent = *pStart;
	}
	D3DXVECTOR3 cFbxData::GetMatVectorMulT(D3DXMATRIXA16* d3dMat, D3DXVECTOR3* d3dVet)
	{
		FbxAMatrix fbxMat;
		FbxVector4 fbxVec;
		D3DXVECTOR3 resultVec;

		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++)
				fbxMat.mData[i][j] = d3dMat->m[i][j];
		}

		fbxVec.mData[0] = d3dVet->x;
		fbxVec.mData[1] = d3dVet->y;
		fbxVec.mData[2] = d3dVet->z;
		fbxVec.mData[3] = 0.0f;

		fbxVec = fbxMat.MultT(fbxVec);

		resultVec.x = fbxVec.mData[0];
		resultVec.y = fbxVec.mData[1];
		resultVec.z = fbxVec.mData[2];

		return resultVec;
	}
	bool cFbxData::IsZeroMatrix(D3DXMATRIXA16* d3dMat)
	{
		//모든 값이 제로 이면 true
		//아니면 false 리턴
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				if (d3dMat->m[i][j] != 0.0)
					return false;
			}

		}
		return true;
	}
	D3DXVECTOR4 cFbxData::MulMatrixVector(D3DXMATRIX* pMat, D3DXVECTOR4* pVec)
	{
		D3DXVECTOR4 result = D3DXVECTOR4(0, 0, 0, 0);

		result.x = (pMat->m[0][0] * pVec->x) + (pMat->m[1][0] * pVec->y) + (pMat->m[2][0] * pVec->z) + pMat->m[3][0];
		result.y = (pMat->m[0][1] * pVec->x) + (pMat->m[1][1] * pVec->y) + (pMat->m[2][1] * pVec->z) + pMat->m[3][1];
		result.z = (pMat->m[0][2] * pVec->x) + (pMat->m[1][2] * pVec->y) + (pMat->m[2][2] * pVec->z) + pMat->m[3][2];
		result.w = 1.0f;

		return result;
	}
}
