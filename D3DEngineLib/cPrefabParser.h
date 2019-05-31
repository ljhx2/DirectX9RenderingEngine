#pragma once

namespace ENGINE
{
	class GameObject;
	class cStaticMeshComponent;
	class cSkinnedMesh;
	class cFbxComponent;
	class cColliderComponent;
	class cAnimComponent;
	class cScriptComponent;
	enum GAMEOBJECT_TAG;

	class cPrefabParser
	{
		D3DXVECTOR3 m_WorldPos;

	public://prefab저장 함수
		void SavePrefab(GameObject* pSelectObject, char* fileName);
		void GameObjectSave(FILE *fp, GameObject* pObject, int childLevel/*루트 게임오브젝트는 이 값이 0*/);
		void TranslateComponentSave(FILE *fp, GameObject* pObject, int childLevel);
		void StaticMeshComponentSave(FILE *fp, cStaticMeshComponent* pComponent, int childLevel);
		void SkinnedMeshComponentSave(FILE *fp, cSkinnedMesh* pComponent, int childLevel);
		void FbxComponentSave(FILE *fp, cFbxComponent* pComponent, int childLevel);
		void ColliderComponentSave(FILE *fp, cColliderComponent* pComponent, int childLevel);
		void AnimComponentSave(FILE *fp, cAnimComponent* pComponent, int childLevel);
		void AttachBipNameSave(FILE *fp, GameObject* pObject, int childLevel);
		void ScriptComponentSave(FILE *fp, cScriptComponent* pComponent, int childLevel);
	public://prefab로드 함수
		GameObject* LoadPrefab(char* fileName, D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0));
		GameObject* GameObjectLoad(FILE *fp, GameObject* pParentObject, int childLevel);
		void TranslateComponentLoad(FILE *fp, GameObject* pObject, int childLevel);
		void StaticMeshComponentLoad(FILE *fp, GameObject* pObject, int childLevel);
		void SkinnedMeshComponentLoad(FILE *fp, GameObject* pObject, int childLevel);
		void FbxComponentLoad(FILE *fp, GameObject* pObject, int childLevel);
		void ColliderComponentLoad(FILE *fp, GameObject* pObject, int childLevel);
		void AnimComponentLoad(FILE *fp, GameObject* pObject, int childLevel);
		void AttachBipNameLoad(FILE *fp, GameObject* pParentObject, GameObject* pObject, int childLevel);
		void ScriptComponentLoad(FILE *fp, GameObject* pObject, int childLevel);

		char* GetToken(FILE *fp, char* strValue);
		
		//태그값을 int값과 변환 해주는 함수
		char* TagToChar(GAMEOBJECT_TAG tag);
		GAMEOBJECT_TAG CharToTag(char* tagName);
	public:
		cPrefabParser();
		~cPrefabParser();
	};
}

