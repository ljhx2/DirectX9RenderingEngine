#include "Common.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"

#include <sstream>

namespace ENGINE
{


	cSkinnedMesh::cSkinnedMesh(char* szFolder, const  char* szFilename)
		: m_pRootFrame(NULL)
		, m_pAnimController(NULL)
		, m_dwWorkingPaletteSize(0)
		, m_pmWorkingPalette(NULL)
		, m_pEffect(NULL)
		, m_vPosition(0, 0, 0)
	{
		cSkinnedMesh* pSkinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

		m_pRootFrame = pSkinnedMesh->m_pRootFrame;
		m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
		m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
		m_pEffect = pSkinnedMesh->m_pEffect;

		pSkinnedMesh->m_pAnimController->CloneAnimationController(
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
			pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
			pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
			&m_pAnimController);
	}

	cSkinnedMesh::cSkinnedMesh()
		: m_pRootFrame(NULL)
		, m_pAnimController(NULL)
		, m_dwWorkingPaletteSize(0)
		, m_pmWorkingPalette(NULL)
		, m_pEffect(NULL)
	{
	}

	cSkinnedMesh::~cSkinnedMesh(void)
	{
		SAFE_RELEASE(m_pAnimController);
	}

	void cSkinnedMesh::Load(char* szDirectory, const char* szFilename)
	{

		m_pEffect = LoadEffect("MultiAnimation.hpp");

		int nPaletteSize = 0;
		m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

		cAllocateHierarchy ah;
		ah.SetDirectory(szDirectory);
		ah.SetDefaultPaletteSize(nPaletteSize);

		std::string sFullPath(szDirectory);
		sFullPath += std::string(szFilename);

		/// std::string => std::wstring
		std::wstring wFullPath = L"";
		wFullPath.assign(sFullPath.begin(), sFullPath.end());

		D3DXLoadMeshHierarchyFromX(wFullPath.c_str(),
			D3DXMESH_MANAGED,
			g_pD3DDevice,
			&ah,
			NULL,
			(LPD3DXFRAME*)&m_pRootFrame,
			&m_pAnimController);

		if (m_pmWorkingPalette)
			delete[] m_pmWorkingPalette;

		m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
		m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
		if (m_pmWorkingPalette == NULL)
		{
			m_dwWorkingPaletteSize = 0;
		}

		if (m_pRootFrame)
			SetupBoneMatrixPtrs(m_pRootFrame);


		//바운딩 스피어 center, radius 구하기
		SetCollisionSphere(&m_vMin, &m_vMax);

	}

	void cSkinnedMesh::UpdateAndRender()
	{
		if (m_pAnimController)
		{

			////현재 트랙 정보를 얻어옴.
			D3DXTRACK_DESC  desc;
			m_pAnimController->GetTrackDesc(0, &desc);
			////현재 트랙 포지션의 위치를 기준으로 애니메이션을 변경하는 샘플 코드
			//if (desc.Position > 2.0)
			//{
			//	SetAnimationIndex(1);
			//	m_pAnimController->SetTrackPosition(0, 0);
			//}

			std::wstringstream strDebug;
			strDebug << desc.Position << std::endl;
			OutputDebugString(strDebug.str().c_str());

		}


		if (m_pAnimController)
		{
			m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
		}

		//여기서 스킨드메시에 대한 World 행렬 만들면 됨.
		//World mat =  Scaling * Rotaion * Translation 
		if (m_pRootFrame)
		{
			//1. 초기화-------------------------------------------------//
			D3DXMATRIXA16 mWorld, mScaling, mRotation, mTranslation;
			D3DXMatrixIdentity(&mWorld); //월드 변환 행렬(Scaling * Rotaion * Translation )
			D3DXMatrixIdentity(&mScaling);	//스케일링 변환 행렬	
			D3DXMatrixIdentity(&mRotation);	//회전 변환 행렬	
			D3DXMatrixIdentity(&mTranslation);//이동 변환 행렬
			//------------------------------------------------------//
			//2. 행렬 세팅---------------------------------------------//
			D3DXMatrixTranslation(&mTranslation, m_vPosition.x, m_vPosition.y, m_vPosition.z);

			/*
			static float angle = 0.0f;
			angle += 0.001f;
			if (angle > 3.14159f * 2.0f)
			angle = 0.0f;
			D3DXMatrixRotationY(&mRotation, angle);*/
			//------------------------------------------------------//

			//3. 월드 행렬 최종 처리-----------------------------------// 
			mWorld = mScaling * mRotation * mTranslation;

			Update(m_pRootFrame, &mWorld);
			Render(m_pRootFrame);
			//-------------------------------------------------------//
		}
	}

