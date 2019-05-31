#pragma once

struct ST_BONE;

namespace ENGINE
{

	class cSkinnedMesh
	{
		friend class cSkinnedMeshManager;
		friend class cSkinnedMeshComponent;

	private:
		//하나만 생성
		ST_BONE*					m_pRootFrame;
		DWORD						m_dwWorkingPaletteSize;
		D3DXMATRIX*					m_pmWorkingPalette;
		LPD3DXEFFECT				m_pEffect;

		// 객체마다 생성
		LPD3DXANIMATIONCONTROLLER	m_pAnimController;

		// 바운딩스피어
		D3DXVECTOR3					m_vMin;
		D3DXVECTOR3					m_vMax;

		//SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
		D3DXVECTOR3 m_vPosition;
	public:
		cSkinnedMesh(char* szFolder, const char* szFilename);
		~cSkinnedMesh(void);

		void UpdateAndRender();
		void SetAnimationIndex(int nIndex);

		void SetRandomTrackPosition(); // 테스트용

		///[jack]
		void SetTrackPosition(DOUBLE pos);
		DOUBLE GetTrackPosition();

		void SetPosition(D3DXVECTOR3 pos){
			m_vPosition = pos;
		}

	private:
		cSkinnedMesh();

		void Load(char* szFolder, const char* szFilename);
		LPD3DXEFFECT LoadEffect(char* szFilename);
		void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
		void Render(ST_BONE* pBone = NULL);
		void SetupBoneMatrixPtrs(ST_BONE* pBone);
		void Destroy();

		//바운딩스피어 구하는 함수
		void SetCollisionSphere(OUT D3DXVECTOR3* _vMin, OUT D3DXVECTOR3* _vMax);
		void GetMinMaxVertex(ST_BONE* pBone, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, bool* pIsInitCollisionBox);
	};

}