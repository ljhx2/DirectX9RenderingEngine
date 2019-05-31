#include "Common.h"
#include "include/D3DRenderer.h"
#include "cSkinnedMesh.h"
#include "cStaticMeshComponent.h"
#include "cSkinnedMeshComponent.h"
#include "cFbxComponent.h"
#include "cAnimComponent.h"
#include "cColliderComponent.h"
#include "cTerrainComponent.h"
#include "cSkyBoxComponent.h"
#include "cScriptComponent.h"
#include "cLevelParser.h"

namespace UTIL
{
	std::string& WStringToString(const std::wstring& in, std::string& out)
	{
		out.clear();

		size_t inSize = in.size();
		for (size_t i = 0; i < inSize; i++)
		{
			out.insert(out.end(), 1, static_cast<const char>(in.at(i) & 0xFF));
		}
		return out;
	}

	std::wstring& StringToWString(const std::string& in, std::wstring& out)
	{
		out.clear();

		size_t inSize = in.size();
		for (size_t i = 0; i < inSize; i++)
		{
			out.insert(out.end(), 1, static_cast<const wchar_t>(in.at(i)));
		}
		return out;
	}
}

namespace ENGINE
{
	void Start()
	{
		//SceneRoot 는 한놈만
		if (g_pSceneRoot == nullptr)
			g_pSceneRoot = new GameObject(0, L"__SCENE_ROOT__");
		else
			assert(!"SceneRoot가 두번 초기화 됨.");
		
	}

	void Shutdown()
	{
		//1. 재귀로 돌면서 childe object의 Component들 먼저 모두 해제
		g_pSceneRoot->DestroyComponents();
		
		//2. 씬 root 의 child object 모두 해제
		g_pSceneRoot->DestroyAllChildObjects();

		if (g_pSceneRoot != nullptr)
			delete g_pSceneRoot;

		
		g_pTextureManager->Destroy();

		g_pSkinnedMeshManager->Destroy();

		g_pStaticMeshManager->Destroy();

		g_pFbxManager->Destroy();

		g_pShaderManager->Destroy();

		g_pShadowManager->Destroy();


		DeviceMgr::GetInstance().Destroy();
	}

	

	void DeleteGameObject(GameObject* obj)
	{
		if (obj == NULL) return;

		GameObject* parent = obj->GetParent();
		if (parent)
			parent->SubChild(obj);
		else
			g_pSceneRoot->SubChild(obj);

		obj->DestroyComponents();
		obj->DestroyAllChildObjects();

		std::multimap<std::wstring, ENGINE::GameObject*>::iterator iter;
		for (iter = g_mulmapEnableObject.begin(); iter != g_mulmapEnableObject.end(); ++iter)
		{
			if (iter->second == obj)
			{
				g_mulmapEnableObject.erase(iter);
				break;
			}
		}
		SAFE_DELETE(obj);
	}

	//void DeleteActorManager(ActorManager* actorManager)
	//{
	//	if (actorManager != nullptr)
	//		delete actorManager;
	//}

	/*GameObject* Instantiate(const std::wstring & prefabPath)
	{

		GameObject* obj = new GameObject(prefabPath);
		obj->LoadPrefab(prefabPath);
		g_pSceneRoot->AddChild(obj);

		return obj;
	}*/

	void SceneSave(const std::string& scenebPath)
	{
		cSceneParser parser;
		char temp[256];
		strcpy(temp, scenebPath.c_str());
		parser.SaveScene(temp);
	}


	GameObject* SceneLoad(const std::string& scenebPath)
	{
		cSceneParser parser;
		char temp[256];
		strcpy(temp, scenebPath.c_str());

		parser.LoadScene(temp);
		
		return  g_pSceneRoot;
	}


	GameObject* Instantiate(const std::string& prefabPath, D3DXVECTOR3 pos)
	{
		cPrefabParser parser;
		char temp[256];
		strcpy(temp, prefabPath.c_str());

		GameObject* obj = parser.LoadPrefab(temp, pos);

		obj->SetParent(g_pSceneRoot);

		return  obj;
	}