	void cSkinnedMesh::Render(ST_BONE* pBone /*= NULL*/)
	{
		assert(pBone);

		// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
		// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
		if (pBone->pMeshContainer)
		{
			ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

			// get bone combinations
			LPD3DXBONECOMBINATION pBoneCombos =
				(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

			D3DXMATRIXA16 matViewProj, matView, matProj;
			g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
			g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
			matViewProj = matView * matProj;

			D3DXMATRIXA16 mView, mInvView;
			g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixInverse(&mInvView, 0, &mView);
			D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
			D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

			// for each palette
			for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
			{
				// set each transform into the palette
				for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
				{
					DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
					if (dwMatrixIndex != UINT_MAX)
					{
						m_pmWorkingPalette[dwPalEntry] =
							pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
							(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
					}
				}

				// set the matrix palette into the effect
				m_pEffect->SetMatrixArray("amPalette",
					m_pmWorkingPalette,
					pBoneMesh->dwNumPaletteEntries);

				m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
				m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
				m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
				m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
				m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
				m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

				// we're pretty much ignoring the materials we got from the x-file; just set
				// the texture here
				m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

				// set the current number of bones; this tells the effect which shader to use
				m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

				// set the technique we use to draw
				m_pEffect->SetTechnique("Skinning20");

				UINT uiPasses, uiPass;

				// run through each pass and draw
				m_pEffect->Begin(&uiPasses, 0);
				for (uiPass = 0; uiPass < uiPasses; ++uiPass)
				{
					m_pEffect->BeginPass(uiPass);
					pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
					m_pEffect->EndPass();
				}
				m_pEffect->End();
			}
		}

		//재귀적으로 모든 프레임에 대해서 실행.
		if (pBone->pFrameSibling)
		{
			Render((ST_BONE*)pBone->pFrameSibling);
		}

		if (pBone->pFrameFirstChild)
		{
			Render((ST_BONE*)pBone->pFrameFirstChild);
		}
	}

	LPD3DXEFFECT cSkinnedMesh::LoadEffect(char* szFilename)
	{
		LPD3DXEFFECT pEffect = NULL;

		D3DXMACRO mac[2] =
		{
			{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
			{ NULL, NULL }
		};

		D3DCAPS9 caps;
		D3DXMACRO *pmac = NULL;
		g_pD3DDevice->GetDeviceCaps(&caps);
		if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
			pmac = mac;

		DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
		dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
		dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

		std::string sFileName = szFilename;
		std::wstring wFileName = L"";
		wFileName.assign(sFileName.begin(), sFileName.end());

		ID3DXBuffer* pBuffer = NULL;
		if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice,
			wFileName.c_str(),
			pmac,
			NULL,
			dwShaderFlags,
			NULL,
			&pEffect,
			&pBuffer)))
		{
			// if creation fails, and debug information has been returned, output debug info
			if (pBuffer)
			{
				OutputDebugStringA((char*)pBuffer->GetBufferPointer());
				SAFE_RELEASE(pBuffer);
			}

			return NULL;
		}

		return pEffect;
	}

	void cSkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
	{
		pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
		if (pmatParent)
		{
			pCurrent->CombinedTransformationMatrix =
				pCurrent->CombinedTransformationMatrix * (*pmatParent);
		}

		if (pCurrent->pFrameSibling)
		{
			Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
		}

		if (pCurrent->pFrameFirstChild)
		{
			Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
		}
	}

