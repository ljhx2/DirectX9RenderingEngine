#include "Common.h"
#include "include\D3DRenderer.h"
#include "cSkinnedMeshComponent.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"

namespace ENGINE
{
	cSkinnedMeshComponent::cSkinnedMeshComponent()
		: m_pRootFrame(NULL)
		, m_pAnimController(NULL)
		, m_dwWorkingPaletteSize(0)
		, m_pmWorkingPalette(NULL)
		, m_pEffect(NULL)
	{
		eComponent_Type = eSKINNED_MESH_TYPE;
		m_pOwnerObject = NULL;

		m_IsPlay = false;
		m_pCurrentClip = NULL;
	}

	cSkinnedMeshComponent::cSkinnedMeshComponent(GameObject* pOwnerObject)
		: m_pRootFrame(NULL)
		, m_pAnimController(NULL)
		, m_dwWorkingPaletteSize(0)
		, m_pmWorkingPalette(NULL)
		, m_pEffect(NULL)
	{
		eComponent_Type = eSKINNED_MESH_TYPE;
		m_pOwnerObject = pOwnerObject;

		m_IsPlay = false;
		m_pCurrentClip = NULL;
	}

	cSkinnedMeshComponent::cSkinnedMeshComponent(GameObject* pOwnerObject, char* szFolder, const char* szFilename)
		: m_pRootFrame(NULL)
		, m_pAnimController(NULL)
		, m_dwWorkingPaletteSize(0)
		, m_pmWorkingPalette(NULL)
		, m_pEffect(NULL)
	{
		eComponent_Type = eSKINNED_MESH_TYPE;
		m_pOwnerObject = pOwnerObject;

		m_IsPlay = false;
		m_pCurrentClip = NULL;

		cSkinnedMesh* pSkinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

		m_pRootFrame = pSkinnedMesh->m_pRootFrame;
		m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
		m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
		m_pEffect = pSkinnedMesh->m_pEffect;
		m_vMin = pSkinnedMesh->m_vMin;
		m_vMax = pSkinnedMesh->m_vMax;

		pSkinnedMesh->m_pAnimController->CloneAnimationController(
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
			pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
			pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
			&(this->m_pAnimController));

		SetMatrixMap(m_pRootFrame, &m_mapBipMatrix);
	}

	cSkinnedMeshComponent::~cSkinnedMeshComponent()
	{
		SAFE_RELEASE(m_pAnimController);
	}

	void cSkinnedMeshComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		m_deltaTime = deltaTime;

		m_matWorld = matWorld;