	GameObject* InstantiateSkinnedMesh(const std::string & fileName)
	{
		//std::string=>std::wstring
		std::wstring objName = L"";
		objName.assign(fileName.begin(), fileName.end());

		//GameObejct 의 컴포넌트로 추가 하고
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());
		cSkinnedMeshComponent* pSkinnedMeshComponent = new cSkinnedMeshComponent(obj, "Prefabs/", fileName.c_str());
		cAnimComponent* pAnimComp = new cAnimComponent(obj);
		
		obj->AddComponent(pSkinnedMeshComponent);
		obj->AddComponent(pAnimComp);

		//콜라이더 컴포넌트
		cColliderComponent* pColliderComp = new cColliderComponent(obj);
		obj->AddComponent(pColliderComp);

		//씬 루트에 추가 한다.
		obj->SetParent(g_pSceneRoot);
		g_mulmapEnableObject.insert(std::make_pair(objName, obj));

		return obj;
	}
	GameObject* InstantiateStaticMesh(const std::string & fileName)
	{
		//std::string=>std::wstring
		std::wstring objName = L"";
		objName.assign(fileName.begin(), fileName.end());

		//GameObejct 의 컴포넌트로 추가 하고
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());
		//staticMesh 로드
		cStaticMeshComponent* pStaticMesh = new cStaticMeshComponent(obj, "Prefabs/", fileName.c_str());

		obj->AddComponent(pStaticMesh);

		//콜라이더 컴포넌트
		cColliderComponent* pColliderComp = new cColliderComponent(obj);
		obj->AddComponent(pColliderComp);

		//씬 루투에 추가 한다
		obj->SetParent(g_pSceneRoot);
		g_mulmapEnableObject.insert(std::make_pair(objName, obj));

		return obj;
	}
	GameObject* InstantiateFbx(const std::string & fileName)
	{
		//std::string=>std::wstring
		std::wstring objName = L"";
		objName.assign(fileName.begin(), fileName.end());

		//GameObejct 의 컴포넌트로 추가 하고
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());
		cFbxComponent* pFbxComponent = new cFbxComponent(obj, "Prefabs/", fileName.c_str());
		cAnimComponent* pAnimComp = new cAnimComponent(obj);

		obj->AddComponent(pFbxComponent);
		obj->AddComponent(pAnimComp);

		//콜라이더 컴포넌트
		cColliderComponent* pColliderComp = new cColliderComponent(obj);
		obj->AddComponent(pColliderComp);

		//씬 루트에 추가 한다.
		obj->SetParent(g_pSceneRoot);
		g_mulmapEnableObject.insert(std::make_pair(objName, obj));

		return obj;
	}

	GameObject* InstantiateTerrain(const std::string & FileName)
	{
		return InstantiateTerrain(FileName, FileName);
	}
	//SY : 나중을 위해서 그냥 두겠음.
	GameObject* InstantiateTerrain(const std::string & HeightMapFileName, const std::string & TextureFileName)
	{
		char szHeightMap[256];
		char szTexture[256];
		szHeightMap[0] = 0;
		szTexture[0] = 0;

		strcpy(szHeightMap, "Terrain/");
		strcat(szHeightMap, HeightMapFileName.c_str());

		strcpy(szTexture, "Terrain/");
		strcat(szTexture, TextureFileName.c_str());
		std::string texFileName(szTexture);
		texFileName.replace(texFileName.size() - 3, 3, "png");

		//GameObject생성
		std::wstring objName = L"";
		objName.assign(HeightMapFileName.begin(), HeightMapFileName.end());
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());

		//터레인컴포넌트생성
		cTerrainComponent* pTerrain = new cTerrainComponent(obj);
		pTerrain->Setup(szHeightMap, texFileName.c_str());

		obj->AddComponent(pTerrain);

		//씬 루트에 추가 한다.
		g_pSceneRoot->AddChild(obj);
		g_mulmapEnableObject.insert(std::make_pair(objName, obj));

		return obj;
	}
	GameObject* InstantiateSkyBox(const std::string & fileName)
	{
		char szPath[256];
		szPath[0] = 0;
		
		strcpy(szPath, "SkyBox/");
		strcat(szPath, fileName.c_str());

		//GameObject생성
		std::wstring objName = L"";
		objName.assign(fileName.begin(), fileName.end());
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());

		//skybox컴포넌트생성
		cSkyBoxComponent* pSkybox = new cSkyBoxComponent(obj, szPath);
		
		obj->AddComponent(pSkybox);

		//씬 루트에 추가
		g_pSceneRoot->AddChild(obj);

		return obj;
	}
	GameObject* InstantiateSkyBox(const std::string & top, const std::string & bottom, const std::string & left, const std::string & right, const std::string & front, const std::string & back)
	{
		char szPath[6][256] = {0,};

		strcpy(szPath[0], "SkyBox/");
		strcat(szPath[0], top.c_str());
		strcpy(szPath[1], "SkyBox/");
		strcat(szPath[1], bottom.c_str());
		strcpy(szPath[2], "SkyBox/");
		strcat(szPath[2], left.c_str());
		strcpy(szPath[3], "SkyBox/");
		strcat(szPath[3], right.c_str());
		strcpy(szPath[4], "SkyBox/");
		strcat(szPath[4], front.c_str());
		strcpy(szPath[5], "SkyBox/");
		strcat(szPath[5], back.c_str());

		//GameObject생성
		std::wstring objName = L"SkyBox";
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());

		//skybox컴포넌트생성
		cSkyBoxComponent* pSkybox = new cSkyBoxComponent(obj, szPath[0], szPath[1], szPath[2], szPath[3], szPath[4], szPath[5]);

		obj->AddComponent(pSkybox);

		//씬 루트에 추가
		g_pSceneRoot->AddChild(obj);

		return obj;

	}

	GameObject* InstantiateLight(ELIGHT_TYPE type, D3DXVECTOR3 pos, D3DXCOLOR color)
	{
		static int count = 1;
		TCHAR szName[256];
		std::wstring objName;
		if (type == eLIGHT_DIRECTIONAL)
		{
			wsprintf(szName, L"DirectionalLight%d", count++);
			objName = szName;
		}
		else if (type == eLIGHT_POINT)
		{
			wsprintf(szName, L"PointLight%d", count++);
			objName = szName;
		}
		
		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());

		//Light컴포넌트 생성
		cLightComponent* pLight = new cLightComponent(obj);
		obj->SetTranslate(pos.x, pos.y, pos.z);
		pLight->SetDiffuseColor(color);

		obj->AddComponent(pLight);

		//씬 루트에 추가
		g_pSceneRoot->AddChild(obj);

		return obj;
	}

	GameObject* InstantiateCamera(HWND hWnd)
	{
		std::wstring objName = L"Camera";

		GameObject* obj = new GameObject(g_pUIDManager->CreateUID(), objName.c_str());

		//camera컴포넌트 생성
		cCameraComponent* pCamera = new cCameraComponent(obj, hWnd);
		obj->AddComponent(pCamera);

		//씬 루트에 추가
		g_pSceneRoot->AddChild(obj);

		return obj;
	}

	void LoadLevel(const std::string & fileName, OUT multiMapGameObjects& outputMultiMap)
	{
		std::string filePath("Prefabs/");
		filePath += fileName;
		cLevelParser levelPaser;
		levelPaser.Load(filePath.c_str(), outputMultiMap);
	}

	bool CheckCollision(COLLISION_SPHERE a, COLLISION_SPHERE b)
	{
		float distance = D3DXVec3Length(&(a.center - b.center));
	
		if (distance <= a.radius + b.radius)
		{
			return true;
		}
	
		return false;
	}
	bool CheckCollisionBox(cCollisionBox & a, cCollisionBox & b)
	{
		return a.CheckCollision(b);
	}
	bool CheckCollisionBox(cCollisionBox* a, cCollisionBox* b)
	{
		return CheckCollisionBox(*a, *b);
	}

			//픽킹관련 내부함수--------------------
	ST_PickingRay CalcPickingRay(int screenX, int screenY)
	{
		float px = 0.0f;
		float py = 0.0f;

		D3DVIEWPORT9 vp;
		g_pD3DDevice->GetViewport(&vp);

		D3DXMATRIX proj;
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

		px = (((2.0f * screenX) / vp.Width) - 1.0f) / proj(0, 0);
		py = (((-2.0f * screenY) / vp.Height) + 1.0f) / proj(1, 1);

		ST_PickingRay ray;
		ray.origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ray.direction = D3DXVECTOR3(px, py, 1.0f);

		return ray;
	}
	void TransformRay(ST_PickingRay* ray, D3DXMATRIX* T)
	{
		D3DXVec3TransformCoord(&ray->origin, &ray->origin, T);
		D3DXVec3TransformNormal(&ray->direction, &ray->direction, T);

		D3DXVec3Normalize(&ray->direction, &ray->direction);
	}
	bool IntersectionRaySphere(ST_PickingRay* ray, COLLISION_SPHERE* sphere)
	{
		D3DXVECTOR3 v = ray->origin - sphere->center;

		float b = 2.0f * D3DXVec3Dot(&ray->direction, &v);
		float c = D3DXVec3Dot(&v, &v) - (sphere->radius * sphere->radius);

		//find the discriminant
		float discriminant = (b * b) - (4.0f * c);

		//test for imaginary number
		if (discriminant < 0.0f) 
			return false;

		discriminant = sqrtf(discriminant);

		float s0 = (-b + discriminant) / 2.0f;
		float s1 = (-b - discriminant) / 2.0f;

		// if a solution is >= 0, then we intersected the sphere
		if (s0 >= 0.0f || s1 >= 0.0f)
			return true;

		return false;
	}
			//----------------픽킹관련 내부함수 end
	bool CheckPicking(int screenX, int screenY, GameObject* obj)
	{
		/*
		cTerrainComponent* terain = obj->GetComponent<cTerrainComponent*>(eTERRAIN_TYPE);
		if (terain != NULL)
		{
			POINT pt;
			pt.x = screenX;
			pt.y = screenY;
			terain->SetMouse(pt);
			return true;
		}
		*/
		/*
		cFbxComponent* fbx = obj->GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);
		cSkinnedMeshComponent* x = obj->GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
		cStaticMeshComponent* staticMesh = obj->GetComponent<cStaticMeshComponent*>(eSTATIC_MESH_TYPE);

		if (!fbx && !x && !staticMesh) return false;

		D3DXVECTOR3 vMin, vMax;

		if (fbx != NULL)
		{
			vMin = fbx->GetMinPoint();
			vMax = fbx->GetMaxPoint();
		}
		else if (x != NULL)
		{
			vMin = x->GetMinPoint();
			vMax = x->GetMaxPoint();
		}
		else if (staticMesh != NULL)
		{
			vMin = staticMesh->GetMinPoint();
			vMax = staticMesh->GetMaxPoint();
		}


		//클릭한 스크린 포인트를 이용해 뷰 스페이스 내에 광선을 계산한다.
		ST_PickingRay ray = CalcPickingRay(screenX, screenY);

		//광선을 월드 스페이스로 변환한다.
		D3DXMATRIX view;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
		
		D3DXMATRIX viewInverse;
		D3DXMatrixInverse(&viewInverse, 0, &view);

		TransformRay(&ray, &viewInverse);

		
		D3DXVec3TransformCoord(&vMin, &vMin, &obj->GetWorldMatrix());
		D3DXVec3TransformCoord(&vMax, &vMax, &obj->GetWorldMatrix());

		D3DXVECTOR3 center = (vMax + vMin) / 2.0f;
		float radius = D3DXVec3Length(&(vMax - vMin)) / 2.0f;

		COLLISION_SPHERE sphere = COLLISION_SPHERE(center, radius);

		//맞추었는지를 테스트한다.
		if (IntersectionRaySphere(&ray, &sphere))
		{
			cSkinnedMeshComponent* skinnedComp = obj->GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
			cStaticMeshComponent* staticComp = obj->GetComponent<cStaticMeshComponent*>(eSTATIC_MESH_TYPE);
			cFbxComponent* fbxComp = obj->GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);
			if (skinnedComp != NULL)
			{
				if (skinnedComp->IntersectRayMesh(ray.origin, ray.direction))
				{
					return true;
				}
			}
			else if (staticComp != NULL)
			{
				if (staticComp->IntersectRayMesh(ray.origin, ray.direction))
				{
					return true;
				}
			}
			else if (fbxComp != NULL)
			{
				if (fbxComp->IntersectRayMesh(ray.origin, ray.direction))
				{
					return true;
				}
			}
		}
		*/
		return false;
	}

	GameObject* GetSceneRoot(){ return g_pSceneRoot; }
	

	bool D3DRenderer::BeginFrame(){

		g_pTimeManager->Update();

		g_pCollisionManager->CollisionUpdate();

		/////
		g_pShadowManager->Update();
		////

		g_pSceneRoot->Update(g_pTimeManager->GetDeltaTime());

		g_pDamageNumberManager->Update();

		g_pD3DDevice->Clear(
			0,
			0,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(100, 100, 100),
			1.0f,
			0);

		if (g_pD3DDevice->BeginScene() != S_OK)
			return false;

		//////
		g_pShadowManager->RenderShadowMap(g_pD3DDevice);
		/////

		LPDIRECT3DTEXTURE9 shadowMap[3];
		for (int i = 0; i < 3; i++)
		{
			shadowMap[i] = g_pShadowManager->GetShadowMap(i);
		}
		g_pShaderManager->SetShadowMap(shadowMap[0], shadowMap[1], shadowMap[2]);

		g_pSceneRoot->SetIsRender(true);
		g_pSceneRoot->Render();

		
		g_pDamageNumberManager->Render(g_pCameraManager->GetCurrentCamera());

		//////
		//g_pShadowManager->ShadowMapRender(g_pD3DDevice);
		/////

		return true;
	}

	void D3DRenderer::EndFrame(){

		g_pD3DDevice->EndScene();
	}

	void D3DRenderer::DisplayFrame(){
		g_pD3DDevice->Present(0, 0, 0, 0);
	}

	void D3DRenderer::CreateDevice(HWND hWnd){
		DeviceMgr::GetInstance().CreateDevice(hWnd);

		g_pShadowManager->CreateShadowMap(g_pD3DDevice);

	}


	//------------------------------------------------------
	//GameObject
	//-------------------------------------------------------
	GameObject::GameObject(UniqueID id, std::wstring objName){
		m_id = id;
		_name = objName;
		isEnable = true;
		m_objectParent = NULL;
		memset(m_AttachBipName, 0, 256);

		funcPtr_OnCollisionEnter = NULL;
		funcPtr_OnCollisionStay = NULL;
		funcPtr_OnCollisionExit = NULL;

		m_tag = TAG_STATIC;

		_transform = new Transform(this);

		g_pGameObjectManager->AddGameObject(this);
	}
	GameObject::~GameObject()
	{
		g_pGameObjectManager->EraseGameObject(m_id);
	}

	void GameObject::SetName(const char* name)
	{
		//g_mulmapEnableObject의 기존 이름으로 key가 되있던걸 지운다
		typedef std::multimap<std::wstring, GameObject*>::iterator mulmapEnableIter;
		std::pair<mulmapEnableIter, mulmapEnableIter> iter_pair;
		iter_pair = g_mulmapEnableObject.equal_range(_name);
		mulmapEnableIter iter;
		for (iter = iter_pair.first; iter != iter_pair.second; ++iter)
		{
			if (iter->second == this)
			{
				g_mulmapEnableObject.erase(iter);
				break;
			}
		}
		//새롭게 이름을 정해주고 g_mulmapEnableObject에 새롭게 넣어준다.
		_name = UTIL::CharToTCHAR(name);
		g_mulmapEnableObject.insert(std::make_pair(UTIL::CharToTCHAR(name), this));
	}
	void GameObject::DestroyComponents()
	{
		for each(auto p in m_vecComponent)
		{
			_SAFE_DELETE(p);
		}

		for each(auto p in m_childGameObjects)
		{
			p->DestroyComponents();
		}
	} 
	void GameObject::DestroyAllChildObjects()
	{
		for each(auto p in m_childGameObjects)
		{
			std::multimap<std::wstring, ENGINE::GameObject*>::iterator iter;
			for (iter = g_mulmapEnableObject.begin(); iter != g_mulmapEnableObject.end(); ++iter)
			{
				if (iter->second == p)
				{
					g_mulmapEnableObject.erase(iter);
					break;
				}
			}
			_SAFE_DELETE(p);
		}
		m_childGameObjects.clear();

	}

	GameObject* GameObject::FindObject(std::wstring objName)
	{
		std::multimap<std::wstring, GameObject*>::iterator iter;
		iter = g_mulmapEnableObject.find(objName);
		if (iter != g_mulmapEnableObject.end())
		{
			return iter->second;
		}
		return NULL;
	}
	void GameObject::FindObjects(IN std::wstring objName, OUT std::vector<GameObject*>& outputVector)
	{
		typedef std::multimap<std::wstring, GameObject*>::iterator mulmapEnableIter;
		std::pair<mulmapEnableIter, mulmapEnableIter> iter_pair;
		iter_pair = g_mulmapEnableObject.equal_range(objName);
		mulmapEnableIter iter;
		for (iter = iter_pair.first; iter != iter_pair.second; ++iter)
		{
			outputVector.push_back(iter->second);
		}
	}

	void GameObject::OnDisable()
	{
		if (!isEnable) return;

		isEnable = false;

		//g_mulmapEnableObject에서 같은 이름을 가진것들을 추린다
		typedef std::multimap<std::wstring, GameObject*>::iterator mulmapEnableIter;
		std::pair<mulmapEnableIter, mulmapEnableIter> iter_pair;
		iter_pair = g_mulmapEnableObject.equal_range(_name);
		//위코드에서 추린 포인터들을 돌면서 자신을 찾으면 g_mulmapEnableObject에서 뺀다
		mulmapEnableIter iter;
		for (iter = iter_pair.first; iter != iter_pair.second; ++iter)
		{
			if (this == iter->second)
			{
				g_mulmapEnableObject.erase(iter);
				break;
			}
		}
		//자식GameObject들도 모두 Disable한다
		for each(auto p in m_childGameObjects)
		{
			p->OnDisable();
		}
	}
	void GameObject::OnEnable()
	{
		if (isEnable) return;

		isEnable = true;
		g_mulmapEnableObject.insert(std::make_pair(this->_name, this));

		for each(auto p in m_childGameObjects)
		{
			p->OnEnable();
		}
	}
	//skinnedmesh만 사용가능한 함수
	void GameObject::AttachObjectToFrame(const char* frameName, GameObject* obj)
	{
		cSkinnedMeshComponent* skinnedComp = GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
		cFbxComponent* fbxComp = GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);
		if (skinnedComp != NULL)
		{
			obj->SetParent(this);
			D3DXMATRIXA16* pMat = skinnedComp->GetMatrixFromeFrame(frameName);
			obj->_transform->pParentWorldMatrix = pMat;
			strcpy(obj->m_AttachBipName, frameName);
		}
		else if (fbxComp != NULL)
		{
			obj->SetParent(this);
			D3DXMATRIXA16* pMat = fbxComp->GetMatrixFromeFrame(frameName);
			obj->_transform->pParentWorldMatrix = pMat;
			strcpy(obj->m_AttachBipName, frameName);
		}
	}
	//------------------------------GameObject


}//namespace ENGINE

ENGINE::GameObject* g_pSceneRoot = nullptr;

std::multimap<std::wstring, ENGINE::GameObject*> g_mulmapEnableObject;
