#pragma once
#include "IComponent.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace ENGINE
{
	class GameObject;
	enum GAMEOBJECT_TAG;
	class cScriptComponent : public IComponent
	{
	private:
		//��ƽ�ũ��Ʈ ����
		lua_State* m_Lua = NULL;

		ENGINE::GameObject* m_pGameObject;
		char m_scriptPath[256];
	public:
		cScriptComponent();
		cScriptComponent(GameObject* pGameObject, char* scriptPath);
		~cScriptComponent();

		void Init(GameObject* pGameObject, char* scriptPath);
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void PrevRender();
		virtual void Render();
	public://�̺�Ʈ �ݺ� �Լ�
		static void CollisionEvent(ENGINE::cCollisionShape* collider1, ENGINE::cCollisionShape* collider2);//�浹 �̺�Ʈ �Լ�
		static void AnimationEvent1(ENGINE::GameObject* obj);
		static void AnimationEvent2(ENGINE::GameObject* obj);
		static void AnimationEvent3(ENGINE::GameObject* obj);
		static void AnimationEvent4(ENGINE::GameObject* obj);
		static void AnimationEvent5(ENGINE::GameObject* obj);
		static void AnimationEvent6(ENGINE::GameObject* obj);
	public://��� �̺�Ʈ �Լ�
		void OnKeyInputEvent(std::string keyValue);
		void OnMouseEvent(std::string eventType, D3DXVECTOR3 pos, int U_ID);
	public:
		char* GetScriptPath(){ return m_scriptPath; }
		lua_State* GetLua(){ return m_Lua; }
	public://��� �Լ�
		GameObject* FindU_IDGameObject(unsigned int u_id);
		//Ư�� �±��� ���ӿ�����Ʈ�� ã�� �Լ� �ִ� 100������ ã����
		void FindTagGameObjects(GAMEOBJECT_TAG tag,IN unsigned int* arr, OUT int& size);
	public://��ƿ �Լ�
		double GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	};
}
