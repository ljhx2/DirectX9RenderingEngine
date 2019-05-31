#include "Common.h"
#include "cObjParser.h"


cObjParser::cObjParser()
{
}


cObjParser::~cObjParser()
{
}

void cObjParser::GetCenterRadius(IN const std::vector<ST_PNT_VERTEX2>& vertexs, OUT D3DXVECTOR3* vMax, OUT D3DXVECTOR3* vMin)
{
	for (size_t i = 0; i < vertexs.size(); i++)
	{
		if (vertexs[i].p.x < vMin->x) vMin->x = vertexs[i].p.x;
		if (vertexs[i].p.y < vMin->y) vMin->y = vertexs[i].p.y;
		if (vertexs[i].p.z < vMin->z) vMin->z = vertexs[i].p.z;
		if (vertexs[i].p.x > vMax->x) vMax->x = vertexs[i].p.x;
		if (vertexs[i].p.y > vMax->y) vMax->y = vertexs[i].p.y;
		if (vertexs[i].p.z > vMax->z) vMax->z = vertexs[i].p.z;
	}
}

void cObjParser::Load(IN const char* szFilePath,
	OUT std::vector<ST_GROUP*>& Groups,
	OUT std::map<std::string, ST_MTL_TEX> &MtlTex)
{
	std::vector<ST_PNT_VERTEX2>	Vertexes;
	char szBuf[1024];

	FILE* fp;
	fopen_s(&fp, szFilePath, "r");

	while (!feof(fp))
	{
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (IsStartWith(szBuf, "mtllib"))
		{
			char szMtlFileName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlFileName, 1024);
			ProcessMtl(szMtlFileName, MtlTex);
		}
		else if (szBuf[0] == 'g')
		{
			if (!Vertexes.empty())
			{
				// 버텍스 버퍼 생성
				g_pD3DDevice->CreateVertexBuffer(
					Vertexes.size() * sizeof(ST_PNT_VERTEX2),
					0,
					ST_PNT_VERTEX2::FVF,
					D3DPOOL_MANAGED,
					&Groups.back()->pVB,
					0);
				// 삼각형 개수 기록
				Groups.back()->nNumTri = Vertexes.size() / 3;

				// 디바이스 메모리에 데이터 기록
				void* pVertex;
				Groups.back()->pVB->Lock(0, 0, &pVertex, 0);
				memcpy(pVertex, &Vertexes[0], Vertexes.size() * sizeof(ST_PNT_VERTEX2));
				Groups.back()->pVB->Unlock();

				//반지름과 중점을 구한기 위한 점
				D3DXVECTOR3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
				D3DXVECTOR3 Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				GetCenterRadius(Vertexes, &Max, &Min);
				Groups.back()->vMax = Max;
				Groups.back()->vMin = Min;

				Vertexes.clear();
			}
		}
		else if (IsStartWith(szBuf, "vt"))
		{
			float fU, fV;
			sscanf_s(szBuf, "%*s %f %f %*f", &fU, &fV);
			m_VT.push_back(D3DXVECTOR2(fU, -fV));
		}
		else if (IsStartWith(szBuf, "vn"))
		{
			float fX, fY, fZ;
			sscanf_s(szBuf, "%*s %f %f %f", &fX, &fY, &fZ);
			m_VN.push_back(D3DXVECTOR3(fX, fY, fZ));
		}
		else if (IsStartWith(szBuf, "v "))
		{
			float fX, fY, fZ;
			sscanf_s(szBuf, "%*s %f %f %f", &fX, &fY, &fZ);
			m_V.push_back(D3DXVECTOR3(fX, fY, fZ));
		}
		else if (IsStartWith(szBuf, "usemtl"))
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);

			ST_GROUP* pGroup = new ST_GROUP;
			pGroup->sMtlName = szMtlName;
			Groups.push_back(pGroup);
		}
		else if (IsStartWith(szBuf, "f "))
		{
			if (!m_VN.empty())	//노말정보가 있을때
			{
				int nIdx[3][3];
				sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
					&nIdx[0][0], &nIdx[0][1], &nIdx[0][2],
					&nIdx[1][0], &nIdx[1][1], &nIdx[1][2],
					&nIdx[2][0], &nIdx[2][1], &nIdx[2][2]);

				for (int i = 0; i < 3; ++i)
				{
					ST_PNT_VERTEX2 v;
					v.p = m_V[nIdx[i][0] - 1];
					v.t = m_VT[nIdx[i][1] - 1];
					v.n = m_VN[nIdx[i][2] - 1];
					Vertexes.push_back(v);
				}
			}
			else    //노말정보가 없을때 (노말정보가 없는 obj파일도 있음)
			{
				int nIdx[3][2];
				sscanf_s(szBuf, "%*s %d/%d %d/%d %d/%d",
					&nIdx[0][0], &nIdx[0][1],
					&nIdx[1][0], &nIdx[1][1],
					&nIdx[2][0], &nIdx[2][1]);

				for (int i = 0; i < 3; ++i)
				{
					ST_PNT_VERTEX2 v;
					v.p = m_V[nIdx[i][0] - 1];
					v.t = m_VT[nIdx[i][1] - 1];

					Vertexes.push_back(v);
				}
			}
		}
	}
	
	//파일을 다 돌고나서 한번도 ST_GROUP을 만들어줘야한다
	if (!Vertexes.empty())
	{
		// 버텍스 버퍼 생성
		g_pD3DDevice->CreateVertexBuffer(
			Vertexes.size() * sizeof(ST_PNT_VERTEX2),
			0,
			ST_PNT_VERTEX2::FVF,
			D3DPOOL_MANAGED,
			&Groups.back()->pVB,
			0);
		// 삼각형 개수 기록
		Groups.back()->nNumTri = Vertexes.size() / 3;

		// 디바이스 메모리에 데이터 기록
		void* pVertex;
		Groups.back()->pVB->Lock(0, 0, &pVertex, 0);
		memcpy(pVertex, &Vertexes[0], Vertexes.size() * sizeof(ST_PNT_VERTEX2));
		Groups.back()->pVB->Unlock();

		//반지름과 중점을 구한기 위한 점
		D3DXVECTOR3 Min(FLT_MAX, FLT_MAX, FLT_MAX);
		D3DXVECTOR3 Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		GetCenterRadius(Vertexes, &Max, &Min);
		Groups.back()->vMax = Max;
		Groups.back()->vMin = Min;

		Vertexes.clear();
	}


	m_V.clear();
	m_VT.clear();
	m_VN.clear();

	fclose(fp);
}

