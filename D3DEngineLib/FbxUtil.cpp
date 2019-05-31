
#include "Common.h"
#include "include\D3DRenderer.h"
#include "FbxUtil.h"

void CalculateModelVectors(TempVertexType pVertex1, TempVertexType pVertex2, TempVertexType pVertex3, D3DXVECTOR3* pTangent, D3DXVECTOR3* pBinormal)
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal, normal;

	// 받은 정점 자료를 복사한다
	vertex1 = pVertex1;
	vertex2 = pVertex2;
	vertex3 = pVertex3;

	// 접선과 종접선을 구한다
	CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

	// 접선과 종접선을 이용하여 외적으로 노말을 구한다(?????근데 노말 왜 다시 구함)
	//CalculateNormal(tangent, binormal, normal);

	//접선 리턴
	*pTangent = tangent;
	//종접선 리턴
	*pBinormal = binormal;
}
void CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, D3DXVECTOR3& tangent, D3DXVECTOR3& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	// Calculate the two vectors for this face.
	vector1[0] = vertex2.p.x - vertex1.p.x;
	vector1[1] = vertex2.p.y - vertex1.p.y;
	vector1[2] = vertex2.p.z - vertex1.p.z;

	vector2[0] = vertex3.p.x - vertex1.p.x;
	vector2[1] = vertex3.p.y - vertex1.p.y;
	vector2[2] = vertex3.p.z - vertex1.p.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.t.x - vertex1.t.x;
	tvVector[0] = vertex2.t.y - vertex1.t.y;

	tuVector[1] = vertex3.t.x - vertex1.t.x;
	tvVector[1] = vertex3.t.y - vertex1.t.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}
void CalculateNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal)
{
	float length;

	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}