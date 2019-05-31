#pragma once
#include "DeviceMgr.h"

//Ŭ���̾�Ʈ���� component���� ������ �Լ����� ����Ϸ���
//�� component���� ��������� include ������� ?
#include "../cStaticMeshComponent.h"
#include "../cSkinnedMeshComponent.h"
#include "../cFbxComponent.h"
#include "../cAnimComponent.h"
#include "../cColliderComponent.h"
#include "../cCollisionBox.h"
#include "../cCollisionSphere.h"
#include "../cTerrainComponent.h"
#include "../cSkyBoxComponent.h"
#include "../cLightComponent.h"
#include "../cCameraComponent.h"
#include "../cScriptComponent.h"

#include "../cSkinnedMeshManager.h"
#include "../cFbxManager.h"
#include "../cStaticMeshManager.h"
#include "../cPrefabParser.h"
#include "../cTerrainMnager.h" //SY �߰� 
#include "../cCameraManager.h"
#include "../cSceneParser.h"
#include "../cWorldMapManager.h"
#include "../cDamageNumberManager.h"
#include "../cShadowManager.h"

namespace UTIL
{
	// Char �� TCHAR
	inline TCHAR* CharToTCHAR(const char* pstrSrc)
	{
		size_t nLen = strlen(pstrSrc) + 1;
		TCHAR * pwstr = (LPWSTR)malloc(sizeof(TCHAR)*nLen);
		size_t con = 0;
		memset(pwstr, 0, sizeof(pwstr));

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pstrSrc, nLen, pwstr, nLen);
		return pwstr;
	}

	// TCHAR �� Char
	inline char * TCHARToChar(const TCHAR* pstrSrc)
	{
		size_t nLen = lstrlen(pstrSrc) + 1;
		char * pstr = new char[sizeof(char)* nLen];
		WideCharToMultiByte(CP_ACP, 0, pstrSrc, nLen, pstr, nLen, NULL, NULL);

		return pstr;
	}

	inline wchar_t* ConstWcharToWchar(const wchar_t* pstrSrc)
	{
		size_t nLen = lstrlen(pstrSrc) + 1;
		TCHAR * pwstr = (LPWSTR)malloc(sizeof(TCHAR)*nLen);
		size_t con = 0;
		memset(pwstr, 0, sizeof(pwstr));
		lstrcpyW(pwstr, pstrSrc);
		return pwstr;
	}

	std::string& WStringToString(const std::wstring& in, std::string& out);

	std::wstring& StringToWString(const std::string& in, std::wstring& out);
}

//Collision���� �ݹ��Լ� �� ����
typedef void(*MY_CALLBACK)(ENGINE::cCollisionShape*, ENGINE::cCollisionShape*);


namespace ENGINE
{

	//-------------------------------------------------
	// ���� �Լ���
	//-------------------------------------------------
class GameObject;


typedef std::multimap<std::string, GameObject*> multiMapGameObjects;


void Start();
void Shutdown();
void DeleteGameObject(GameObject* obj);

void		SceneSave(const std::string& scenebPath);
GameObject* SceneLoad(const std::string& scenebPath);
GameObject* Instantiate(const std::string& prefabPath, D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0));
GameObject* GetSceneRoot();
GameObject* InstantiateSkinnedMesh(const std::string & fileName);
GameObject* InstantiateStaticMesh(const std::string & fileName);
GameObject* InstantiateFbx(const std::string & fileName);
GameObject* InstantiateSkyBox(const std::string & fileName);
GameObject* InstantiateSkyBox(const std::string & top, const std::string & bottom, const std::string & left, const std::string & right, const std::string & front, const std::string & back);
//�ͷ��� 
GameObject* InstantiateTerrain(const std::string & FileName); //  RAW
GameObject* InstantiateTerrain(const std::string & HeightMapFileName, const std::string & TextureFileName);

//Light
GameObject* InstantiateLight(ELIGHT_TYPE type, D3DXVECTOR3 pos, D3DXCOLOR color);
//Camera
GameObject* InstantiateCamera(HWND hWnd);

void		LoadLevel(IN const std::string & fileName, OUT multiMapGameObjects& outputMultiMap);

