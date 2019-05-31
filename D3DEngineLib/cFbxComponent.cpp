#include "Common.h"
#include "include\D3DRenderer.h"
#include "cObjParser.h"
#include "cFbxComponent.h"

namespace ENGINE
{
	cFbxComponent::cFbxComponent()
	{
		eComponent_Type = eFBX_MESH_TYPE;
		m_pOwnerObject = NULL;
		m_matWorld = NULL;
		m_pFbxData = NULL;
		m_IsPlay = true;
		m_pCurrentClip = NULL;
		mStart = 0;
		mStop = 0;
		mCurrentTime = 0;
		m_AccrueDeltaTime = 0.0;
	}

	cFbxComponent::cFbxComponent(GameObject* pOwnerObject)
	{
		eComponent_Type = eFBX_MESH_TYPE;
		m_pOwnerObject = pOwnerObject;
		m_matWorld = NULL;
		m_pFbxData = NULL;
		m_IsPlay = true;
		m_pCurrentClip = NULL;
		mStart = 0;
		mStop = 0;
		mCurrentTime = 0;
		m_AccrueDeltaTime = 0.0;
	}

	cFbxComponent::cFbxComponent(GameObject* pOwnerObject, char* szFolder, const char* szFilename)
	{
		eComponent_Type = eFBX_MESH_TYPE;
		m_pOwnerObject = pOwnerObject;
		m_matWorld = NULL;
		m_IsPlay = true;
		m_pCurrentClip = NULL;
		mStart = 0;
		mStop = 0;
		mCurrentTime = 0;
		m_AccrueDeltaTime = 0.0;

		m_pFbxData = g_pFbxManager->GetFbxData(szFolder, szFilename);
		m_pFbxData->SetCollisionSphere(&m_vMin, &m_vMax);
		//m_pFbxData->StartAnim(0, 85, 105, &mStart, &mStop, &mCurrentTime);
		m_pFbxData->UpdateBipMatrixMap(&m_vBipMatrixMap, m_pOwnerObject->GetWorldMatrix());

		//로드된 파일 경로 저장
		memset(m_szFolder, 0, 256);
		memset(m_szFilename, 0, 256);
		strcpy(m_szFolder, szFolder);
		strcpy(m_szFilename, szFilename);
	}

	cFbxComponent::~cFbxComponent()
	{
	}

	void cFbxComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		D3DXVECTOR3 vecMin, vecMax;
		D3DXVec3TransformCoord(&vecMin, &m_vMin, matWorld);
		D3DXVec3TransformCoord(&vecMax, &m_vMax, matWorld);
		D3DXVECTOR3 center = (vecMin + vecMax) / 2.0f;
		float radius = D3DXVec3Length(&(vecMax - center));
		g_pShadowManager->AllocGameObjectToShadowMap(center, radius, m_pOwnerObject);

		//m_pOwnerObject
	/*	if(m_pOwnerObject->GetParent() == GetSceneRoot())
		{
			GameObject* pRootNode = GetSceneRoot();
			for each (auto iter in pRootNode->GetVecChild())
			{
				cTerrainComponent* terain =  iter->GetComponent<cTerrainComponent*>(eTERRAIN_TYPE);
				terain->GetHeightmapPos(vPos); //bool 좌표 변경되면 true , 아니면 true
			}
		}*/
		m_matWorld = matWorld;

