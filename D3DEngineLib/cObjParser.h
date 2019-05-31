#pragma once

#include "cStaticMeshComponent.h"	//ST_PNT_VERTEX2, ST_GROUP, ST_MTL_TEX 사용하기 위해

class cObjParser
{
private:
	std::vector<D3DXVECTOR3>			m_V;
	std::vector<D3DXVECTOR2>			m_VT;
	std::vector<D3DXVECTOR3>			m_VN;

public:
	void Load(IN const char* szFilePath,
		OUT std::vector<ST_GROUP*>& Groups,
		OUT std::map<std::string, ST_MTL_TEX> &MtlTex);

	cObjParser();
	~cObjParser();
private:
	void ProcessMtl(IN char* szFileName,
		OUT std::map<std::string, ST_MTL_TEX> &MtlTex);

	bool IsStartWith(char* szStr, char* szBeginStr);

	void GetCenterRadius(IN const std::vector<ST_PNT_VERTEX2>& vertexs, OUT D3DXVECTOR3* vMax, OUT D3DXVECTOR3* vMin);



};