void cObjParser::ProcessMtl(IN char* szFileName,
	OUT std::map<std::string, ST_MTL_TEX> &MtlTex)
{
	std::string filepath("Prefabs/");
	filepath += std::string(szFileName);
	
	char szBuf[1024];

	FILE* fp;
	fopen_s(&fp, filepath.c_str(), "r");

	char szMtlName[1024];

	while (!feof(fp))
	{
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (IsStartWith(szBuf, "newmtl"))
		{
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
		}
		else if (IsStartWith(szBuf, "Ka"))
		{
			float fR, fG, fB;
			sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
			MtlTex[szMtlName].Mtl.Ambient.r = fR;
			MtlTex[szMtlName].Mtl.Ambient.g = fG;
			MtlTex[szMtlName].Mtl.Ambient.b = fB;
			MtlTex[szMtlName].Mtl.Ambient.a = 1.0f;
		}
		else if (IsStartWith(szBuf, "Kd"))
		{
			float fR, fG, fB;
			sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
			MtlTex[szMtlName].Mtl.Diffuse.r = fR;
			MtlTex[szMtlName].Mtl.Diffuse.g = fG;
			MtlTex[szMtlName].Mtl.Diffuse.b = fB;
			MtlTex[szMtlName].Mtl.Diffuse.a = 1.0f;
		}
		else if (IsStartWith(szBuf, "Ks"))
		{
			float fR, fG, fB;
			sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
			MtlTex[szMtlName].Mtl.Specular.r = fR;
			MtlTex[szMtlName].Mtl.Specular.g = fG;
			MtlTex[szMtlName].Mtl.Specular.b = fB;
			MtlTex[szMtlName].Mtl.Specular.a = 1.0f;
		}
		else if (IsStartWith(szBuf, "map_Kd"))
		{
			char szTextureFileName[1024];
			sscanf_s(szBuf, "%*s %s", szTextureFileName, 1024);

			//convert to unicode string
			//wchar_t wszTextureFileName[256] = { 0, };
			//::MultiByteToWideChar(CP_ACP, 0, szTextureFileName, -1, wszTextureFileName, strlen(szTextureFileName));
			//
			//D3DXCreateTextureFromFile(g_pD3DDevice, wszTextureFileName, &MtlTex[szMtlName].pTex);
			std::string filepath("Prefabs/");
			filepath += std::string(szTextureFileName);
			MtlTex[szMtlName].pTex = g_pTextureManager->GetTexture(filepath);
		}
	}
	fclose(fp);
}

bool cObjParser::IsStartWith(char* szStr, char* szBeginStr)
{
	char* szTemp = strstr(szStr, szBeginStr);
	if (szTemp == szStr)
		return true;
	return false;
}