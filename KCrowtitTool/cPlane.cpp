#include "stdafx.h"
#include "cPlane.h"

cPlane::cPlane()
{
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vAngle = D3DXVECTOR3(0, 0, 0);

	D3DXMatrixScaling(&m_Scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&m_Rotation, m_vAngle.y, m_vAngle.x, m_vAngle.z);
	D3DXMatrixTranslation(&m_Translation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_World = m_Scale * m_Rotation * m_Translation;
}
cPlane::~cPlane()
{

}

void cPlane::Setup(int count)
{
	/*
	memset(&m_Matrial, 0, sizeof(D3DMATERIAL9));

	m_Matrial.Ambient.r = 1.0f;
	m_Matrial.Ambient.g = 1.0f;
	m_Matrial.Ambient.b = 1.0f;
	m_Matrial.Ambient.a = 1.0f;

	m_Matrial.Diffuse.r = 1.0f;
	m_Matrial.Diffuse.g = 1.0f;
	m_Matrial.Diffuse.b = 1.0f;
	m_Matrial.Diffuse.a = 1.0f;
	*/
	//XYZ
	m_VertexPC.resize((GRID_MAX_NUM * 2) * 2 + 2);

	int mid = (G_SIZE - 1);
	for (int x = 0; x < GRID_MAX_NUM * 2; x += 2)
	{
		m_VertexPC[x].p = D3DXVECTOR3(-GRID_SIZE_X * GRID_SPACE + GRID_SPACE, 0, GRID_START + (x * GRID_SPACE) + GRID_SPACE);
		m_VertexPC[x + 1].p = D3DXVECTOR3(GRID_SIZE_X * GRID_SPACE - (2 * GRID_SPACE) + GRID_SPACE, 0, GRID_START + (x * GRID_SPACE) + GRID_SPACE);
		if (mid == x)
		{
			m_VertexPC[x].c = D3DCOLOR_XRGB(255, 0, 0);
			m_VertexPC[x + 1].c = D3DCOLOR_XRGB(255, 0, 0);
		}
		else
		{
			m_VertexPC[x].c = D3DCOLOR_XRGB(255, 255, 255);
			m_VertexPC[x + 1].c = D3DCOLOR_XRGB(255, 255, 255);
		}
	}

	for (int z = GRID_MAX_NUM * 2; z < GRID_MAX_NUM * 4; z += 2)
	{
		m_VertexPC[z].p = D3DXVECTOR3((-GRID_SIZE_X * 2 * GRID_SPACE) + GRID_START + (z * GRID_SPACE) + GRID_SPACE, 0, -GRID_SIZE_Z* GRID_SPACE + GRID_SPACE);
		m_VertexPC[z + 1].p = D3DXVECTOR3((-GRID_SIZE_X * 2 * GRID_SPACE) + GRID_START + (z * GRID_SPACE) + GRID_SPACE, 0, GRID_SIZE_Z* GRID_SPACE - (2 * GRID_SPACE) + GRID_SPACE);
		if (GRID_MAX_NUM * 2 + mid == z)
		{
			m_VertexPC[z].c = D3DCOLOR_XRGB(0, 0, 255);
			m_VertexPC[z + 1].c = D3DCOLOR_XRGB(0, 0, 255);
		}
		else
		{
			m_VertexPC[z].c = D3DCOLOR_XRGB(255, 255, 255);
			m_VertexPC[z + 1].c = D3DCOLOR_XRGB(255, 255, 255);
		}
	}

	m_VertexPC[GRID_MAX_NUM * 4].p = D3DXVECTOR3(-0, -GRID_SIZE_Y, -0);
	m_VertexPC[GRID_MAX_NUM * 4].c = D3DCOLOR_XRGB(0, 255, 0);
	m_VertexPC[GRID_MAX_NUM * 4 + 1].p = D3DXVECTOR3(-0, GRID_SIZE_Y, -0);
	m_VertexPC[GRID_MAX_NUM * 4 + 1].c = D3DCOLOR_XRGB(0, 255, 0);

}
void cPlane::Update()
{
	D3DXMatrixScaling(&m_Scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&m_Rotation, m_vAngle.y, m_vAngle.x, m_vAngle.z);
	D3DXMatrixTranslation(&m_Translation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_World = m_Scale * m_Rotation * m_Translation;
}
void cPlane::Render()
{
	g_pD3DDevice->LightEnable(0, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, NULL);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_World);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_VertexPC.size() / 2, &m_VertexPC[0], sizeof(ST_PC_VERTEX));
}