//�浹����
bool		CheckCollision(COLLISION_SPHERE a, COLLISION_SPHERE b);	//�浹üũ
bool		CheckCollisionBox(cCollisionBox & a, cCollisionBox & b);
bool		CheckCollisionBox(cCollisionBox* a, cCollisionBox* b);
bool		CheckCollisionBox(cCollisionBox & a, cCollisionBox & b);
//��ŷ����
bool		CheckPicking(int screenX, int screenY, GameObject* obj);



class Transform
{
	friend class GameObject;
private:
	typedef struct tagLINEVERTEX{
		D3DXVECTOR3 pos;
		DWORD color;
		enum{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

	}LINEVERTEX, *LPLINEVERTEX;

public:
	Transform(GameObject* obj){
		localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		localRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		localRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		localUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		localFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		pParentWorldMatrix = NULL;

		m_pOwnerGameObject = obj;
	}
	virtual ~Transform(){}

	virtual void Update(float deltaTime)
	{
		D3DXMATRIXA16 matScale, matRot, matTrans;
		D3DXMatrixScaling(&matScale, localScale.x, localScale.y, localScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRot, localRotation.y, localRotation.x, localRotation.z);

		D3DXVec3TransformNormal(&localRight, &D3DXVECTOR3(1, 0.0f, 0.0f), &matRot);
		D3DXVec3TransformNormal(&localUp, &D3DXVECTOR3(0.0f, 1, 0.0f), &matRot);
		D3DXVec3TransformNormal(&localFront, &D3DXVECTOR3(0.0f, 0.0f, 1), &matRot);

		D3DXMatrixTranslation(&matTrans, localPosition.x, localPosition.y, localPosition.z);
		m_matWorld = matScale * matRot * matTrans;
		if (pParentWorldMatrix != NULL)
		{
			m_matWorld = m_matWorld * (*pParentWorldMatrix);
		}
	}
	virtual void Render(){}

	void RenderGizmo(LPDIRECT3DDEVICE9 pDevice)
	{
		//���� ����̽��� ���õǾ��ִ� ��������� ����ϰ�
		//����� �� �׸��� ����س��Ҵ� ���� ��ķ� �ǵ�����.
		D3DXMATRIXA16 matPrevWorld;
		pDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);		//���� ��ƺ񽺿� ���õǾ� �ִ� ��������� matPrevWorld �� ����

		//���� ���� ���ð� ���
		DWORD pPrevLighting;
		pDevice->GetRenderState(D3DRS_LIGHTING, &pPrevLighting);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


		LINEVERTEX v[6];
		v[0].pos = D3DXVECTOR3(0, 0, 0);
		v[1].pos = D3DXVECTOR3(0, 0, 10);
		v[0].color = 0xff0000ff;
		v[1].color = 0xff0000ff;


		v[2].pos = D3DXVECTOR3(0, 0, 0);
		v[3].pos = D3DXVECTOR3(0, 10, 0);
		v[2].color = 0xff00ff00;
		v[3].color = 0xff00ff00;

		v[4].pos = D3DXVECTOR3(0, 0, 0);
		v[5].pos = D3DXVECTOR3(10, 0, 0);
		v[4].color = 0xffff0000;
		v[5].color = 0xffff0000;


		pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

		pDevice->SetFVF(LINEVERTEX::FVF);
		pDevice->DrawPrimitiveUP(D3DPT_LINELIST,
			3, v, sizeof(LINEVERTEX));


		//�ٽ� ���� �������� �ǵ�����.
		pDevice->SetRenderState(D3DRS_LIGHTING, pPrevLighting);

		//�ٽ� ���� ���� ��ķ� �ǵ�����.
		pDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);
	}
	

	//Transform* m_parent;
	D3DXMATRIXA16* pParentWorldMatrix;

	//�̵�, ȸ��, �����ϸ�
	void Translation_World(float dx, float dy, float dz) //���� �������� �̵�
	{
		localPosition.x += dx;
		localPosition.y += dy;
		localPosition.z += dz;
	}
	void Translation_Self(float right_dx, float up_dy, float front_dz) //�ڽ��� �������� �̵�
	{
		localPosition += localRight * right_dx;
		localPosition += localUp * up_dy;
		localPosition += localFront * front_dz;
	}
	void Scaling(float dx, float dy, float dz)	//������ ���� ��ȭ
	{
		localScale.x += dx;
		localScale.y += dy;
		localScale.z += dz;
	}
	void Rotate_World(float dx, float dy, float dz)	//x,y,z�� ��ŭ ������ �������� ȸ��
	{
		localRotation.x += dx;
		localRotation.y += dy;
		localRotation.z += dz;
	}

	D3DXVECTOR3 GetLocalFront(){ return localFront; }
	D3DXVECTOR3 GetLocalRotation(){ return localRotation; }
	void SetLocalRotation(D3DXVECTOR3 localRot){ localRotation = localRot; }
