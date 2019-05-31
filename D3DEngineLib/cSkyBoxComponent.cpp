#include "Common.h"
#include "include\D3DRenderer.h"
#include "cSkyBoxComponent.h"

namespace ENGINE
{
	const DWORD cSkyBoxComponent::ST_PNT_VERTEX::FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	cSkyBoxComponent::cSkyBoxComponent() : m_pMesh(NULL), m_pEnvEffect(NULL)
		, m_pEnvTex(NULL)
	{
	}
	cSkyBoxComponent::~cSkyBoxComponent()
	{
		SAFE_RELEASE(m_pEnvTex);
		SAFE_RELEASE(m_pEnvEffect);
		SAFE_RELEASE(m_pMesh);
	}
	cSkyBoxComponent::cSkyBoxComponent(GameObject* pOwnerObject, char* fileName)
	{
		eComponent_Type = eSKYBOX_TYPE;
		m_pOwnerObject = pOwnerObject;

		m_szFileName[0] = fileName;
		bIsTexOnce = TRUE;
		D3DXCreateSphere(g_pD3DDevice, 12.0f, 12.0f, 12.0f, &m_pMesh, NULL);
		D3DXCreateCubeTextureFromFile(g_pD3DDevice, UTIL::CharToTCHAR(fileName), &m_pEnvTex);

		m_pEnvEffect = g_pShaderManager->GetShader("SkyBox/", "sky.fx");

	}

