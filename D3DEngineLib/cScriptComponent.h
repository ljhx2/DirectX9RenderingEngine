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
		//루아스크립트 변수
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
	public://이벤트 콜벡 함수
		static void CollisionEvent(ENGINE::cCollisionShape* collider1, ENGINE::cCollisionShape* collider2);//충돌 이벤트 함수
		static void AnimationEvent1(ENGINE::GameObject* obj);
		static void AnimationEvent2(ENGINE::GameObject* obj);
		static void AnimationEvent3(ENGINE::GameObject* obj);
		static void AnimationEvent4(ENGINE::GameObject* obj);
		static void AnimationEvent5(ENGINE::GameObject* obj);
		static void AnimationEvent6(ENGINE::GameObject* obj);
	public://루아 이벤트 함수
		void OnKeyInputEvent(std::string keyValue);
		void OnMouseEvent(std::string eventType, D3DXVECTOR3 pos, int U_ID);
	public:
		char* GetScriptPath(){ return m_scriptPath; }
		lua_State* GetLua(){ return m_Lua; }
	public://기능 함수
		GameObject* FindU_IDGameObject(unsigned int u_id);
		//특정 태그의 게임오브젝트를 찾는 함수 최대 100개까지 찾느다
		void FindTagGameObjects(GAMEOBJECT_TAG tag,IN unsigned int* arr, OUT int& size);
	public://유틸 함수
		double GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	};
}