		//m_pCurrentClip�� ������ �����ؼ�
		//�ִϸ��̼��� �� �糪
		// loopType �̶�� �ٽ� TrackPosition�� Clip.Start �� ����


	}

	void cSkinnedMeshComponent::Render()
	{
		if (m_IsPlay)
		{
			if (m_pAnimController) m_pAnimController->AdvanceTime(m_deltaTime, NULL);
			DOUBLE temp = GetTrakPosition();
			if (temp >= m_pCurrentClip->end) //�ִϸ��̼��� ������
			{
				if (m_pCurrentClip->isLoop) SetTrackPosition(m_pCurrentClip->start);//������ ó������ ���ư�
				else m_IsPlay = false;  //������ �ƴϸ� ����
			}
		}
		else
		{
			if (m_pAnimController) m_pAnimController->AdvanceTime(0, NULL);
		}

		XUpdate(m_pRootFrame, m_matWorld);
		XRender(m_pRootFrame);
	}

	void cSkinnedMeshComponent::StartAnim(ST_AnimClip* clip)
	{
		m_IsPlay = true;
		m_pCurrentClip = clip;
		SetAnimIndex(m_pCurrentClip->setIndex);
		SetTrackPosition(m_pCurrentClip->start);
	}

	void cSkinnedMeshComponent::XRender(ST_BONE* pBone)
	{
		assert(pBone);

		// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
		// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
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

		//��������� ��� �����ӿ� ���ؼ� ����.
		if (pBone->pFrameSibling)
		{
			XRender((ST_BONE*)pBone->pFrameSibling);
		}

		if (pBone->pFrameFirstChild)
		{
			XRender((ST_BONE*)pBone->pFrameFirstChild);
		}
	}

	void cSkinnedMeshComponent::XUpdate(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
	{
		pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
		if (pCurrent->Name)
			m_mapBipMatrix[pCurrent->Name] = pCurrent->TransformationMatrix;

		if (pmatParent)
		{
			pCurrent->CombinedTransformationMatrix =
				pCurrent->CombinedTransformationMatrix * (*pmatParent);
			if (pCurrent->Name)
				m_mapBipMatrix[pCurrent->Name] = pCurrent->CombinedTransformationMatrix;
		}

		if (pCurrent->pFrameSibling)
		{
			XUpdate((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
		}

		if (pCurrent->pFrameFirstChild)
		{
			XUpdate((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
		}
	}

	void cSkinnedMeshComponent::SetAnimIndex(int nIndex)
	{
		if (!m_pAnimController) return;
		LPD3DXANIMATIONSET pAnimSet = NULL;
		m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
		m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
		SAFE_RELEASE(pAnimSet);
	}

	void cSkinnedMeshComponent::SetTrackPosition(DOUBLE pos)
	{
		if (!m_pAnimController) return;
		m_pAnimController->SetTrackPosition(0, pos);
	}

	DOUBLE cSkinnedMeshComponent::GetTrakPosition()
	{
		D3DXTRACK_DESC  desc;
		m_pAnimController->GetTrackDesc(0, &desc);
		return desc.Position;
	}

	D3DXMATRIXA16* cSkinnedMeshComponent::GetMatrixFromeFrame(const char* frameName)
	{
		//D3DXMATRIXA16* pMat = NULL;
		//
		//FindMatrixFromFrame(m_pRootFrame, frameName, pMat);
		//
		//return pMat;

		return &m_mapBipMatrix[frameName];
	}

	void cSkinnedMeshComponent::FindMatrixFromFrame(IN ST_BONE* pBone, IN const char* frameName, OUT D3DXMATRIXA16* & pMat)
	{
		//D3DXFRAME* pTempFrame = D3DXFrameFind(m_pRootFrame, "�������̸�");

		if (pBone == NULL) return;

		if (pBone->Name != NULL && strcmp(pBone->Name, frameName) == 0)
		{
			pMat = &pBone->CombinedTransformationMatrix;
		}

		FindMatrixFromFrame((ST_BONE*)pBone->pFrameSibling, frameName, pMat);
		FindMatrixFromFrame((ST_BONE*)pBone->pFrameFirstChild, frameName, pMat);
	}

	bool cSkinnedMeshComponent::IntersectRayMesh(D3DXVECTOR3 origin, D3DXVECTOR3 dir)
	{
		BOOL bHit = FALSE; //�������� �޽��� ���� �ߴ��� ��Ÿ���� bool��
		DWORD dwFace; // ������ ������ ���� ����� ���� �ε�����
		FLOAT fU, fV;
		FLOAT fDist = FLT_MAX; //�߽� ��Ʈ ��ǥ U��, �߽� ��Ʈ ��ǥ V��, �������� �������� ���������� �Ÿ�

		// ���ڷ� ���� origin�� dir�� ������ǥ�� �������� �� ���̴�.
		// orgin�� dir�� ������ǥ �������� �ٲ��ش�.
		D3DXMATRIXA16 worldInverse;
		D3DXMatrixInverse(&worldInverse, 0, m_matWorld);

		D3DXVECTOR3 localOrigin, localDir;
		D3DXVec3TransformCoord(&localOrigin, &origin, &worldInverse);
		D3DXVec3TransformNormal(&localDir, &dir, &worldInverse);
		D3DXVec3Normalize(&localDir, &localDir);
		
		//��� �޽��� ���鼭 ��ŷ�˻��Ѵ�.
		CheckPicking(localOrigin, localDir, m_pRootFrame, &bHit, &dwFace, &fU, &fV, &fDist);

		if (bHit == TRUE)
		{
			return true;
		}

		return false;
	}

	void cSkinnedMeshComponent::CheckPicking(IN D3DXVECTOR3 origin, IN D3DXVECTOR3 dir, IN ST_BONE* pBone,
		OUT BOOL* _bHit, OUT DWORD* _dwFace, OUT FLOAT* _fU, OUT FLOAT* _fV, OUT FLOAT* _fDist)
	{
		if (pBone->pMeshContainer)
		{
			ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
			LPD3DXMESH mesh = pBoneMesh->pWorkingMesh;

			BOOL bHit; //�������� �޽��� ���� �ߴ��� ��Ÿ���� bool��
			DWORD dwFace; // ������ ������ ���� ����� ���� �ε�����
			FLOAT fU, fV, fDist; //�߽� ��Ʈ ��ǥ U��, �߽� ��Ʈ ��ǥ V��, �������� �������� ���������� �Ÿ�

			D3DXIntersect(mesh, &origin, &dir, &bHit, &dwFace, &fU, &fV, &fDist, NULL, NULL);

			if (bHit == TRUE)
			{
				*_bHit = TRUE;
				if (fDist < *_fDist)
				{
					*_dwFace = dwFace;
					*_fU = fU;
					*_fV = fV;
					*_fDist = fDist;
				}
			}
		}
		if (pBone->pFrameSibling)
			CheckPicking(origin, dir, (ST_BONE*)pBone->pFrameSibling, _bHit, _dwFace, _fU, _fV, _fDist);
		if (pBone->pFrameFirstChild)
			CheckPicking(origin, dir, (ST_BONE*)pBone->pFrameFirstChild, _bHit, _dwFace, _fU, _fV, _fDist);
	}

	void cSkinnedMeshComponent::SetMatrixMap(IN ST_BONE* pBone, OUT std::map<std::string, D3DXMATRIXA16>* map)
	{
		if (pBone == NULL) return;
		if (pBone->Name == NULL) return;
		
		//map[pBone->Name] = pBone->TransformationMatrix;
		map->insert(std::make_pair(std::string(pBone->Name), pBone->CombinedTransformationMatrix));

		if (pBone->pFrameSibling)
			SetMatrixMap((ST_BONE*)pBone->pFrameSibling, map);
		if (pBone->pFrameFirstChild)
			SetMatrixMap((ST_BONE*)pBone->pFrameFirstChild, map);
	}
}