	cSkyBoxComponent::cSkyBoxComponent(GameObject* pOwnerObject, char* top, char* bottom, char* left, char* right, char* front, char* back)
	{
		eComponent_Type = eSKYBOX_TYPE;
		m_pOwnerObject = pOwnerObject;

		bIsTexOnce = FALSE;

		/////////////////////////
		//LPDIRECT3DSURFACE9	pSrf;
		//D3DSURFACE_DESC		dscC;
		//D3DSURFACE_DESC		dscD;

		//if (FAILED(g_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrf)))
		//{
		//	::MessageBox(0,L"SKYBOX_MONO",0,0);
		//	return;
		//}

		//pSrf->GetDesc(&dscC);
		//pSrf->Release();

		//if (FAILED(g_pD3DDevice->GetDepthStencilSurface(&pSrf)))
		//{
		//	::MessageBox(0, L"SKYBOX_STENCIL", 0, 0);
		//	return;
		//}

		//pSrf->GetDesc(&dscD);
		//pSrf->Release();
		////////
		//
		//mMultiTex[0] = g_pTextureManager->GetTexture(top);
		//mMultiTex[1] = g_pTextureManager->GetTexture(bottom);
		//mMultiTex[3] = g_pTextureManager->GetTexture(left);
		//mMultiTex[2] = g_pTextureManager->GetTexture(right);
		//mMultiTex[4] = g_pTextureManager->GetTexture(front);
		//mMultiTex[5] = g_pTextureManager->GetTexture(back);
		//
		//
		//if (FAILED(D3DXCreateRenderToEnvMap(g_pD3DDevice, 512
		//	, 1, dscC.Format
		//	, TRUE, dscD.Format
		//	, &m_pRndEnv)))
		//{
		//	::MessageBox(0, L"SKYBOX_ENVMAP", 0, 0);
		//	return;
		//}

		//// Create Spheremap
		//if (FAILED(D3DXCreateTexture(g_pD3DDevice
		//	, 512, 512
		//	, 1, D3DUSAGE_RENDERTARGET
		//	, dscC.Format
		//	, D3DPOOL_DEFAULT, &m_pTxSph)))
		//{
		//	::MessageBox(0, L"SKYBOX_TEX", 0, 0);
		//	return;
		//}

		//D3DXVECTOR3 vcEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//D3DXVECTOR3 vcLook;
		//D3DXVECTOR3 vcUp;

		//for (DWORD i = 0; i < 6; i++)
		//{

		//	switch (i)
		//	{
		//	case D3DCUBEMAP_FACE_POSITIVE_X:
		//		vcLook = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//		vcUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//		break;
		//	case D3DCUBEMAP_FACE_NEGATIVE_X:
		//		vcLook = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//		vcUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//		break;
		//	case D3DCUBEMAP_FACE_POSITIVE_Y:
		//		vcLook = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//		vcUp = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//		break;
		//	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		//		vcLook = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//		vcUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//		break;
		//	case D3DCUBEMAP_FACE_POSITIVE_Z:
		//		vcLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//		vcUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//		break;
		//	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		//		vcLook = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//		vcUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//		break;
		//	}
		//}

		//m_Vertex.resize(36);

		////// 상 평면
		//m_Vertex[0].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		//m_Vertex[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//m_Vertex[0].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[1].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		//m_Vertex[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//m_Vertex[1].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[2].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		//m_Vertex[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//m_Vertex[2].t = D3DXVECTOR2(0.0f, 1.0f);

		//m_Vertex[3].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		//m_Vertex[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//m_Vertex[3].t = D3DXVECTOR2(0.0f, 1.0f);

		//m_Vertex[4].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		//m_Vertex[4].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//m_Vertex[4].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[5].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//m_Vertex[5].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//m_Vertex[5].t = D3DXVECTOR2(1.0f, 1.0f);

		//// 하 평면
		//m_Vertex[6].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//m_Vertex[6].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//m_Vertex[6].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[7].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		//m_Vertex[7].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//m_Vertex[7].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[8].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		//m_Vertex[8].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//m_Vertex[8].t = D3DXVECTOR2(0.0f, 1.0f);

		//m_Vertex[9].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//m_Vertex[9].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//m_Vertex[9].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[10].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		//m_Vertex[10].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//m_Vertex[10].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[11].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		//m_Vertex[11].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//m_Vertex[11].t = D3DXVECTOR2(1.0f, 1.0f);

		//// 우 평면
		//m_Vertex[12].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//m_Vertex[12].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//m_Vertex[12].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[13].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		//m_Vertex[13].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//m_Vertex[13].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[14].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		//m_Vertex[14].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//m_Vertex[14].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[15].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//m_Vertex[15].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//m_Vertex[15].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[16].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		//m_Vertex[16].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//m_Vertex[16].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[17].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		//m_Vertex[17].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//m_Vertex[17].t = D3DXVECTOR2(0.0f, 1.0f);

		//// 좌 평면
		//m_Vertex[18].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		//m_Vertex[18].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//m_Vertex[18].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[19].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		//m_Vertex[19].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//m_Vertex[19].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[20].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//m_Vertex[20].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//m_Vertex[20].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[21].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		//m_Vertex[21].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//m_Vertex[21].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[22].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//m_Vertex[22].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//m_Vertex[22].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[23].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		//m_Vertex[23].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		//m_Vertex[23].t = D3DXVECTOR2(0.0f, 1.0f);


		////근평면
		//m_Vertex[24].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//m_Vertex[24].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//m_Vertex[24].t = D3DXVECTOR2(0.0f, 1.0f);

		//m_Vertex[25].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//m_Vertex[25].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//m_Vertex[25].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[26].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		//m_Vertex[26].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//m_Vertex[26].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[27].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		//m_Vertex[27].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//m_Vertex[27].t = D3DXVECTOR2(0.0f, 1.0f);

		//m_Vertex[28].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		//m_Vertex[28].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//m_Vertex[28].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[29].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		//m_Vertex[29].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//m_Vertex[29].t = D3DXVECTOR2(1.0f, 0.0f);

		//// 원 평면
		//m_Vertex[30].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		//m_Vertex[30].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//m_Vertex[30].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[31].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		//m_Vertex[31].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//m_Vertex[31].t = D3DXVECTOR2(1.0f, 0.0f);

		//m_Vertex[32].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		//m_Vertex[32].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//m_Vertex[32].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[33].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		//m_Vertex[33].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//m_Vertex[33].t = D3DXVECTOR2(0.0f, 0.0f);

		//m_Vertex[34].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		//m_Vertex[34].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//m_Vertex[34].t = D3DXVECTOR2(1.0f, 1.0f);

		//m_Vertex[35].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		//m_Vertex[35].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		//m_Vertex[35].t = D3DXVECTOR2(0.0f, 1.0f);
	}



