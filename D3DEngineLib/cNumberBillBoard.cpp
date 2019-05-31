#include "Common.h"
#include "cNumberBillBoard.h"
#include "cCameraComponent.h"

namespace ENGINE
{
	const DWORD cNumberBillBoard::ST_PT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_TEX1);


	cNumberBillBoard::cNumberBillBoard()
	{
		m_pTex = NULL;
	}


	cNumberBillBoard::~cNumberBillBoard()
	{
		m_Vertex.clear();
	}


	void cNumberBillBoard::Init(LPDIRECT3DTEXTURE9 tex)
	{
		m_pTex = tex;

		m_Vertex.resize(6);


		m_Vertex[0].t.x = 0.0f;
		m_Vertex[0].t.y = 1.0f;
		m_Vertex[1].t.x = 1.0f;
		m_Vertex[1].t.y = 0.0f;
		m_Vertex[2].t.x = 1.0f;
		m_Vertex[2].t.y = 1.0f;
		m_Vertex[3].t.x = 0.0f;
		m_Vertex[3].t.y = 1.0f;
		m_Vertex[4].t.x = 0.0f;
		m_Vertex[4].t.y = 0.0f;
		m_Vertex[5].t.x = 1.0f;
		m_Vertex[5].t.y = 0.0f;
	}


	void cNumberBillBoard::Render(cCameraComponent* camera, D3DXVECTOR3 pos)
	{
		if (m_pTex == NULL) assert(!"숫자 이미지 없음");


		m_Origin = pos;

		D3DXVECTOR3 vToCamera = camera->GetPosition() - m_Origin;

		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &(D3DXVECTOR3(0.0f, 1.0f, 0.0f)), &vToCamera);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVECTOR3 vLocalUP;
		D3DXVec3Cross(&vLocalUP, &vToCamera, &vRight);
		D3DXVec3Normalize(&vLocalUP, &vLocalUP);

		D3DXVECTOR3 offsetX = vRight * (m_width / 2);
		D3DXVECTOR3 offsetY = vLocalUP * (m_height / 2);

		m_Vertex[0].p = m_Origin + offsetX - offsetY;
		m_Vertex[1].p = m_Origin - offsetX + offsetY;
		m_Vertex[2].p = m_Origin - offsetX - offsetY;
		m_Vertex[3].p = m_Origin + offsetX - offsetY;
		m_Vertex[4].p = m_Origin + offsetX + offsetY;
		m_Vertex[5].p = m_Origin - offsetX + offsetY;


		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
		g_pD3DDevice->SetTexture(0, m_pTex);

		D3DXMATRIXA16 world;
		D3DXMatrixIdentity(&world);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);

		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_Vertex[0], sizeof(ST_PT_VERTEX));

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}


}