	void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
	{
		assert(pBone);

		// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
		// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
		if (pBone->pMeshContainer)
		{
			ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
			if (pBoneMesh->pSkinInfo)
			{
				LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
				// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
				// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
				// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
				// 찾아서 월드매트릭스를 걸어줘라.
				DWORD dwNumBones = pSkinInfo->GetNumBones();
				for (DWORD i = 0; i < dwNumBones; ++i)
				{
					LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
					if (szBoneName == NULL || strlen(szBoneName) == 0)
						continue;
					ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
					pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
				}
			}
		}

		//재귀적으로 모든 프레임에 대해서 실행.
		if (pBone->pFrameSibling)
		{
			SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
		}

		if (pBone->pFrameFirstChild)
		{
			SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
		}
	}

	void cSkinnedMesh::SetAnimationIndex(int nIndex)
	{
		if (!m_pAnimController)
			return;
		LPD3DXANIMATIONSET pAnimSet = NULL;
		m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
		m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
		SAFE_RELEASE(pAnimSet);
	}

	void cSkinnedMesh::Destroy()
	{
		cAllocateHierarchy ah;
		D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
		SAFE_DELETE_ARRAY(m_pmWorkingPalette);
		SAFE_RELEASE(m_pEffect);
	}

	void cSkinnedMesh::SetRandomTrackPosition()
	{
		m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
	}

	void cSkinnedMesh::SetTrackPosition(DOUBLE pos)
	{
		m_pAnimController->SetTrackPosition(0, pos);
	}

	DOUBLE cSkinnedMesh::GetTrackPosition()
	{
		D3DXTRACK_DESC  desc;
		m_pAnimController->GetTrackDesc(0, &desc);
		return desc.Position;
	}

	void cSkinnedMesh::SetCollisionSphere(OUT D3DXVECTOR3* _vMin, OUT D3DXVECTOR3* _vMax)
	{
		D3DXVECTOR3 vMin, vMax;
		ST_BONE* pBone;
		bool isInitCollisionBox = false;

		//모든 메쉬를 확인하고 충돌박스 크기를 구한다.
		vMin = D3DXVECTOR3(0, 0, 0);
		vMax = D3DXVECTOR3(0, 0, 0);
		pBone = m_pRootFrame;

		GetMinMaxVertex(m_pRootFrame, &vMin, &vMax, &isInitCollisionBox);

		*_vMin = vMin;
		*_vMax = vMax;
	}

	void cSkinnedMesh::GetMinMaxVertex(ST_BONE* pBone, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, bool* pIsInitCollisionBox)
	{
		LPDIRECT3DVERTEXBUFFER9 pVB;
		void* pVOid;

		if (pBone && pBone->pMeshContainer)
		{
			ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

			pBoneMesh->pOrigMesh->GetVertexBuffer(&pVB);
			pVB->Lock(0, 0, &pVOid, 0);

			DWORD numVerts = pBoneMesh->pOrigMesh->GetNumVertices();
			DWORD fvfSize = D3DXGetFVFVertexSize(pBoneMesh->pOrigMesh->GetFVF());

			char* pBuffer = (char*)pVOid;
			D3DXVECTOR3* pVertex;
			for (DWORD i = 0; i < numVerts; i++)
			{
				pVertex = (D3DXVECTOR3*)pBuffer;

				if (*pIsInitCollisionBox == false)
				{
					*pIsInitCollisionBox = true;
					*pMin = (D3DXVECTOR3)*pVertex;
					*pMax = (D3DXVECTOR3)*pVertex;
				}
				else
				{
					if (pMax->x < pVertex->x) pMax->x = pVertex->x;
					if (pMax->y < pVertex->y) pMax->y = pVertex->y;
					if (pMax->z < pVertex->z) pMax->z = pVertex->z;

					if (pMin->x > pVertex->x) pMin->x = pVertex->x;
					if (pMin->y > pVertex->y) pMin->y = pVertex->y;
					if (pMin->z > pVertex->z) pMin->z = pVertex->z;
				}
				pBuffer += fvfSize;
			}
			pVB->Unlock();
		}

		if (pBone->pFrameSibling)
			GetMinMaxVertex((ST_BONE*)pBone->pFrameSibling, pMin, pMax, pIsInitCollisionBox);
		if (pBone->pFrameFirstChild)
			GetMinMaxVertex((ST_BONE*)pBone->pFrameFirstChild, pMin, pMax, pIsInitCollisionBox);
	}
}