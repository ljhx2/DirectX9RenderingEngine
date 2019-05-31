#pragma once
#include "IComponent.h"

struct ST_BONE;


namespace ENGINE
{
	class GameObject;
	

	class cSkinnedMeshComponent : public IComponent
	{
	public:
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();

		void SetAnimIndex(int nIndex);
		void SetTrackPosition(DOUBLE pos);
		DOUBLE GetTrakPosition();

		D3DXMATRIXA16* GetMatrixFromeFrame(const char* frameName);

		void StartAnim(ST_AnimClip* clip);		//AnimComponent에서 사용

		D3DXVECTOR3 GetMaxPoint() { return m_vMax; }//cCollider에서사용
		D3DXVECTOR3	GetMinPoint() { return m_vMin; }//cCollider에서사용

		bool IntersectRayMesh(D3DXVECTOR3 origin, D3DXVECTOR3 dir); //메쉬와 충돌 반직선과 충돌여부검사
	

		cSkinnedMeshComponent();
		cSkinnedMeshComponent(GameObject* pOwnerObject);
		cSkinnedMeshComponent(GameObject* pOwnerObject, char* szFolder, const char* szFilename);
		virtual ~cSkinnedMeshComponent();

	private:
		void XRender(ST_BONE* pBone);
		void XUpdate(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);

		void FindMatrixFromFrame(IN ST_BONE* pBone, IN const char* frameName, OUT D3DXMATRIXA16* & pMat);
		void FindMesh(IN ST_BONE* pBone, OUT ID3DXMesh* & pMesh);

		void CheckPicking(IN D3DXVECTOR3 origin, IN D3DXVECTOR3 dir, IN ST_BONE* pBone, 
			OUT BOOL* bHit, OUT DWORD* dwFace, OUT FLOAT* fU, OUT FLOAT* fV, OUT FLOAT* fDist);

		void SetMatrixMap(IN ST_BONE* pBone, OUT std::map<std::string, D3DXMATRIXA16>* map);
	private:
		ST_BONE*				m_pRootFrame;
		DWORD					m_dwWorkingPaletteSize;
		D3DXMATRIX*				m_pmWorkingPalette;
		LPD3DXEFFECT			m_pEffect;
		

		LPD3DXANIMATIONCONTROLLER	m_pAnimController;

		//바운딩스피어
		D3DXVECTOR3					m_vMin;
		D3DXVECTOR3					m_vMax;



		D3DXMATRIXA16*				m_matWorld;
		float						m_deltaTime;

		bool						m_IsPlay;
		ST_AnimClip*				m_pCurrentClip;

		std::map<std::string, D3DXMATRIXA16> m_mapBipMatrix;
	};

}