	void  cSkyBoxComponent::SaveSkyBox(const char * fileName)
	{
		char szFullPath[256];
		strcpy(szFullPath, "SkyBox/");
		strcat(szFullPath, fileName);

		tinyxml2::XMLDocument m_XMLDoc;
		//해더 저장  
		XMLDeclaration * decl = m_XMLDoc.NewDeclaration();
		m_XMLDoc.LinkEndChild(decl);
		XMLElement * root = m_XMLDoc.NewElement("SKYBOX");
		root->SetAttribute("Bool", bIsTexOnce);
		if (bIsTexOnce)
		{
			root->SetAttribute("Texture", m_szFileName[0].c_str());
		}
		else
		{
			root->SetAttribute("Top", m_szFileName[0].c_str());
			root->SetAttribute("Down", m_szFileName[1].c_str());
			root->SetAttribute("Left", m_szFileName[2].c_str());
			root->SetAttribute("Right",m_szFileName[3].c_str());
			root->SetAttribute("Back", m_szFileName[4].c_str());
			root->SetAttribute("Front",m_szFileName[5].c_str());
		}
		m_XMLDoc.LinkEndChild(root);

		m_XMLDoc.SaveFile(szFullPath);
	}
	void cSkyBoxComponent::ChangeTexture(char * fileName)
	{
		tinyxml2::XMLDocument m_XMLDoc;
		XMLElement * elem;
		if (XML_NO_ERROR == m_XMLDoc.LoadFile(fileName))
		{
			for (XMLNode *sky = (XMLNode*)m_XMLDoc.FirstChild(); sky != 0; sky = (XMLNode*)sky->NextSibling())
			{
				if (elem = sky->ToElement())
				{
					std::string vlaue(sky->Value());
					if (vlaue.compare("SKYBOX") == 0)
					{
						XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
						std::string tempValue(xmlTemp->Name());
						if (tempValue.compare("Bool") == 0)
						{
							bIsTexOnce= xmlTemp->BoolValue();
						}
						if (bIsTexOnce)
						{
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Texture") == 0)
							{
								m_szFileName[0] = xmlTemp->Value();
							}
							//나중에 매니저로 뺄것
							D3DXCreateCubeTextureFromFile(g_pD3DDevice, UTIL::CharToTCHAR(m_szFileName[0].c_str()), &m_pEnvTex);
							m_pEnvEffect = g_pShaderManager->GetShader("SkyBox/", "sky.fx");
						}
						else
						{
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Top") == 0)
							{
								m_szFileName[0] = xmlTemp->Value();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Down") == 0)
							{
								m_szFileName[1] = xmlTemp->Value();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Left") == 0)
							{
								m_szFileName[2] = xmlTemp->Value();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Right") == 0)
							{
								m_szFileName[3] = xmlTemp->Value();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Back") == 0)
							{
								m_szFileName[4] = xmlTemp->Value();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							tempValue = xmlTemp->Name();
							if (tempValue.compare("Front") == 0)
							{
								m_szFileName[5] = xmlTemp->Value();
							}
							mMultiTex[0] = g_pTextureManager->GetTexture(m_szFileName[0]);
							mMultiTex[1] = g_pTextureManager->GetTexture(m_szFileName[1]);
							mMultiTex[3] = g_pTextureManager->GetTexture(m_szFileName[3]);
							mMultiTex[2] = g_pTextureManager->GetTexture(m_szFileName[2]);
							mMultiTex[4] = g_pTextureManager->GetTexture(m_szFileName[4]);
							mMultiTex[5] = g_pTextureManager->GetTexture(m_szFileName[5]);
						}
						
					}
				} 
			}
		}
	}
	void cSkyBoxComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		D3DXMatrixScaling(&m_mScale, 500, 500, 500);
		cCameraComponent * camera = g_pCameraManager->GetCurrentCamera();
		D3DXMatrixTranslation(&m_mCamera, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
		m_mWorld = m_mScale  * m_mCamera*(*matWorld);

	}

	void cSkyBoxComponent::Render()
	{

		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

		if (bIsTexOnce)
		{

			D3DXMATRIXA16 mView, mProj, mWorldViewProj;
			D3DXMatrixIdentity(&mWorldViewProj);
			g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
			g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mProj);
			mWorldViewProj = m_mWorld * mView* mProj;
			D3DXVECTOR4 vWorldCameraPosition;
			vWorldCameraPosition.x = mView._41;
			vWorldCameraPosition.y = mView._42;
			vWorldCameraPosition.z = 1.0f;
			vWorldCameraPosition.w = mView._44;
			D3DXMatrixTranslation(&m_mCamera, mView._41, mView._42, mView._43);

			m_pEnvEffect->SetTechnique("TechSky");
			m_pEnvEffect->SetMatrix("gWorldViewProjectionMatrix", &mWorldViewProj);
			m_pEnvEffect->SetVector("gWorldCameraPosition", &vWorldCameraPosition);
			m_pEnvEffect->SetTexture("EnvironmentMap_Tex", m_pEnvTex);

			m_pEnvEffect->Begin(NULL, 0);
			m_pEnvEffect->BeginPass(0);

			m_pMesh->DrawSubset(0);

			m_pEnvEffect->EndPass();
			m_pEnvEffect->End();

		}
		else
		{
			////텍스쳐 필터링
			//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


			//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//컬링 끈다.
			////		g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			//Z버퍼 끄기
			//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//조명 끄기


			////		텍스쳐 UV Address를 설정 
			//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mWorld);


			//int count = 0;
			//for (int i = 0; i < 31; i += 6)
			//{
			//	g_pD3DDevice->SetTexture(0, mMultiTex[count]);
			//	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_Vertex[i], sizeof(ST_PNT_VERTEX));
			//	count++;
			//}
		}

		g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//컬링 ccw.
		//g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			//Z버퍼 끄기
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//조명 끄기

		D3DXMatrixIdentity(&m_mWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mWorld);

	}
}
