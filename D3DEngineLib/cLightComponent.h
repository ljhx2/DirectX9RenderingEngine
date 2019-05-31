#pragma once
#include "IComponent.h"

namespace ENGINE
{
	enum ELIGHT_TYPE
	{
		eLIGHT_DIRECTIONAL = 0,
		eLIGHT_POINT
	};

	class cLightComponent : public IComponent
	{
	public:
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();

		void SetLightType(ELIGHT_TYPE type) { m_Type = type; }
		void SetDiffuseColor(D3DXCOLOR color) { m_DiffuseColor = color; }
		void SetSpecularColor(D3DXCOLOR color) { m_SpecularColor = color; }
		void SetAmbientColor(D3DXCOLOR color) { m_AmbientColor = color; }
		void SetDirection(D3DXVECTOR3 dir) { m_Direction = dir; }

		ELIGHT_TYPE GetLightType() { return m_Type; }
		D3DXCOLOR GetDiffuseColor() { return m_DiffuseColor; }
		D3DXCOLOR GetSpecularColor() { return m_SpecularColor; }
		D3DXCOLOR GetAmbientColor() { return m_AmbientColor; }
		D3DXVECTOR3 GetPosition() { return m_Position; }
		D3DXVECTOR3 GetDirection() { return m_Direction; }

		void cLightComponent::Draw3DLine(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, D3DCOLOR color);

		cLightComponent();
		cLightComponent(GameObject* pOwner);
		~cLightComponent();

	private:
		ELIGHT_TYPE m_Type;

		D3DXCOLOR m_DiffuseColor;
		D3DXCOLOR m_SpecularColor;
		D3DXCOLOR m_AmbientColor;

		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_Direction;

		ID3DXMesh* m_Mesh;

		D3DXMATRIXA16 m_World;

	};

}

