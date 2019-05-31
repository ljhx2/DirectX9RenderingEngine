#include "stdafx.h"
#include "cGrid.h"


cGrid::cGrid()
{
}


cGrid::~cGrid()
{
}

void cGrid::Setup(float fGap, int nNumHalf)
{
	fGap /= 2.0f;

	nNumHalf += (nNumHalf % 2);

	m_nNumHalf = nNumHalf;

	int nLine = nNumHalf * 2;

	float fMinusX = -nNumHalf * fGap;
	float fPlusX = nNumHalf * fGap;
	for (int i = 0; i < nLine + 1; i += 2)
	{
		float fPosZ = (-nNumHalf + i) * fGap;

		D3DCOLOR c;
		if (i == nNumHalf)
			c = D3DCOLOR_XRGB(255, 0, 0);
		else if ((i - nNumHalf) % 5 == 0)
			c = D3DCOLOR_XRGB(255, 255, 255);
		else
			c = D3DCOLOR_XRGB(190, 190, 190);

		ST_PC_VERTEX v;
		v.c = c;
		v.p = D3DXVECTOR3(fMinusX, 0.0f, fPosZ);
		m_LineVertex.push_back(v);

		v.p = D3DXVECTOR3(fPlusX, 0.0f, fPosZ);
		m_LineVertex.push_back(v);
	}

	float fMinusZ = -nNumHalf * fGap;
	float fPlusZ = nNumHalf * fGap;
	for (int i = 0; i < nLine + 1; i += 2)
	{
		float fPosX = (-nNumHalf + i) * fGap;

		D3DCOLOR c;
		if (i == nNumHalf)
			c = D3DCOLOR_XRGB(0, 0, 255);
		else if ((i - nNumHalf) % 5 == 0)
			c = D3DCOLOR_XRGB(255, 255, 255);
		else
			c = D3DCOLOR_XRGB(190, 190, 190);

		ST_PC_VERTEX v;
		v.c = c;
		v.p = D3DXVECTOR3(fPosX, 0.0f, fMinusZ);
		m_LineVertex.push_back(v);

		v.p = D3DXVECTOR3(fPosX, 0.0f, fPlusZ);
		m_LineVertex.push_back(v);
	}

	ST_PC_VERTEX v;
	v.p = D3DXVECTOR3(0.0f, -nNumHalf * fGap, 0.0f);
	v.c = D3DCOLOR_XRGB(0, 255, 0);
	m_LineVertex.push_back(v);

	v.p = D3DXVECTOR3(0.0f, nNumHalf * fGap, 0.0f);
	m_LineVertex.push_back(v);
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIXA16 mI;
	D3DXMatrixIdentity(&mI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mI);

	int nLine = m_nNumHalf + 1;
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_LineVertex.size() / 2, &m_LineVertex[0], sizeof(ST_PC_VERTEX));
}