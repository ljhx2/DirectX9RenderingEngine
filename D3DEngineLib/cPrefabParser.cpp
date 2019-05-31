#include "Common.h"
#include "cPrefabParser.h"
#include "include\D3DRenderer.h"

namespace ENGINE
{
	cPrefabParser::cPrefabParser()
	{
		m_WorldPos = D3DXVECTOR3(0, 0, 0);
	}
	cPrefabParser::~cPrefabParser()
	{
	}
	//---------------Prefab Save -----------------------
	void cPrefabParser::SavePrefab(GameObject* pSelectObject, char* fileName)
	{
		FILE *fp;
		fopen_s(&fp, fileName, "wt");

		//해더 저장 
		fprintf_s(fp, "#Prefab File Load\n");
		
		GameObjectSave(fp, pSelectObject, 0);

		fclose(fp);
	}
	void cPrefabParser::GameObjectSave(FILE *fp, GameObject* pObject, int childLevel)
	{
		for (int i = 0; i < childLevel; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "gameObject\n");

		for (int i = 0; i < childLevel; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//게임 오브젝트 괄호 시작

		//게임 오브젝트이름 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "name\n");

		char* bufName = UTIL::TCHARToChar(pObject->GetName().c_str());
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%s\n", bufName);

		//게임 오보젝트 태그 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "GAMEOBJECT_TAG\n");

		GAMEOBJECT_TAG tag = pObject->GetGameObjectTag();
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%s\n", TagToChar(tag));

		//srt 저장
		TranslateComponentSave(fp, pObject, childLevel);

		//컴포넌트들 저장
		int componentSize = pObject->GetComponentSize();

		for (int i = 0; i < componentSize; i++)
		{
			if (pObject->GetCompoent(i)->GetType() == ENGINE::eSTATIC_MESH_TYPE)
			{
				StaticMeshComponentSave(fp, (cStaticMeshComponent*)pObject->GetCompoent(i), childLevel);
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eSKINNED_MESH_TYPE)
			{
				SkinnedMeshComponentSave(fp, (cSkinnedMesh*)pObject->GetCompoent(i), childLevel);
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eFBX_MESH_TYPE)
			{
				FbxComponentSave(fp, (cFbxComponent*)pObject->GetCompoent(i), childLevel);
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eCOLLIDER_TYPE)
			{
				ColliderComponentSave(fp, (cColliderComponent*)pObject->GetCompoent(i), childLevel);
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
			{
				AnimComponentSave(fp, (cAnimComponent*)pObject->GetCompoent(i), childLevel);
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eSKYBOX_TYPE)
			{

			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eTERRAIN_TYPE)
			{

			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eSCRIPT_TYPE)
			{
				ScriptComponentSave(fp, (cScriptComponent*)pObject->GetCompoent(i), childLevel);
			}
			else
			{
				fprintf_s(fp, "\tCOMPONENT ERROR\n");
			}
		}

		// 씬루트가 아닌부모가 있다면 어태치 정보를 저장한다
		if (pObject->GetParent()  != NULL && pObject->GetParent() != GetSceneRoot())
			AttachBipNameSave(fp, pObject, childLevel);

		//자식 게임 오브젝트 저장
		for (int i = 0; i < pObject->GetVecChild().size(); i++)
			GameObjectSave(fp, pObject->GetVecChild()[i], childLevel + 1);


		for (int i = 0; i < childLevel; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//게임 오브젝트 괄호 종료
	}
	void cPrefabParser::TranslateComponentSave(FILE *fp, GameObject* pObject, int childLevel)
	{
		//translate component 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "TranslateComponent\n");
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//translate component 괄호 시작

		D3DXVECTOR3 pos = pObject->GetTranslate();
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "POS\n");
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%f %f %f\n", pos.x, pos.y, pos.z);

		D3DXVECTOR3 rot = pObject->GetRotation();
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "ROT\n");
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%f %f %f\n", rot.x, rot.y, rot.z);

		D3DXVECTOR3 scale = pObject->GetScale();
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "SCALE\n");
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%f %f %f\n", scale.x, scale.y, scale.z);

		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//translate component 괄호 종료
	}
	void cPrefabParser::StaticMeshComponentSave(FILE *fp, cStaticMeshComponent* pComponent, int childLevel)
	{

	}
	void cPrefabParser::SkinnedMeshComponentSave(FILE *fp, cSkinnedMesh* pComponent, int childLevel)
	{

	}
	void cPrefabParser::FbxComponentSave(FILE *fp, cFbxComponent* pComponent, int childLevel)
	{
		//fbx component 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "FbxMeshComponent\n");
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//fbx component 괄호 시작

		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "folderPath %s\n", pComponent->GetFolderPath());


		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "filePath %s\n", pComponent->GetFilePath());

		int nMeterialTextureNum = pComponent->GetMeterialTextureNum();

		for (int cnt = 0; cnt < nMeterialTextureNum; cnt++)
		{
			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "MeterialTexture\n");

			char chName[512];
			char chPath[512];
			D3DMATERIAL9 mtl;

			pComponent->GetMeterialTextureData(cnt, chName, chPath, &mtl);
			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "%s %s\n", chName, chPath);

			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "%f %f %f %f\n", mtl.Diffuse.r, mtl.Diffuse.g, mtl.Diffuse.b, mtl.Diffuse.a);
		}

		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//fbx component 괄호 종료
	}
	void cPrefabParser::ColliderComponentSave(FILE *fp, cColliderComponent* pComponent, int childLevel)
	{
		//collider component 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "ColliderComponent\n");
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//collider component 괄호 시작

		//BoundingSphere 렌더링 여부
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "BOUNDING_RENDER\n");
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%d\n", pComponent->GetRenderCOllisionSphere());
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		//-----
		fprintf_s(fp, "COLLISION_TYPE\n");
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%d\n", (int)pComponent->GetCollisionTYPE());
		//-----

		//Collision
		std::map<std::string, cCollisionShape*>* pMapCollision = pComponent->GetMapCollision();
		std::map<std::string, cCollisionShape*>::iterator iter2;
		for (iter2 = pMapCollision->begin(); iter2 != pMapCollision->end(); ++iter2)
		{
			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "COLLISION_SHAPE\n");
			
			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "%d %s %f %f %f %f %f %f %f %f %f \"%s\" %d\n", 
				iter2->second->GetShapeType(),
				iter2->second->GetName().c_str(),
				iter2->second->GetLocalScale().x, iter2->second->GetLocalScale().y, iter2->second->GetLocalScale().z,
				iter2->second->GetLocalRotation().x, iter2->second->GetLocalRotation().y, iter2->second->GetLocalRotation().z,
				iter2->second->GetLocalPosition().x, iter2->second->GetLocalPosition().y, iter2->second->GetLocalPosition().z,
				iter2->second->GetParentName().c_str(),
				iter2->second->GetIsRender());
		}

		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//anim component 괄호 종료
	}
	void cPrefabParser::AnimComponentSave(FILE *fp, cAnimComponent* pComponent, int childLevel)
	{
		//anim component 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "AnimComponent\n");
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//anim component 괄호 시작

		int clipSize = pComponent->GetAniClipNum();
		std::map<std::string, ST_AnimClip*>* pvAniClip = pComponent->GetAniClip();
		std::map<std::string, ST_AnimClip*>::iterator iter;

		for (iter = pvAniClip->begin(); iter != pvAniClip->end(); ++iter)
		{
			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "ANIM_CLIP\n");
			for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
			fprintf_s(fp, "%s %d %f %f %d\n", iter->second->name.c_str(), iter->second->setIndex, iter->second->start, iter->second->end, iter->second->isLoop);
		}

		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//anim component 괄호 종료
	}
	void cPrefabParser::AttachBipNameSave(FILE *fp, GameObject* pObject, int childLevel)
	{
		//attachBipName 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "AttackBipName\n");
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//괄호 시작

		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, pObject->GetAttackBipName()); fprintf_s(fp, "\n");

		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//괄호 종료
	}
	void cPrefabParser::ScriptComponentSave(FILE *fp, cScriptComponent* pComponent, int childLevel)
	{
		//Script component 저장
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "ScriptComponent\n");
		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "{\n");//Script component 괄호 시작


		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "ScriptPath\n");
		for (int i = 0; i < childLevel + 2; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "%s\n", pComponent->GetScriptPath());


		for (int i = 0; i < childLevel + 1; i++)  fprintf_s(fp, "\t");
		fprintf_s(fp, "}\n");//Script component 괄호 종료
	}

	//---------------Prefab Load -----------------------
	GameObject* cPrefabParser::LoadPrefab(char* fileName, D3DXVECTOR3 pos)
	{
		m_WorldPos = pos;
		FILE *fp = NULL;
		char szBuf[1024];
		fopen_s(&fp, fileName, "r");

		if (!fp) return NULL;

		fgets(szBuf, 1024, fp);
		if (strcmp("#Prefab File Load\n", szBuf) == 1) return NULL;

		GameObject* pObj = NULL;
		fgets(szBuf, 1024, fp);
		if (strcmp("gameObject\n", szBuf) == 0)	{
			pObj = GameObjectLoad(fp, NULL, 0);
			pObj->SetParent(g_pSceneRoot);
			
		}

		fclose(fp);

		return pObj;

		/*
		char* -> wstring

		std::wstring objName;
		objName = std::wstring(fileName, fileName + strlen(fileName));
		*/
		/*
		wstring -> char*

		std::wstring objName;
		size_t getValue = 0;
		const wchar_t *pszUserID = objName.c_str();
		wcstombs_s(&getValue, fileName, 60, pszUserID, _TRUNCATE);
		*/
	}
	GameObject* cPrefabParser::GameObjectLoad(FILE *fp, GameObject* pParentObject, int childLevel)
	{
		GameObject* pObj = NULL;
		char szBuf[1024];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);

			if (szBuf[0] == '{'){//게임 오브젝트 시작 괄호
			}
			else if (szBuf[0] == '}'){ //게임 오브젝트 종료 괄호
				break; 
			}
			else if (strcmp("name", szBuf) == 0){//이름을 로드하고 게임오브젝트 생성
				GetToken(fp, szBuf);//이름값 받아오기
				std::wstring objName;
				objName = std::wstring(szBuf, szBuf + strlen(szBuf));

				pObj = new GameObject(g_pUIDManager->CreateUID(), objName);
				g_mulmapEnableObject.insert(std::make_pair(pObj->GetName(), pObj));
			}
			else if (strcmp("GAMEOBJECT_TAG", szBuf) == 0){//태그를 읽어 들여 게임 오브젝트에 넣어준다
				GetToken(fp, szBuf);

				GAMEOBJECT_TAG tag = CharToTag(szBuf);

				pObj->SetGameObjectTag(tag);
			}
			else if (strcmp("TranslateComponent", szBuf) == 0){//transComponent Load
				TranslateComponentLoad(fp, pObj, childLevel);
			}
			else if (strcmp("FbxMeshComponent", szBuf) == 0){//FbxMeshComponent Load
				FbxComponentLoad(fp, pObj, childLevel);
			}
			else if (strcmp("AnimComponent", szBuf) == 0){//AnimComponent Load
				AnimComponentLoad(fp, pObj, childLevel);
			}
			else if (strcmp("ColliderComponent", szBuf) == 0) {//ColliderComponent Load
				ColliderComponentLoad(fp, pObj, childLevel);
			}
			else if (strcmp("AttackBipName", szBuf) == 0){//AttackBipName Load
				AttachBipNameLoad(fp, pParentObject, pObj, childLevel);
			}
			else if (strcmp("ScriptComponent", szBuf) == 0){//ScriptComponent Load
				ScriptComponentLoad(fp, pObj, childLevel);
			}
			else if (strcmp("gameObject", szBuf) == 0){//child gameObject Load
				GameObjectLoad(fp, pObj, childLevel + 1);
			}
		}

		return pObj;
	}
	void cPrefabParser::TranslateComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{
		char szBuf[1024];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);
			if (szBuf[0] == '{'){//시작 괄호
			}
			else if (szBuf[0] == '}'){ //종료 괄호
				break;
			}
			else if (strcmp("POS", szBuf) == 0){
				D3DXVECTOR3 pos;

				GetToken(fp, szBuf);
				pos.x = atof(szBuf);

				GetToken(fp, szBuf);
				pos.y = atof(szBuf);

				GetToken(fp, szBuf);
				pos.z = atof(szBuf);

				//생성할때 위치 값 입력 안받았으면 프리팹 안에 값을 넣어준다
				//if (m_WorldPos.x == 0 && m_WorldPos.y == 0 && m_WorldPos.z == 0)
					pObject->SetTranslate(pos.x, pos.y, pos.z);
				//else
				//	pObject->SetTranslate(m_WorldPos.x, m_WorldPos.y, m_WorldPos.z);
			}
			else if (strcmp("ROT", szBuf) == 0){
				D3DXVECTOR3 rot;

				GetToken(fp, szBuf);
				rot.x = atof(szBuf);

				GetToken(fp, szBuf);
				rot.y = atof(szBuf);

				GetToken(fp, szBuf);
				rot.z = atof(szBuf);

				pObject->SetRotation(rot.x, rot.y, rot.z);
			}
			else if (strcmp("SCALE", szBuf) == 0){
				D3DXVECTOR3 scale;

				GetToken(fp, szBuf);
				scale.x = atof(szBuf);

				GetToken(fp, szBuf);
				scale.y = atof(szBuf);

				GetToken(fp, szBuf);
				scale.z = atof(szBuf);

				pObject->SetScale(scale.x, scale.y, scale.z);
			}
		}
	}
	void cPrefabParser::StaticMeshComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{

	}
	void cPrefabParser::SkinnedMeshComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{

	}
	void cPrefabParser::FbxComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{
		cFbxComponent* pFbxComponent = NULL;
		char szBuf[1024];
		char folderPath[256];
		char filePath[256];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);
			if (szBuf[0] == '{'){//시작 괄호
			}
			else if (szBuf[0] == '}'){ //종료 괄호
				break;
			}
			else if (strcmp("folderPath", szBuf) == 0){
				GetToken(fp, szBuf);
				strcpy(folderPath, szBuf);
			}
			else if (strcmp("filePath", szBuf) == 0){
				GetToken(fp, szBuf);
				strcpy(filePath, szBuf);

				//경로를 모두 입력 받았을 경우 컴포넌트 생성
				pFbxComponent = new cFbxComponent(pObject, folderPath, filePath);
			}
			else if (strcmp("MeterialTexture", szBuf) == 0){
				char chMulName[512];
				char chMulPath[512];
				D3DMATERIAL9 mtl;

				GetToken(fp, szBuf);
				strcpy(chMulName, szBuf);

				GetToken(fp, szBuf);
				strcpy(chMulPath, szBuf);

				//mtl diffuse값
				GetToken(fp, szBuf);
				mtl.Diffuse.r = atof(szBuf);

				GetToken(fp, szBuf);
				mtl.Diffuse.g = atof(szBuf);

				GetToken(fp, szBuf);
				mtl.Diffuse.b = atof(szBuf);

				GetToken(fp, szBuf);
				mtl.Diffuse.a = atof(szBuf);

				if (pFbxComponent != NULL)
					pFbxComponent->SetMeterialTexture(chMulName, chMulPath, mtl.Diffuse.r, mtl.Diffuse.g, mtl.Diffuse.b, mtl.Diffuse.a);
			}
		}

		//컴포넌트 추가
		pObject->AddComponent(pFbxComponent);
		
	}
	void cPrefabParser::ColliderComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{
		//Collider 컴포넌트 추가
		cColliderComponent* pColliderComp = new cColliderComponent(pObject);
		pObject->AddComponent(pColliderComp);

		char szBuf[1024];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);
			if (szBuf[0] == '{'){//시작 괄호
			}
			else if (szBuf[0] == '}'){ //종료 괄호
				break;
			}
			else if (strcmp("BOUNDING_RENDER", szBuf) == 0){
				bool isRender;

				GetToken(fp, szBuf);
				isRender = (bool)atoi(szBuf);

				pColliderComp->SetRenderCollisionSphere(isRender);
			}
			//-----
			else if (strcmp("COLLISION_TYPE", szBuf) == 0){
				ECOLLISION_TYPE colliderType;

				GetToken(fp, szBuf);
				colliderType = (ECOLLISION_TYPE)atoi(szBuf);

				pColliderComp->SetCollisionTYPE(colliderType);
			}
			//------
			
			else if (strcmp("COLLISION_SHAPE", szBuf) == 0){
				ESHAPE_TYPE type;
				char name[256];
				D3DXVECTOR3 scale, rotation, position;
				char parent_name[256];
				bool isRender;

				GetToken(fp, szBuf);
				type = (ESHAPE_TYPE)atoi(szBuf);

				GetToken(fp, szBuf);
				strcpy(name, szBuf);

				GetToken(fp, szBuf);
				scale.x = atof(szBuf);
				GetToken(fp, szBuf);
				scale.y = atof(szBuf);
				GetToken(fp, szBuf);
				scale.z = atof(szBuf);

				GetToken(fp, szBuf);
				rotation.x = atof(szBuf);
				GetToken(fp, szBuf);
				rotation.y = atof(szBuf);
				GetToken(fp, szBuf);
				rotation.z = atof(szBuf);

				GetToken(fp, szBuf);
				position.x = atof(szBuf);
				GetToken(fp, szBuf);
				position.y = atof(szBuf);
				GetToken(fp, szBuf);
				position.z = atof(szBuf);

				GetToken(fp, szBuf);
				strcpy(parent_name, szBuf);

				GetToken(fp, szBuf);
				isRender = (bool)atoi(szBuf);


				cCollisionShape* box;
				if (type == eBOX_SHAPE)
				{
					pColliderComp->AddCollisionBox(name);
					box = pColliderComp->GetCollisionBox(name);
				}
				else if (type == eSPHERE_SHAPE)
				{
					pColliderComp->AddCollisionSphere(name);
					box = pColliderComp->GetClassCollisionSphere(name);
				}
				box->SetLocalScale(scale);
				box->SetLocalRotation(rotation);
				box->SetLocalPosition(position);
				box->SetParentName(parent_name);
				box->SetIsRender(isRender);

				if (strcmp(parent_name, "NULL") == 0)
				{
					D3DXMATRIXA16* pMat = pObject->GetPWorldMatrix();
					box->SetParentMatrix(pMat);
				}
				else
				{
					cSkinnedMeshComponent* skinnedComp = pObject->GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
					cFbxComponent* fbxComp = pObject->GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);
					if (skinnedComp != NULL)
					{
						D3DXMATRIXA16* pMat = skinnedComp->GetMatrixFromeFrame(parent_name);
						box->SetParentMatrix(pMat);
					}
					else if (fbxComp != NULL)
					{
						D3DXMATRIXA16* pMat = fbxComp->GetMatrixFromeFrame(parent_name);
						box->SetParentMatrix(pMat);
					}
				}

			}
		}
	}
	void cPrefabParser::AnimComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{
		//애니 컴포넌트 추가
		cAnimComponent* pAnimComp = new cAnimComponent(pObject);
		pObject->AddComponent(pAnimComp);

		char szBuf[1024];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);
			if (szBuf[0] == '{'){//시작 괄호
			}
			else if (szBuf[0] == '}'){ //종료 괄호
				break;
			}
			else if (strcmp("ANIM_CLIP", szBuf) == 0){
				char name[256];
				int index; 
				float start, end;
				bool isLoop;

				GetToken(fp, szBuf);
				strcpy(name, szBuf);

				GetToken(fp, szBuf);
				index = atoi(szBuf);

				GetToken(fp, szBuf);
				start = atof(szBuf);

				GetToken(fp, szBuf);
				end = atof(szBuf);

				GetToken(fp, szBuf);
				isLoop = (bool)atoi(szBuf);

				cAnimComponent* animComp = pObject->GetComponent<cAnimComponent*>(eANIMATION_TYPE);
				animComp->AddClip(name, index, start, end, isLoop);
			}
		}
	}
	void cPrefabParser::AttachBipNameLoad(FILE *fp, GameObject* pParentObject, GameObject* pObject, int childLevel)
	{
		char szBuf[1024];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);
			if (szBuf[0] == '{'){//시작 괄호
				char name[256];
				GetToken(fp, szBuf);
				strcpy(name, szBuf);
				pParentObject->AttachObjectToFrame(name, pObject);
			}
			else if (szBuf[0] == '}'){ //종료 괄호
				break;
			}
		}
	}
	void cPrefabParser::ScriptComponentLoad(FILE *fp, GameObject* pObject, int childLevel)
	{
		cScriptComponent* pScriptComponent = NULL;

		char szBuf[1024];
		char scriptPath[256];

		while (!feof(fp))
		{
			GetToken(fp, szBuf);
			if (szBuf[0] == '{'){//시작 괄호
			}
			else if (szBuf[0] == '}'){ //종료 괄호
				break;
			}
			else if (strcmp("ScriptPath", szBuf) == 0){
				GetToken(fp, szBuf);
				strcpy(scriptPath, szBuf);
			}
		}

		//애니 컴포넌트 추가
		pScriptComponent = new cScriptComponent(pObject, scriptPath);
		pObject->AddComponent(pScriptComponent);
	}


	char* cPrefabParser::GetToken(FILE *fp, char* strValue)
	{
		int		nCnt = 0;
		bool	isQuote = false;
		memset(strValue,0,1024);

		while (!feof(fp))
		{
			char c = fgetc(fp);

			if (c < 33 && !isQuote)	// white space
			{
				if (nCnt > 0) break;
				else continue;
			}

			if (c == '\"')
			{
				if (isQuote) break;
				else
				{
					isQuote = true;
					continue;
				}
			}

			strValue[nCnt++] = c;
		}

		if (nCnt == 0)
			return NULL;

		strValue[nCnt] = '\0';
		return strValue;
	}
	char* cPrefabParser::TagToChar(GAMEOBJECT_TAG tag)
	{
		if (tag == GAMEOBJECT_TAG::TAG_STATIC)
			return "TAG_STATIC";
		else if (tag == GAMEOBJECT_TAG::TAG_PLAYER)
			return "TAG_PLAYER";
		else if (tag == GAMEOBJECT_TAG::TAG_NPC)
			return "TAG_NPC";
		else if (tag == GAMEOBJECT_TAG::TAG_ENEMY)
			return "TAG_ENEMY";
		else if (tag == GAMEOBJECT_TAG::TAG_WEAPON)
			return "TAG_WEAPON";
		else if (tag == GAMEOBJECT_TAG::TAG_LIGHT)
			return "TAG_LIGHT";
		else if (tag == GAMEOBJECT_TAG::TAG_CAMERA)
			return "TAG_CAMERA";
		else if (tag == GAMEOBJECT_TAG::TAG_TERRAIN)
			return "TAG_TERRAIN";
		else if (tag == GAMEOBJECT_TAG::TAG_SKYBOX)
			return "TAG_SKYBOX";
		else if (tag == GAMEOBJECT_TAG::TAG_BILLBOARD)
			return "TAG_BILLBOARD";
		else if (tag == GAMEOBJECT_TAG::TAG_PARTICLE)
			return "TAG_PARTICLE";
		else
			return "TAG_ETC";
	}
	GAMEOBJECT_TAG cPrefabParser::CharToTag(char* tagName)
	{
		if (strcmp(tagName, "TAG_STATIC") == 0)
			return GAMEOBJECT_TAG::TAG_STATIC;
		else if (strcmp(tagName, "TAG_PLAYER") == 0)
			return GAMEOBJECT_TAG::TAG_PLAYER;
		else if (strcmp(tagName, "TAG_NPC") == 0)
			return GAMEOBJECT_TAG::TAG_NPC;
		else if (strcmp(tagName, "TAG_ENEMY") == 0)
			return GAMEOBJECT_TAG::TAG_ENEMY;
		else if (strcmp(tagName, "TAG_WEAPON") == 0)
			return GAMEOBJECT_TAG::TAG_WEAPON;
		else if (strcmp(tagName, "TAG_LIGHT") == 0)
			return GAMEOBJECT_TAG::TAG_LIGHT;
		else if (strcmp(tagName, "TAG_CAMERA") == 0)
			return GAMEOBJECT_TAG::TAG_CAMERA;
		else if (strcmp(tagName, "TAG_TERRAIN") == 0)
			return GAMEOBJECT_TAG::TAG_TERRAIN;
		else if (strcmp(tagName, "TAG_SKYBOX") == 0)
			return GAMEOBJECT_TAG::TAG_SKYBOX;
		else if (strcmp(tagName, "TAG_BILLBOARD") == 0)
			return GAMEOBJECT_TAG::TAG_BILLBOARD;
		else if (strcmp(tagName, "TAG_PARTICLE") == 0)
			return GAMEOBJECT_TAG::TAG_PARTICLE;
		else
			return GAMEOBJECT_TAG::TAG_ETC;
	}
}