		if (m_pFbxData){
			//애니 시간 업데이트
			if (mCurrentTime >= mStop && m_IsPlay){
				mCurrentTime = mStart;
			}
			else if (mCurrentTime >= mStop && !m_IsPlay){
				mCurrentTime = mStop;
			}

			FbxTime fbxTime;
			fbxTime.SetTime(0, 0, 0, 1, 0);

			m_AccrueDeltaTime += deltaTime;
			float aniFrame = 1.0 / 30.0;

			while (1){
				if (m_AccrueDeltaTime >= aniFrame){
					mCurrentTime += fbxTime;
					m_AccrueDeltaTime -= aniFrame;
				}
				else
					break;
			}

			//m_pFbxData->Update(&mCurrentTime);
			//m_pCurrentClip의 index받아와서 사용
			//mCurrentTime.GetFrameCount();

			//바이패드 매트릭스 업데이트
			//m_pFbxData->UpdateBipMatrixMap(&m_vBipMatrixMap, m_pOwnerObject->GetWorldMatrix());
		}
	}

	void cFbxComponent::PrevRender()
	{
		if (m_pFbxData) m_pFbxData->PrevRender(&mCurrentTime, *m_matWorld);
	}

	void cFbxComponent::Render()
	{
		if (m_pFbxData) m_pFbxData->Render(&mCurrentTime, *m_matWorld, &m_vMeterialTextureMap);
		m_pFbxData->UpdateBipMatrixMap(&m_vBipMatrixMap, m_pOwnerObject->GetWorldMatrix());
	}

	void cFbxComponent::Destroy()
	{
		m_vBipMatrixMap.clear();

		for each(auto p in m_vMeterialTextureMap)
		{
			delete p.second;
		}
		m_vMeterialTextureMap.clear();
	}
	void cFbxComponent::StartAnim(ST_AnimClip* clip)
	{
		m_pCurrentClip = clip;
		m_IsPlay = m_pCurrentClip->isLoop;
		m_pFbxData->StartAnim(m_pCurrentClip->setIndex, m_pCurrentClip->start, m_pCurrentClip->end, &mStart, &mStop, &mCurrentTime);
	}
	D3DXMATRIXA16* cFbxComponent::GetMatrixFromeFrame(const char* frameName)
	{
		if (m_vBipMatrixMap.find(frameName) == m_vBipMatrixMap.end()){
			return NULL;
		}
		else{
			//m_vBipMatrixMap[frameName] = m_vBipMatrixMap[frameName] * m_pOwnerObject->GetWorldMatrix();
			return &m_vBipMatrixMap[frameName];
		}
	}

	bool cFbxComponent::IntersectRayMesh(D3DXVECTOR3 origin, D3DXVECTOR3 dir)
	{
		// orgin과 dir을 로컬좌표 기준으로 바꿔준다.
		D3DXMATRIXA16 worldInverse;
		D3DXMatrixInverse(&worldInverse, 0, m_matWorld);

		D3DXVECTOR3 localOrigin, localDir;
		D3DXVec3TransformCoord(&localOrigin, &origin, &worldInverse);
		D3DXVec3TransformNormal(&localDir, &dir, &worldInverse);
		D3DXVec3Normalize(&localDir, &localDir);

		FLOAT fDist = FLT_MAX;
		FLOAT fU, fV;
		FBX_MESH_DATA_PNTi* pVertices;

		for (size_t i = 0; i < m_pFbxData->m_Groups.size(); i++)
		{
			m_pFbxData->m_Groups[i]->pVB->Lock(0, 0, (void**)&pVertices, 0);

			for (size_t j = 0; j < m_pFbxData->m_Groups[i]->nNumTri; j++)
			{
				D3DXVECTOR3 v0 = pVertices[3 * j + 0].p;
				D3DXVECTOR3 v1 = pVertices[3 * j + 1].p;
				D3DXVECTOR3 v2 = pVertices[3 * j + 2].p;

				if (IntersectTriangle(localOrigin, localDir, v0, v1, v2, &fDist, &fU, &fV))
				{
					m_pFbxData->m_Groups[i]->pVB->Unlock();
					return true;
				}
			}
			m_pFbxData->m_Groups[i]->pVB->Unlock();
		}
		return false;
	}

	bool cFbxComponent::IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
		D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
	{
		// Find vectors for two edges sharing vert0
		D3DXVECTOR3 edge1 = v1 - v0;
		D3DXVECTOR3 edge2 = v2 - v0;

		// Begin calculating determinant - also used to calculate U parameter
		D3DXVECTOR3 pvec;
		D3DXVec3Cross(&pvec, &dir, &edge2);

		// If determinant is near zero, ray lies in plane of triangle
		FLOAT det = D3DXVec3Dot(&edge1, &pvec);

		D3DXVECTOR3 tvec;
		if (det > 0)
		{
			tvec = orig - v0;
		}
		else
		{
			tvec = v0 - orig;
			det = -det;
		}

		if (det < 0.0001f)
			return FALSE;

		// Calculate U parameter and test bounds
		*u = D3DXVec3Dot(&tvec, &pvec);
		if (*u < 0.0f || *u > det)
			return FALSE;

		// Prepare to test V parameter
		D3DXVECTOR3 qvec;
		D3DXVec3Cross(&qvec, &tvec, &edge1);

		// Calculate V parameter and test bounds
		*v = D3DXVec3Dot(&dir, &qvec);
		if (*v < 0.0f || *u + *v > det)
			return FALSE;

		// Calculate t, scale parameters, ray intersects triangle
		*t = D3DXVec3Dot(&edge2, &qvec);
		FLOAT fInvDet = 1.0f / det;
		*t *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;

		return TRUE;
	}
	void cFbxComponent::SetMeterialTexture(char* name, char* path, float r, float g, float b, float a)
	{
		bool isCreate = true;
		FBX_MTL_TEX* pFbxMtl;

		//추가 할려는 재질에 이미 데이터가 들어갔는지 여부 확인
		// 있으면 추가로 생성 안하고 데이터만 바꾸어 준다
		for each(auto p in m_vMeterialTextureMap)
		{
			if (strcmp(p.first.c_str(), name) == 0 )
			{
				isCreate = false;
				pFbxMtl = p.second;
				break;
			}
		}

		// diffuse텍스쳐 경로 -> 다른 텍스쳐 경로 저장
		char normalMapPath[512];
		char specularMapPath[512];
		memset(normalMapPath, 0, 512);
		memset(specularMapPath, 0, 512);

		SetTexturePath(normalMapPath, path, "_NRM");
		SetTexturePath(specularMapPath, path, "_SPEC");

		//char -> wchar_t 형변환
		wchar_t wchDiffusePath[512];
		wchar_t wchNormalMapPath[512];
		wchar_t wchSpecularMapPath[512];

		int nLen = (int)strlen(path) + 1;
		mbstowcs(wchDiffusePath, path, nLen);

		nLen = (int)strlen(normalMapPath) + 1;
		mbstowcs(wchNormalMapPath, normalMapPath, nLen);

		nLen = (int)strlen(specularMapPath) + 1;
		mbstowcs(wchSpecularMapPath, specularMapPath, nLen);

		if (isCreate == false){
			pFbxMtl->stMtl.Diffuse.r = r;
			pFbxMtl->stMtl.Diffuse.g = g;
			pFbxMtl->stMtl.Diffuse.b = b;
			pFbxMtl->stMtl.Diffuse.a = a;

			// diffuse 텍스쳐 로드
			if (pFbxMtl->DiffuseTex){
				pFbxMtl->DiffuseTex->Release();
				pFbxMtl->DiffuseTex = NULL;
			}
			if (D3DXCreateTextureFromFile(g_pD3DDevice, wchDiffusePath, &pFbxMtl->DiffuseTex) == S_OK){
				strcpy(pFbxMtl->path, path);
			}

			// normal 텍스쳐 로드
			if (pFbxMtl->NormalMapTex){
				pFbxMtl->NormalMapTex->Release();
				pFbxMtl->NormalMapTex = NULL;
			}
			if (D3DXCreateTextureFromFile(g_pD3DDevice, wchNormalMapPath, &pFbxMtl->NormalMapTex) == S_OK){
				int s = 0;
				s++;
			}

			// specular 텍스쳐 로드
			if (pFbxMtl->SpecularMapTex){
				pFbxMtl->SpecularMapTex->Release();
				pFbxMtl->SpecularMapTex = NULL;
			}
			if (D3DXCreateTextureFromFile(g_pD3DDevice, wchSpecularMapPath, &pFbxMtl->SpecularMapTex) == S_OK){
				int s = 0;
				s++;
			}
		}
		else{
			pFbxMtl = new FBX_MTL_TEX();
			
			pFbxMtl->stMtl.Diffuse.r = r;
			pFbxMtl->stMtl.Diffuse.g = g;
			pFbxMtl->stMtl.Diffuse.b = b;
			pFbxMtl->stMtl.Diffuse.a = a;

			// diffuse 텍스쳐 로드
			if (D3DXCreateTextureFromFile(g_pD3DDevice, wchDiffusePath, &pFbxMtl->DiffuseTex) == S_OK){
				strcpy(pFbxMtl->path, path);
			}
			else{
				delete pFbxMtl;
				return;
			}

			// normal 텍스쳐 로드
			D3DXCreateTextureFromFile(g_pD3DDevice, wchNormalMapPath, &pFbxMtl->NormalMapTex);
			// specular 텍스쳐 로드
			D3DXCreateTextureFromFile(g_pD3DDevice, wchSpecularMapPath, &pFbxMtl->SpecularMapTex);

			m_vMeterialTextureMap[name] = pFbxMtl;
		}
	}
	//diffuse텍스쳐 경로에서 normalMap,SpecularMap 텍스쳐 경로를 알아낸다( add - _NRM, _SPEC)
	void cFbxComponent::SetTexturePath(char* dest, char* source, char* addStr)
	{
		int destCnt = 0;
		int sourceCnt = 0;

		for (; sourceCnt < strlen(source); sourceCnt++)
		{
			//확장자 전까지 다 복사 되었으면 addStr 추가한다
			if (source[sourceCnt] == '.'){
				for (int i = 0; i < strlen(addStr); i++)
					dest[destCnt++] = addStr[i];
			}

			dest[destCnt++] = source[sourceCnt];
		}
	}
}

