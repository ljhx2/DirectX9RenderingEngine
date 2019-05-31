#pragma once

struct TempVertexType
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;
};

//삼각형 정점자료 받아서 tangent(접선), binormal(종접선)을 리턴해준다
void CalculateModelVectors(TempVertexType pVertex1, TempVertexType pVertex2, TempVertexType pVertex3, D3DXVECTOR3* pTangent, D3DXVECTOR3* pBinormal);
//접선과 종접선을 구한다
void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, D3DXVECTOR3&, D3DXVECTOR3&);
//접선과 종접선을 이용하여 노말값을 구한다
void CalculateNormal(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3&);