protected:
	GameObject* m_pOwnerGameObject;

	D3DXVECTOR3 localPosition;
	D3DXVECTOR3 localRotation;
	D3DXVECTOR3 localScale;
	D3DXMATRIXA16 m_matWorld;

	D3DXVECTOR3 localRight;
	D3DXVECTOR3 localUp;
	D3DXVECTOR3 localFront;
};

enum GAMEOBJECT_TAG
{
	TAG_STATIC = 0,	//�� �����̴� �� ������
	TAG_PLAYER,		
	TAG_NPC,
	TAG_ENEMY,
	TAG_WEAPON,
	TAG_LIGHT,
	TAG_CAMERA,
	TAG_TERRAIN,
	TAG_SKYBOX,
	TAG_BILLBOARD,
	TAG_PARTICLE,
	TAG_ETC
};
//-------------------------------------------------
//�� �׷����� Node ��� �����ϸ� ��.
class GameObject
{
public:
	typedef unsigned int UniqueID;

public:
	GameObject(UniqueID id, std::wstring objName);
	virtual ~GameObject();
		

	virtual void Update(float deltaTime)
	{	
		if (!isEnable) return;

		_transform->Update(deltaTime);

		for each(auto p in m_vecComponent)
		{
			p->Update(deltaTime, &(_transform->m_matWorld));
		}
		for each(auto p in m_childGameObjects)
		{
			p->Update(deltaTime);
		}
	}
	virtual void PrevRender()
	{
		if (!isEnable) return;

		for each(auto p in m_vecComponent)
		{
			p->PrevRender();
		}
		//for each(auto p in m_childGameObjects)
		//{
		//	p->PrevRender();
		//}
	}
	virtual void Render()
	{
		if (!isEnable) return;
		//if (!isRender) return;
		
		for each(auto p in m_vecComponent)
		{
			p->Render();
		}
		for each(auto p in m_childGameObjects)
		{
			p->Render();
		}

		isRender = false;
	}
	void AddComponent(IComponent* component)
	{
		m_vecComponent.push_back(component);
	}
	template<typename T>
	T GetComponent(ECOMPONENT_TYPE type)
	{
		for each(auto p in m_vecComponent)
		{
			
			if (p->GetType() == type)
			{
				return (T)p;
			}
		}
		return NULL;
	}
	int GetComponentSize()
	{
		return m_vecComponent.size();
	}
	IComponent* GetCompoent(int i)
	{
		return m_vecComponent[i];
	}

	UniqueID GetID() { return m_id; }
	GAMEOBJECT_TAG GetGameObjectTag(){ return m_tag; }
	void SetGameObjectTag(GAMEOBJECT_TAG tag){ m_tag = tag; }
	Transform* GetTransform() { return _transform; }
	void SetTranslate(float x, float y, float z) { _transform->localPosition = D3DXVECTOR3(x, y, z); }
	void SetRotation(float x, float y, float z) { _transform->localRotation = D3DXVECTOR3(x, y, z); }
	void SetScale(float x, float y, float z) { _transform->localScale = D3DXVECTOR3(x, y, z); }
	D3DXVECTOR3 GetTranslate() { return _transform->localPosition; }
	D3DXVECTOR3 GetRotation() { return _transform->localRotation; }
	D3DXVECTOR3 GetScale() { return _transform->localScale; }
	D3DXMATRIXA16 GetWorldMatrix() { return _transform->m_matWorld; }
	D3DXMATRIXA16* GetPWorldMatrix() { return &_transform->m_matWorld; }
	void RenderGizmo(LPDIRECT3DDEVICE9 pDevice) { _transform->RenderGizmo(pDevice); }

	void SetName(const char* name);
	std::wstring GetName() { return _name; }

	void DestroyComponents();

	//SceneRoot �� child gameobject ��� ����(SceneRoot�� �����ؾ���)
	void DestroyAllChildObjects();

