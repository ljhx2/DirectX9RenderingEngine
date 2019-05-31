#pragma once
#include "IComponent.h"

namespace ENGINE
{

	class cCameraComponent : public IComponent
	{
	public:
		
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();

		void Reset(HWND hWnd);
		void UpdateProj();

		float GetFovy() { return m_fovy; }
		float GetWidth() { return m_width; }
		float GetHeight() { return m_height; }
		float GetZNear() { return m_z_near; }
		float GetZFar() { return m_z_far; }
		bool GetViewThisCamera() { return viewThisCamera; }
		void SetFovy(float value) { m_fovy = value; UpdateProj(); }
		void SetWidth(float value) { m_width = value; UpdateProj(); }
		void SetHeight(float value) { m_height = value; UpdateProj(); }
		void SetZNear(float value) { m_z_near = value; UpdateProj(); }
		void SetZFar(float value) { m_z_far = value; UpdateProj(); }
		void SetViewThisCamera(bool b) { viewThisCamera = b; UpdateProj(); }

		D3DXVECTOR3 GetPosition() { return m_Position; }
		D3DXVECTOR3 GetFront() { return m_Front; }
		D3DXVECTOR3 GetRight() { return m_Right; }
		D3DXVECTOR3 GetUp() { return m_Up; }

		D3DXMATRIXA16& GetMatView() { return m_matView; }
		D3DXMATRIXA16& GetMatProj() { return m_matProj; }

		POINT WorldToScreen(float x, float y, float z);

		void SetWheelDelta(short* wheel) { m_WheelDeltaWPARAM = wheel; }

		cCameraComponent();
		cCameraComponent(GameObject* pOwner, HWND hWnd);
		~cCameraComponent();

	protected:
		//projection 행렬 인자값
		float m_fovy;		//시야각
		float m_width, m_height;	//종횡비
		float m_z_near;
		float m_z_far;

		D3DXMATRIXA16 m_matView;
		D3DXMATRIXA16 m_matProj;

		bool viewThisCamera;

		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_Front;
		D3DXVECTOR3 m_Right;
		D3DXVECTOR3 m_Up;

		short* m_WheelDeltaWPARAM;

	};

}