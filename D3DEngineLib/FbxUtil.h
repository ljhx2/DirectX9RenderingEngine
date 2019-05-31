#pragma once

struct TempVertexType
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;
};

//�ﰢ�� �����ڷ� �޾Ƽ� tangent(����), binormal(������)�� �������ش�
void CalculateModelVectors(TempVertexType pVertex1, TempVertexType pVertex2, TempVertexType pVertex3, D3DXVECTOR3* pTangent, D3DXVECTOR3* pBinormal);
//������ �������� ���Ѵ�
void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, D3DXVECTOR3&, D3DXVECTOR3&);
//������ �������� �̿��Ͽ� �븻���� ���Ѵ�
void CalculateNormal(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3&);