	void AddChild(GameObject* child)
	{
		//child->SetParent(this);
		m_childGameObjects.push_back(child);
	}
	void SubChild(GameObject* child)
	{
		std::vector<GameObject*>::iterator iter;
		for (iter = m_childGameObjects.begin(); iter != m_childGameObjects.end(); ++iter)
		{
			if ((*iter) == child)
			{
				m_childGameObjects.erase(iter);
				return;
			}
		}
		assert(!"�θ��� �ڽĺ��Ϳ��� �ڽ�(this) ���� ����");
	}
	void SetParent(GameObject* parent)
	{
		if (isEnable == false)
		{
			assert(!"disable�� GameObject�� SetParent() ȣ��");
			return;
		}
		if (m_objectParent != NULL)
		{
			//�θ��� �ڽĵ� �߿� �ڽ��� ����
			m_objectParent->SubChild(this);
		}
		if (parent == NULL)
		{
			m_objectParent = GetSceneRoot();
			_transform->pParentWorldMatrix = NULL;
			GetSceneRoot()->AddChild(this);
		}
		else
		{
			m_objectParent = parent;
			_transform->pParentWorldMatrix = &(parent->_transform->m_matWorld);
			parent->AddChild(this);
		}
	}

	static GameObject* FindObject(std::wstring objName);
	static void FindObjects(IN std::wstring objName, OUT std::vector<GameObject*>& outputVector);

	void OnDisable();
	void OnEnable();

	//skinnedmesh�� ��밡���� �Լ�
	void AttachObjectToFrame(const char* frameName, GameObject* obj);

	// �̱���
	//void LoadPrefab(const std::wstring& prefabFileName){}
	GameObject* GetParent() { return m_objectParent; }

	std::vector<GameObject*> & GetVecChild() { return m_childGameObjects; }

	char* GetAttackBipName(){ return m_AttachBipName; }

	bool CheckingPicking();

	//�浹 �� ȣ���� �ݹ��Լ� ����
	void SetCallback_OnCollisionEnter(MY_CALLBACK cbFunc)
	{
		funcPtr_OnCollisionEnter = cbFunc;
	}
	void SetCallback_OnCollisionStay(MY_CALLBACK cbFunc)
	{
		funcPtr_OnCollisionStay = cbFunc;
	}
	void SetCallback_OnCollisionExit(MY_CALLBACK cbFunc)
	{
		funcPtr_OnCollisionExit = cbFunc;
	}

	//�ݹ��Լ� ����
	void ActionCallbackOnCollisionEnter(cCollisionShape* myShape, cCollisionShape* targetShape)
	{
		if (funcPtr_OnCollisionEnter == NULL) return;

		funcPtr_OnCollisionEnter(myShape, targetShape);
	}
	void ActionCallbackOnCollisionStay(cCollisionShape* myShape, cCollisionShape* targetShape)
	{
		if (funcPtr_OnCollisionStay == NULL) return;

		funcPtr_OnCollisionStay(myShape, targetShape);
	}
	void ActionCallbackOnCollisionExit(cCollisionShape* myShape, cCollisionShape* targetShape)
	{
		if (funcPtr_OnCollisionExit == NULL) return;

		funcPtr_OnCollisionExit(myShape, targetShape);
	}

	void SetIsRender(bool b) { isRender = b; }
	bool GetIsRender() { return isRender; }

	bool GetIsEnable() { return isEnable; }

protected:
	

	UniqueID		m_id;
	GAMEOBJECT_TAG			m_tag;
	std::wstring _name;
	Transform* _transform;
	std::vector<GameObject*> m_childGameObjects;
	std::vector<IComponent*> m_vecComponent;
	GameObject* m_objectParent;
	bool isEnable;
	char m_AttachBipName[256];

	MY_CALLBACK funcPtr_OnCollisionEnter;
	MY_CALLBACK funcPtr_OnCollisionStay;
	MY_CALLBACK funcPtr_OnCollisionExit;

	bool isRender;
};


class D3DRenderer
{
public:
	D3DRenderer(){}
	virtual ~D3DRenderer(){}

	bool BeginFrame();
	void EndFrame();
	void DisplayFrame();
	void CreateDevice(HWND hWnd);

	
	/*static void SetLight(DWORD index, const D3DLIGHT9* d3dLight)
	{
		g_pD3DDevice->SetLight(index, d3dLight);
	}*/

	
protected:
	
};

//----------------------------------------------
}

extern ENGINE::GameObject* g_pSceneRoot;
extern std::multimap<std::wstring, ENGINE::GameObject*> g_mulmapEnableObject;
