#pragma once
#include "fbxsdk.h"
#include "IComponent.h"
#include "cFbxData.h"
//#include "includeFbx\fbxsdk.h"

namespace ENGINE
{
	class GameObject;
	class cFbxData;
	class cFbxComponent : public IComponent
	{
	private://내부 함수
		bool IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
			D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v); // 픽킹에사용

	public://기본 함수
		cFbxComponent();
		cFbxComponent(GameObject* pOwnerObject);
		cFbxComponent(GameObject* pOwnerObject, char* szFolder, const char* szFilename);
		virtual ~cFbxComponent();

		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void PrevRender();
		virtual void Render();
		void Destroy();	//버텍스 버퍼 릴리즈(staticmeshManager에서만 호출)
	public://Get,Set함수
		D3DXVECTOR3 GetMaxPoint() { return m_vMax; }//cCollider에서사용
		D3DXVECTOR3	GetMinPoint() { return m_vMin; }//cCollider에서사용
		D3DXMATRIXA16* GetMatrixFromeFrame(const char* frameName);//하이라키 할떄 쓸용
		void StartAnim(ST_AnimClip* clip);		//AnimComponent에서 사용
		char* GetFolderPath(){ return m_szFolder; }
		char* GetFilePath(){ return m_szFilename; }
		std::map<std::string, D3DXMATRIXA16>* GetBipMatrixMap(){ return &m_vBipMatrixMap; }
		char* GetMeterialTexturePath(const char* name)
		{
			std::map<std::string, FBX_MTL_TEX*>::iterator iter;
			for (iter = m_vMeterialTextureMap.begin(); iter != m_vMeterialTextureMap.end(); iter++)
			{
				if (strcmp(iter->first.c_str(), name) == 0)
					return iter->second->path;
			}
			return NULL;
		}
		void GetMeterialTextureData(int i, char* outName, char* outPath, D3DMATERIAL9* pMtl)
		{
			std::map<std::string, FBX_MTL_TEX*>::iterator iter;
			iter = m_vMeterialTextureMap.begin();
			for (int cnt = 0; cnt < i; cnt++)	iter++;

			strcpy(outName, iter->first.c_str());
			strcpy(outPath, iter->second->path);

			*pMtl = iter->second->stMtl;
		}
		int GetMeterialTextureNum(){ return m_vMeterialTextureMap.size(); }
		void SetMeterialTexture(char* name, char* path, float r, float g, float b, float a);
		int GetFbxGroupDataNum(){ return m_pFbxData->GetGroupDataNum(); }
		const char* GetFbxGroupDataName(int i){ return m_pFbxData->GetGroupDataName(i); }

		long GetCurrentFrame() { return mCurrentTime.GetFrameCount(); }

	public: //픽킹
		bool IntersectRayMesh(D3DXVECTOR3 origin, D3DXVECTOR3 dir);//메쉬와 충돌 반직선과 충돌여부검사
	public:
		//diffuse텍스쳐 경로에서 normalMap,SpecularMap 텍스쳐 경로를 알아낸다( add - _NRM, _SPEC)
		void SetTexturePath(char* dest, char* source, char* addStr);
	public:
		char						m_szFolder[256];
		char						m_szFilename[256];
	private:
		cFbxData*					m_pFbxData;
		D3DXMATRIXA16*				m_matWorld;

		//바운딩스피어
		D3DXVECTOR3					m_vMin;
		D3DXVECTOR3					m_vMax;

		//애니메이션 정보
		float						m_deltaTime;
		bool						m_IsPlay;

		ST_AnimClip*				m_pCurrentClip;
		mutable FbxTime mStart, mStop, mCurrentTime;
		//fbx bone 매트릭스 저장
		std::map<std::string, D3DXMATRIXA16>	m_vBipMatrixMap;
		//각 노드의 텍스쳐와 재질값 저장 맵
		std::map<std::string, FBX_MTL_TEX*>	m_vMeterialTextureMap;

		float m_AccrueDeltaTime;
	};
}

