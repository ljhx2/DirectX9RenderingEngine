#include "Common.h"
#include "include\D3DRenderer.h"
#include "cLightComponent.h"

namespace ENGINE
{

	cLightComponent::cLightComponent()
	{
		eComponent_Type = eLIGHT_TYPE;
		m_pOwnerObject = NULL;
		m_Mesh = NULL;
	}

	cLightComponent::cLightComponent(GameObject* pOwner)
	{
		eComponent_Type = eLIGHT_TYPE;
		m_pOwnerObject = pOwner;

		m_DiffuseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_SpecularColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_AmbientColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		D3DXCreateSphere(g_pD3DDevice, 10.0f, 6U, 6U, &m_Mesh, 0);

		g_pLightManager->AddLight(this);
	}


	cLightComponent::~cLightComponent()
	{
		g_pLightManager->EraseLight(this);
	}

	void cLightComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		m_World = *matWorld;
		m_Position = D3DXVECTOR3(m_World._41, m_World._42, m_World._43);
		D3DXVECTOR3 front = D3DXVECTOR3(m_World._31, m_World._32, m_World._33);
		D3DXVec3Normalize(&m_Direction, &front);
		
	}

	void cLightComponent::Render()
	{
		D3DCOLOR color = D3DCOLOR_ARGB(0, 0, 0, 0);

		Draw3DLine(m_Position, m_Position + m_Direction * 20, color);

		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_World);
		m_Mesh->DrawSubset(0);

		
	}

	void cLightComponent::Draw3DLine(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, D3DCOLOR color)
	{
		struct T_LINE_3D
		{
			D3DXVECTOR3 pos;
			D3DCOLOR color;
		};
		const DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
		D3DXMATRIX zero_mat;
		D3DXMatrixTranslation(&zero_mat, 0.0f, 0.0f, 0.0f);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &zero_mat);

		T_LINE_3D lines[2];
		lines[0].pos = p1;
		lines[0].color = color;
		lines[1].pos = p2;
		lines[1].color = color;

		g_pD3DDevice->SetFVF(fvf);
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, lines, sizeof(T_LINE_3D));
	}
}