#include "Common.h"
#include "cSceneParser.h"
#include "include\D3DRenderer.h"


namespace ENGINE
{
	cSceneParser::cSceneParser()
	{

	}
	cSceneParser::~cSceneParser()
	{

	}
	void cSceneParser::SaveScene(char* fileName)
	{
		//해더 저장  
		XMLDeclaration * decl = m_XMLDoc.NewDeclaration();
		m_XMLDoc.LinkEndChild(decl);
		XMLElement * root = m_XMLDoc.NewElement("SCENE");
		m_XMLDoc.LinkEndChild(root);
		//코멘트
		XMLComment * SceneCommentStart = m_XMLDoc.NewComment("Scene Start");
		root->LinkEndChild(SceneCommentStart);
 
		//GameObject
		for each(auto iter in GetSceneRoot()->GetVecChild())
		{
			GameObjectSave(root, iter);
		}
		//코멘트
		XMLComment * SceneCommentEnd = m_XMLDoc.NewComment("Scene End");
		root->LinkEndChild(SceneCommentEnd);

		m_XMLDoc.SaveFile(fileName);
	}
	void cSceneParser::GameObjectSave(XMLElement * xmlGameObjectElement, GameObject* pObject)
	{
		//코멘트
		XMLComment * ObjCommentStart = m_XMLDoc.NewComment("GameObject Start");
		xmlGameObjectElement->LinkEndChild(ObjCommentStart);
		//GAME OBJECT
		XMLElement * xmlGameObject = m_XMLDoc.NewElement("GAMEOBJECT");
		xmlGameObjectElement->LinkEndChild(xmlGameObject);
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("NAME");
		xmlGameObject->LinkEndChild(xmlGameObjectName);
		std::string str;
		UTIL::WStringToString(pObject->GetName(), str);
		XMLText * text = m_XMLDoc.NewText(str.c_str());
		xmlGameObjectName->LinkEndChild(text);		//게임 오브젝트의 네임

		//Translate Coponent 
		TranslateComponentSave(xmlGameObject, pObject);

		for (int i = 0; i < pObject->GetComponentSize(); ++i)
		{
			if (pObject->GetCompoent(i)->GetType() == ENGINE::eSTATIC_MESH_TYPE)
			{
				StaticMeshComponentSave(xmlGameObject, (cStaticMeshComponent*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eSKINNED_MESH_TYPE)
			{
				SkinnedMeshComponentSave(xmlGameObject, (cSkinnedMesh*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eFBX_MESH_TYPE)
			{
				FbxComponentSave(xmlGameObject, (cFbxComponent*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eCOLLIDER_TYPE)
			{
				ColliderComponentSave(xmlGameObject, (cColliderComponent*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
			{
				AnimationComponentSave(xmlGameObject, (cAnimComponent*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eSKYBOX_TYPE)
			{
				SkyBoxComponentSave(xmlGameObject, (cSkyBoxComponent*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eTERRAIN_TYPE)
			{
				TerrainComponentSave(xmlGameObject, (cTerrainComponent*)pObject->GetCompoent(i));
			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eSCRIPT_TYPE)
			{
				ScriptComponentSave(xmlGameObject, (cScriptComponent*)pObject->GetCompoent(i));

			}
			else if (pObject->GetCompoent(i)->GetType() == ENGINE::eLIGHT_TYPE)
			{
				LightComponentSave(xmlGameObject, (cLightComponent*)pObject->GetCompoent(i));

			}
			//else
			//{
			//	//fprintf_s(fp, "\tCOMPONENT ERROR\n");
			//}

		}//Component End

		/*씬 루트가 아닌 부모가 있따면 Attach 정보저장*/
		if (pObject->GetParent() != NULL && pObject->GetParent() != GetSceneRoot())
			AttachBipNameSave(xmlGameObject, pObject);

		//자식 게임 오브젝트 저장
		for (int i = 0; i < pObject->GetVecChild().size(); i++)
			GameObjectSave(xmlGameObject, pObject->GetVecChild()[i]);

		//comment
		XMLComment * ObjCommentEnd = m_XMLDoc.NewComment("GameObject End");
		xmlGameObjectElement->LinkEndChild(ObjCommentEnd);

	}

	void cSceneParser::LightComponentSave(XMLElement * xmlGameObjectElement, cLightComponent* pComponent)
	{

		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("LIGHT_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);
		XMLElement * typeXml = m_XMLDoc.NewElement("OPTION");
		typeXml->SetAttribute("Type", pComponent->GetLightType());
		xmlGameObjectName->LinkEndChild(typeXml);
		XMLElement * worldXml = m_XMLDoc.NewElement("WORLD_POS");
		D3DXVECTOR3 Pos = pComponent->GetPosition();
		worldXml->SetAttribute("x", Pos.x);
		worldXml->SetAttribute("y", Pos.y);
		worldXml->SetAttribute("z", Pos.z);
		xmlGameObjectName->LinkEndChild(worldXml);

		XMLElement * dirXML = m_XMLDoc.NewElement("DIR");
		D3DXVECTOR3 dir = pComponent->GetDirection();
		dirXML->SetAttribute("x", dir.x);
		dirXML->SetAttribute("y", dir.y);
		dirXML->SetAttribute("z", dir.z);
		xmlGameObjectName->LinkEndChild(dirXML);
		XMLElement * colorXml = m_XMLDoc.NewElement("COLOR");
		D3DXCOLOR diffuse = pComponent->GetDiffuseColor();
		D3DXCOLOR Specular = pComponent->GetSpecularColor();
		D3DXCOLOR Ambient = pComponent->GetAmbientColor();
		colorXml->SetAttribute("Diffuse.R", diffuse.r);
		colorXml->SetAttribute("Diffuse.G", diffuse.g);
		colorXml->SetAttribute("Diffuse.B", diffuse.b);
		colorXml->SetAttribute("Diffuse.A", diffuse.a);
		colorXml->SetAttribute("Specular.R", Specular.r);
		colorXml->SetAttribute("Specular.G", Specular.g);
		colorXml->SetAttribute("Specular.B", Specular.b);
		colorXml->SetAttribute("Specular.A", Specular.a);
		colorXml->SetAttribute("Ambient.R", Ambient.r);
		colorXml->SetAttribute("Ambient.G", Ambient.g);
		colorXml->SetAttribute("Ambient.B", Ambient.b);
		colorXml->SetAttribute("Ambient.A", Ambient.a);
		xmlGameObjectName->LinkEndChild(colorXml);
	}
	

	void cSceneParser::TranslateComponentSave(XMLElement * xmlGameObjectElement, GameObject* pObject)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("TRANSLATE_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);

		XMLElement * ScaleXml = m_XMLDoc.NewElement("SCALE");
		D3DXVECTOR3 scale = pObject->GetScale();
		ScaleXml->SetAttribute("x", scale.x);
		ScaleXml->SetAttribute("y", scale.y);
		ScaleXml->SetAttribute("z", scale.z);
		xmlGameObjectName->LinkEndChild(ScaleXml);

		XMLElement * RotXml = m_XMLDoc.NewElement("ROT");
		D3DXVECTOR3 rot = pObject->GetRotation();
		RotXml->SetAttribute("x", rot.x);
		RotXml->SetAttribute("y", rot.y);
		RotXml->SetAttribute("z", rot.z);
		xmlGameObjectName->LinkEndChild(RotXml);

		XMLElement * PosXml = m_XMLDoc.NewElement("POS");
		D3DXVECTOR3 pos = pObject->GetTranslate();
		PosXml->SetAttribute("x", pos.x);
		PosXml->SetAttribute("y", pos.y);
		PosXml->SetAttribute("z", pos.z);
		xmlGameObjectName->LinkEndChild(PosXml);

	}	

	void cSceneParser::AttachBipNameSave(XMLElement * xmlGameObjectElement, GameObject* pObject)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("ATTACH_BIP_NAME");
		xmlGameObjectName->SetAttribute("Name", pObject->GetAttackBipName());
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);
	}

	//Static mesh 
	void cSceneParser::StaticMeshComponentSave(XMLElement * xmlGameObjectElement, cStaticMeshComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("STATIC_MESH_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);

		//XMLElement * FolderXml = xmlm_XMLDoc->NewElement("FOLDER");
		//FolderXml->SetAttribute("name", pComponent->GetFolderPath());
		//xmlGameObjectName->LinkEndChild(FolderXml);
		//XMLElement * FilePathXml = xmlm_XMLDoc->NewElement("FILE_FATH");
		//FilePathXml->SetAttribute("name", pComponent->GetFilePath());
		//xmlGameObjectName->LinkEndChild(FilePathXml);
	}
	//Skinned mesh 
	void cSceneParser::SkinnedMeshComponentSave(XMLElement * xmlGameObjectElement, cSkinnedMesh* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("SKINNED_MESH_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);
	}
	//FBX
	void cSceneParser::FbxComponentSave(XMLElement * xmlGameObjectElement, cFbxComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("FBX_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);

		XMLElement * FolderXml = m_XMLDoc.NewElement("FOLDER");
		FolderXml->SetAttribute("Name", pComponent->GetFolderPath());
		xmlGameObjectName->LinkEndChild(FolderXml);
		XMLElement * FilePathXml = m_XMLDoc.NewElement("FILE_FATH");
		FilePathXml->SetAttribute("Name", pComponent->GetFilePath());
		xmlGameObjectName->LinkEndChild(FilePathXml);

		int nMeterialTextureNum = pComponent->GetMeterialTextureNum();

		XMLElement * mtlXml = m_XMLDoc.NewElement("MATERIAL");
		xmlGameObjectName->LinkEndChild(mtlXml);

		for (int cnt = 0; cnt < nMeterialTextureNum; cnt++)
		{
			char chName[512];
			char chPath[512];
			D3DMATERIAL9 mtl;

			pComponent->GetMeterialTextureData(cnt, chName, chPath, &mtl);
			XMLElement * ptName = m_XMLDoc.NewElement(chName);
			ptName->SetAttribute("Path", chPath);
			ptName->SetAttribute("Diffuse.R", mtl.Diffuse.r);
			ptName->SetAttribute("Diffuse.G", mtl.Diffuse.g);
			ptName->SetAttribute("Diffuse.B", mtl.Diffuse.b);
			ptName->SetAttribute("Diffuse.A", mtl.Diffuse.a);
			mtlXml->LinkEndChild(ptName);
		}
	}

	//collider
	void cSceneParser::ColliderComponentSave(XMLElement * xmlGameObjectElement, cColliderComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("COLLIDER_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);

		XMLElement * bRender = m_XMLDoc.NewElement("BOUNDING_RENDER");
		bRender->SetAttribute("Bool", pComponent->GetRenderCOllisionSphere());
		xmlGameObjectName->LinkEndChild(bRender);

		XMLElement * bType = m_XMLDoc.NewElement("COLLISION_TYPE");
		bType->SetAttribute("Int", pComponent->GetCollisionTYPE());
		xmlGameObjectName->LinkEndChild(bType);

		//Collision
		std::map<std::string, cCollisionShape*>* pMapCollision = pComponent->GetMapCollision();

		for (auto iter = pMapCollision->begin(); iter != pMapCollision->end(); ++iter)
		{

			XMLElement * CollisionXml = m_XMLDoc.NewElement("COLLISION");
			CollisionXml->SetAttribute("Shape", iter->second->GetShapeType());
			CollisionXml->SetAttribute("Name", iter->second->GetName().c_str());

			XMLElement * localXml = m_XMLDoc.NewElement("LOCAL");
			CollisionXml->LinkEndChild(localXml);
			XMLElement * ParentsXml = m_XMLDoc.NewElement("PARENTS");
			ParentsXml->SetAttribute("Name", iter->second->GetParentName().c_str());
			localXml->LinkEndChild(ParentsXml);

			XMLElement * bSphereRenderXml = m_XMLDoc.NewElement("RENDER");
			bSphereRenderXml->SetAttribute("Bool", iter->second->GetIsRender());
			localXml->LinkEndChild(bSphereRenderXml);

			XMLElement * ScaleXml = m_XMLDoc.NewElement("SCALE");
			ScaleXml->SetAttribute("x", iter->second->GetLocalScale().x);
			ScaleXml->SetAttribute("y", iter->second->GetLocalScale().y);
			ScaleXml->SetAttribute("z", iter->second->GetLocalScale().z);
			localXml->LinkEndChild(ScaleXml);

			XMLElement * RotXml = m_XMLDoc.NewElement("ROT");
			RotXml->SetAttribute("x", iter->second->GetLocalRotation().x);
			RotXml->SetAttribute("y", iter->second->GetLocalRotation().y);
			RotXml->SetAttribute("z", iter->second->GetLocalRotation().z);
			localXml->LinkEndChild(RotXml);
			XMLElement * TransXml = m_XMLDoc.NewElement("POS");
			TransXml->SetAttribute("x", iter->second->GetLocalPosition().x);
			TransXml->SetAttribute("y", iter->second->GetLocalPosition().y);
			TransXml->SetAttribute("z", iter->second->GetLocalPosition().z);
			localXml->LinkEndChild(TransXml);
			xmlGameObjectName->LinkEndChild(CollisionXml);
		}
	}
	//Animation
	void cSceneParser::AnimationComponentSave(XMLElement * xmlGameObjectElement, cAnimComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("ANIMATION_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);


		std::map<std::string, ST_AnimClip*>* pvAniClip = pComponent->GetAniClip();
		for (auto iter = pvAniClip->begin(); iter != pvAniClip->end(); ++iter)
		{
			XMLElement * ClipXml = m_XMLDoc.NewElement("CLIP");
			ClipXml->SetAttribute("Name", iter->second->name.c_str());
			ClipXml->SetAttribute("Index", iter->second->setIndex);
			ClipXml->SetAttribute("Start", iter->second->start);
			ClipXml->SetAttribute("End", iter->second->end);
			ClipXml->SetAttribute("Loop", iter->second->isLoop);
			xmlGameObjectName->LinkEndChild(ClipXml);
		}
	}
	//Script
	void cSceneParser::ScriptComponentSave(XMLElement * xmlGameObjectElement, cScriptComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("SCRIPT_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);
		XMLElement * scriptXml = m_XMLDoc.NewElement("SCRIPT");
		scriptXml->SetAttribute("Path", pComponent->GetScriptPath());
		xmlGameObjectName->LinkEndChild(scriptXml);
	}

	//Terrain
	void cSceneParser::TerrainComponentSave(XMLElement * xmlGameObjectElement, cTerrainComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("TERRAIN_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);

		XMLElement * FilePathXml = m_XMLDoc.NewElement("FILE_FATH");
		std::string szPath(pComponent->GetFullPath());
		szPath.erase(0,8);
		FilePathXml->SetAttribute("Name", szPath.c_str());
		xmlGameObjectName->LinkEndChild(FilePathXml);
		pComponent->SaveAllData(szPath.c_str());

		XMLElement * cellXml = m_XMLDoc.NewElement("CELL");
		cellXml->SetAttribute("Spacing", pComponent->GetTerrainData()->GetCellSpacing());
		xmlGameObjectName->LinkEndChild(cellXml);

		XMLElement * fogXml = m_XMLDoc.NewElement("FOG");
		fogXml->SetAttribute("Bool", pComponent->GetTerrainData()->GetFog());
		xmlGameObjectName->LinkEndChild(fogXml);
		
	}
	//SkyBox
	void cSceneParser::SkyBoxComponentSave(XMLElement * xmlGameObjectElement, cSkyBoxComponent* pComponent)
	{
		XMLElement * xmlGameObjectName = m_XMLDoc.NewElement("SKYBOX_COMPONENT");
		xmlGameObjectElement->LinkEndChild(xmlGameObjectName);

		XMLElement * FilePathXml = m_XMLDoc.NewElement("FILE_OPTION");
		//해더 저장  
		bool bIsTexOnce = pComponent->GetIsTexOnce();
		FilePathXml->SetAttribute("Bool", bIsTexOnce);
		if (bIsTexOnce)
		{
			FilePathXml->SetAttribute("Name", pComponent->GetIsTexName()[0].c_str());
		}
		else
		{
			FilePathXml->SetAttribute("Top", pComponent->GetIsTexName()[0].c_str());
			FilePathXml->SetAttribute("Down", pComponent->GetIsTexName()[1].c_str());
			FilePathXml->SetAttribute("Left", pComponent->GetIsTexName()[2].c_str());
			FilePathXml->SetAttribute("Right",pComponent->GetIsTexName()[3].c_str());
			FilePathXml->SetAttribute("Back", pComponent->GetIsTexName()[4].c_str());
			FilePathXml->SetAttribute("Front",pComponent->GetIsTexName()[5].c_str());
		}
		xmlGameObjectName->LinkEndChild(FilePathXml);
	}

	//=========================================================================
	//Load
	//=========================================================================
	void cSceneParser::LoadScene(char* fileName)
	{
		tinyxml2::XMLDocument loadDoc;
		if (XML_NO_ERROR == loadDoc.LoadFile(fileName))
		{
			for (XMLNode *Scene = (XMLNode*)loadDoc.FirstChild(); Scene != 0; Scene = (XMLNode*)Scene->NextSibling())
			{
				std::string vlaue(Scene->Value());
				if (vlaue.compare("SCENE") == 0)
				{
					MessageBox(0, L"잠시만 기다려주세요", L"Succeed", 0);
					for (XMLNode * child = (XMLNode*)Scene->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
					{
						m_pRootGameObject = GetSceneRoot();
						std::string szObj(child->Value());
						if (szObj.compare("GAMEOBJECT") == 0)
						{
							//MessageBox(0, UTIL::CharToTCHAR(szObj.c_str()), 0, 0);
							GameObjectLoad(child, m_pRootGameObject);
						}
					}
				}
			}
 		}
		else
			MessageBox(0, L"Scene Load Error", 0, 0);
	}

	void cSceneParser::GameObjectLoad(const XMLNode * parent, GameObject* pParentsObject)
	{
		XMLNode * child;
		XMLElement * elem;
		XMLText * text;

		for (child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string vlaue(child->Value());
				if (vlaue.compare("GAMEOBJECT") == 0)
				{
					//MessageBox(0, UTIL::CharToTCHAR(vlaue.c_str()), 0, 0);
					GameObjectLoad(child, m_pRootGameObject);
				}
				else if (vlaue.compare("NAME") == 0)
				{
					XMLNode * name = child->FirstChild();
					m_pObjTemp = new GameObject(g_pUIDManager->CreateUID(), UTIL::CharToTCHAR(name->Value()));
					g_mulmapEnableObject.insert(std::make_pair(m_pObjTemp->GetName(), m_pObjTemp));
					m_pObjTemp->SetParent(pParentsObject); //무조건 신루트를 루트로 만들었나
					m_pRootGameObject = m_pObjTemp;
				}
				else if (vlaue.compare("TRANSLATE_COMPONENT") == 0)
				{
					//컴포넌트 하나 붙이고 
					TranslateComponentLoad(child, m_pObjTemp);
				}
				else if (vlaue.compare("FBX_COMPONENT") == 0)
				{
					FbxComponentLoad(child, m_pObjTemp);
				}
				else if (vlaue.compare("ANIMATION_COMPONENT") == 0)
				{

				}
				else if (vlaue.compare("COLLIDER_COMPONENT") == 0)
				{
					ColliderComponentLoad(child, m_pObjTemp);
				}
				else if (vlaue.compare("TERRAIN_COMPONENT") == 0)
				{
					TerrainComponentLoad(child, m_pObjTemp);
				}
				else if (vlaue.compare("SKYBOX_COMPONENT") == 0)
				{
					SkyBoxComponentLoad(child, m_pObjTemp);
				}
				else if (vlaue.compare("ATTACH_BIP_NAME") == 0)
				{

				}
				else if (vlaue.compare("STATIC_MESH_COMPONENT") == 0)
				{

				}
				else if (vlaue.compare("SKINNED_MESH_COMPONENT") == 0)
				{

				}

				else if (vlaue.compare("SCRIPT_COMPONENT") == 0)
				{

				}
				else if (vlaue.compare("LIGHT_COMPONENT") == 0)
				{
					LightComponentLoad(child, m_pObjTemp);
				}
			}
		}
	}
	void cSceneParser::SkyBoxComponentLoad(const XMLNode * parent, GameObject* pObject)
	{
	
		bool bIsOnce;
		std::string m_szFileName[6];
		char szfilName[256];

		XMLElement * elem;
		for (XMLNode * child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string szTemp(elem->Name());
				if (szTemp.compare("FILE_OPTION") == 0)
				{

					XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					if (Attrivlaue.compare("Bool") == 0)
					{
						bIsOnce = xmlAttrTemp->BoolValue();
					}
					if (bIsOnce)
					{

						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Name") == 0)
						{
							m_szFileName[0] = xmlAttrTemp->Value();
						}
					}
					else
					{
						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Top") == 0)
						{
							m_szFileName[0] = xmlAttrTemp->Value();
						}
						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Down") == 0)
						{
							m_szFileName[1] = xmlAttrTemp->Value();
						}
						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Left") == 0)
						{
							m_szFileName[2] = xmlAttrTemp->Value();
						}
						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Right") == 0)
						{
							m_szFileName[3] = xmlAttrTemp->Value();
						}
						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Back") == 0)
						{
							m_szFileName[4] = xmlAttrTemp->Value();
						}
						xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
						Attrivlaue = xmlAttrTemp->Name();
						if (Attrivlaue.compare("Front") == 0)
						{
							m_szFileName[5] = xmlAttrTemp->Value();
						}
					}
				}

			}
		}
		if (bIsOnce)
		{
			cSkyBoxComponent* pSkyComp = new cSkyBoxComponent(pObject, (char*)m_szFileName[0].c_str());
			pObject->AddComponent(pSkyComp);
		
		}
		else
		{
			cSkyBoxComponent* pSkyComp = new cSkyBoxComponent(pObject, (char*)m_szFileName[0].c_str(), (char*)m_szFileName[1].c_str(),
				(char*)m_szFileName[3].c_str(), (char*)m_szFileName[2].c_str(), (char*)m_szFileName[4].c_str(), (char*)m_szFileName[5].c_str());
			pObject->AddComponent(pSkyComp);
		}
	

	}
	void cSceneParser::LightComponentLoad(const XMLNode * parent, GameObject* pObject)
	{
		cLightComponent* pLightComp = new cLightComponent(pObject);
		pObject->AddComponent(pLightComp);


		XMLElement * elem;
		for (XMLNode * child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string szTemp(elem->Name());
				if (szTemp.compare("OPTION") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					if (Attrivlaue.compare("Type") == 0)
					{
						pLightComp->SetLightType((ELIGHT_TYPE)xmlAttrTemp->IntValue());
					}
				}
				else if (szTemp.compare("WORLD_POS") == 0)
				{
					/*XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					if (Attrivlaue.compare("x") == 0)
					{
						pLightComp->set((ELIGHT_TYPE)xmlAttrTemp->IntValue());
					}*/
				}
				else if (szTemp.compare("DIR") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					D3DXVECTOR3 dir;

					if (Attrivlaue.compare("x") == 0)
					{
						dir.x = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("y") == 0)
					{
						dir.y = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("z") == 0)
					{
						dir.z = xmlAttrTemp->FloatValue();
					}
					pLightComp->SetDirection(dir);
				}
				else if (szTemp.compare("COLOR") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					D3DXCOLOR diffuse;
					
					if (Attrivlaue.compare("Diffuse.R") == 0)
					{
						diffuse.r = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Diffuse.G") == 0)
					{
						diffuse.g = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Diffuse.B") == 0)
					{
						diffuse.b = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Diffuse.A") == 0)
					{
						diffuse.a = xmlAttrTemp->FloatValue();
					}

					D3DXCOLOR Specular;
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Specular.R") == 0)
					{
						Specular.r = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Specular.G") == 0)
					{
						Specular.g = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Specular.B") == 0)
					{
						Specular.b = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Specular.A") == 0)
					{
						Specular.a = xmlAttrTemp->FloatValue();
					}

					D3DXCOLOR ambient;
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Ambient.R") == 0)
					{
						ambient.r = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Ambient.G") == 0)
					{
						ambient.g = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Ambient.B") == 0)
					{
						ambient.b = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("Ambient.A") == 0)
					{
						ambient.a = xmlAttrTemp->FloatValue();
					}

					pLightComp->SetDiffuseColor(diffuse);
					pLightComp->SetSpecularColor(Specular);
					pLightComp->SetAmbientColor(ambient);
				}
			}
		}
	}

	void cSceneParser::ColliderComponentLoad(const XMLNode * parent, GameObject* pObject)
	{
		//Collider 컴포넌트 추가
		cColliderComponent* pColliderComp = new cColliderComponent(pObject);
		pObject->AddComponent(pColliderComp);

		XMLElement * elem;
		XMLText * text;

		char szfolderPath[256];
		char szfilePath[256];

		for (XMLNode * child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string szTemp(elem->Name());
				if (szTemp.compare("BOUNDING_RENDER") == 0)
				{

					XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					if (Attrivlaue.compare("Bool") == 0)
					{
						pColliderComp->SetRenderCollisionSphere(xmlAttrTemp->BoolValue());
					}
				}
				else if (szTemp.compare("COLLISION_TYPE") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					if (Attrivlaue.compare("Int") == 0)
					{
						pColliderComp->SetCollisionTYPE((ECOLLISION_TYPE)xmlAttrTemp->IntValue());
					}
				}
				else if (szTemp.compare("COLLISION") == 0)
				{
					
					ColliderLocalLoad(elem,pObject, pColliderComp);
				}
			}
		}
	}
	void cSceneParser::ColliderLocalLoad(const XMLElement * parent, GameObject* pObject, cColliderComponent* pColliderComp)
	{
		D3DXVECTOR3 scale, rotation, position;
		char parent_name[256];
		bool bisRender;
		char name[256];
		bool bIsShape;

		XMLAttribute * xmlAttrTemp = (XMLAttribute *)parent->FirstAttribute();
		std::string Attrivlaue(xmlAttrTemp->Name());
		if (Attrivlaue.compare("Shape") == 0)
		{
			bIsShape = xmlAttrTemp->BoolValue();
		}
		if (Attrivlaue.compare("Name") == 0)
		{
			strcpy_s(name, xmlAttrTemp->Value());
		}

		for (XMLNode * inlocal = (XMLNode*)parent->FirstChild()->FirstChild(); inlocal != 0; inlocal = (XMLNode*)inlocal->NextSibling())
		{
			XMLElement * localElem;
			if (localElem = inlocal->ToElement())
			{
				std::string localName(localElem->Name());
				if (localName.compare("PARENTS") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)localElem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
					if (Attrivlaue.compare("Name") == 0)
					{
						strcpy_s(parent_name, xmlAttrTemp->Value());
					}
				}
				if (localName.compare("RENDER") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)localElem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());

					if (Attrivlaue.compare("Bool") == 0)
					{
						bisRender = xmlAttrTemp->BoolValue();
					}
				}
				if (localName.compare("SCALE") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)localElem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());
	 
					if (Attrivlaue.compare("x") == 0)
					{
						scale.x = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("y") == 0)
					{
						scale.y = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("z") == 0)
					{
						scale.z = xmlAttrTemp->FloatValue();
					} 
				}
				if (localName.compare("ROT") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)localElem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());

					if (Attrivlaue.compare("x") == 0)
					{
						rotation.x = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("y") == 0)
					{
						rotation.y = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("z") == 0)
					{
						rotation.z = xmlAttrTemp->FloatValue();
					}
				}
				if (localName.compare("POS") == 0)
				{
					XMLAttribute * xmlAttrTemp = (XMLAttribute *)localElem->FirstAttribute();
					std::string Attrivlaue(xmlAttrTemp->Name());

					if (Attrivlaue.compare("x") == 0)
					{
						position.x = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("y") == 0)
					{
						position.y = xmlAttrTemp->FloatValue();
					}
					xmlAttrTemp = (XMLAttribute *)xmlAttrTemp->Next();
					Attrivlaue = xmlAttrTemp->Name();
					if (Attrivlaue.compare("z") == 0)
					{
						position.z = xmlAttrTemp->FloatValue();
					}
				}

			}
		}
		cCollisionShape* shape;
		if (bIsShape)
		{
			pColliderComp->AddCollisionSphere(name);
			shape = pColliderComp->GetClassCollisionSphere(name);

		}
		else
		{
			pColliderComp->AddCollisionBox(name);
			shape = pColliderComp->GetCollisionBox(name);
		}
		if (!shape) return;
		shape->SetLocalScale(scale);
		shape->SetLocalRotation(rotation);
		shape->SetLocalPosition(position);
		shape->SetParentName(parent_name);
		shape->SetIsRender(bisRender);

		if (strcmp(parent_name, "NULL") == 0)
		{
			D3DXMATRIXA16* pMat = pObject->GetPWorldMatrix();
			shape->SetParentMatrix(pMat);
		}
		else
		{
			cSkinnedMeshComponent* skinnedComp = pObject->GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
			cFbxComponent* fbxComp = pObject->GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);
			if (skinnedComp != NULL)
			{
				D3DXMATRIXA16* pMat = skinnedComp->GetMatrixFromeFrame(parent_name);
				shape->SetParentMatrix(pMat);
			}
			else if (fbxComp != NULL)
			{
				D3DXMATRIXA16* pMat = fbxComp->GetMatrixFromeFrame(parent_name);
				shape->SetParentMatrix(pMat);
			}
		}
	}
	void cSceneParser::TerrainComponentLoad(const XMLNode * parent, GameObject* pObject)
	{
		cTerrainComponent* pTerrainComponent = NULL;
		XMLElement * elem;
		char szfilePath[256];
		char szFile[256];
		float fcell = 1;
		bool bIsFog = 0;

		for (XMLNode * child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string szEleXML(elem->Name());
				if (szEleXML.compare("FILE_FATH") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string foldervlaue(xmlTemp->Name());
					if (foldervlaue.compare("Name") == 0)
					{
						strcpy(szFile, xmlTemp->Value());
						std::string szpath("Terrain/");
						szpath += szFile;
						strcpy(szfilePath, szpath.c_str());
					}
				}
				else if (szEleXML.compare("CELL") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string szVlaue(xmlTemp->Name());
					if (szVlaue.compare("Spacing") == 0)
					{
						fcell = xmlTemp->FloatValue();
					}
				}
				else if (szEleXML.compare("FOG") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string szVlaue(xmlTemp->Name());
					if (szVlaue.compare("Bool") == 0)
					{
						bIsFog = xmlTemp->BoolValue();
					}
				}
			}
		}
		if (g_pTerrainManager->AddTerrain(szfilePath, szFile, fcell, bIsFog))
		{
			pTerrainComponent = new cTerrainComponent(pObject);
			pTerrainComponent->Setup(szfilePath, szFile);
			pObject->AddComponent(pTerrainComponent);
		}
		else
		{
			MessageBox(0,L"Terrain Component_Scene",0,0);
		}

	}
	void cSceneParser::FbxComponentLoad(const XMLNode * parent, GameObject* pObject)
	{
		cFbxComponent* pFbxComponent = NULL;
		XMLElement * elem;

		char szfolderPath[256];
		char szfilePath[256];

		for (XMLNode * child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string vlaue(child->Value());
				if (vlaue.compare("FOLDER") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string foldervlaue(xmlTemp->Name());
					if (foldervlaue.compare("Name") == 0)
					{
						strcpy(szfolderPath, xmlTemp->Value());
					}
				}
				else if (vlaue.compare("FILE_FATH") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string foldervlaue(xmlTemp->Name());
					if (foldervlaue.compare("Name") == 0)
					{
						strcpy(szfilePath, xmlTemp->Value());
						pFbxComponent = new cFbxComponent(pObject, szfolderPath, szfilePath);
					}
				}
				else if (vlaue.compare("MATERIAL") == 0)
				{
					XMLNode * material;
					XMLElement * materialElem;

					for (material = (XMLNode*)child->FirstChild(); material != 0; material = (XMLNode*)material->NextSibling())
					{
						char szMulName[512];
						char szMulPath[512];
						D3DMATERIAL9 mtl;

						if (materialElem = material->ToElement())
						{
							strcpy(szMulName,materialElem->Value());
							//path 
							XMLAttribute * xmlTemp = (XMLAttribute *)materialElem->FirstAttribute();
							std::string temp(xmlTemp->Name());
							if (temp.compare("Path") == 0)
							{
								strcpy(szMulPath, xmlTemp->Value());
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							temp = xmlTemp->Name();
							if (temp.compare("Diffuse.R") == 0)
							{
								mtl.Diffuse.r = xmlTemp->FloatValue();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							temp = xmlTemp->Name();
							if (temp.compare("Diffuse.G") == 0)
							{
								mtl.Diffuse.g = xmlTemp->FloatValue();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							temp = xmlTemp->Name();
							if (temp.compare("Diffuse.B") == 0)
							{
								mtl.Diffuse.b = xmlTemp->FloatValue();
							}
							xmlTemp = (XMLAttribute *)xmlTemp->Next();
							temp = xmlTemp->Name();
							if (temp.compare("Diffuse.A") == 0)
							{
								mtl.Diffuse.a = xmlTemp->FloatValue();
							}
						}
						if (pFbxComponent != NULL)
							pFbxComponent->SetMeterialTexture(szMulName, szMulPath, mtl.Diffuse.r, mtl.Diffuse.g, mtl.Diffuse.b, mtl.Diffuse.a);
					}
				}
			}
		}

		pObject->AddComponent(pFbxComponent);

	}
	void cSceneParser::TranslateComponentLoad(const XMLNode * parent, GameObject* pObject)
	{
		XMLNode * child;
		XMLElement * elem;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		for (child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			if (elem = child->ToElement())
			{
				std::string vlaue(child->Value());
				if (vlaue.compare("SCALE") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string tempValue(xmlTemp->Name());
					if (tempValue.compare("x") == 0)
					{
						x = xmlTemp->FloatValue();
					}
					xmlTemp = (XMLAttribute *)xmlTemp->Next();
					tempValue = xmlTemp->Name();
					if (tempValue.compare("y") == 0)
					{
						y = xmlTemp->FloatValue();
					}
					xmlTemp = (XMLAttribute *)xmlTemp->Next();
					tempValue = xmlTemp->Name();
					if (tempValue.compare("z") == 0)
					{
						z = xmlTemp->FloatValue();
					}
					pObject->SetScale(x, y, z);
				}
				else if (vlaue.compare("ROT") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string tempValue(xmlTemp->Name());
					if (tempValue.compare("x") == 0)
					{
						x = xmlTemp->FloatValue();
					}
					xmlTemp = (XMLAttribute *)xmlTemp->Next();
					tempValue = xmlTemp->Name();
					if (tempValue.compare("y") == 0)
					{
						y = xmlTemp->FloatValue();
					}
					xmlTemp = (XMLAttribute *)xmlTemp->Next();
					tempValue = xmlTemp->Name();
					if (tempValue.compare("z") == 0)
					{
						z = xmlTemp->FloatValue();
					}
					pObject->SetRotation(x, y, z);
				}
				else if (vlaue.compare("POS") == 0)
				{
					XMLAttribute * xmlTemp = (XMLAttribute *)elem->FirstAttribute();
					std::string tempValue(xmlTemp->Name());
					if (tempValue.compare("x") == 0)
					{
						x = xmlTemp->FloatValue();
					}
					xmlTemp = (XMLAttribute *)xmlTemp->Next();
					tempValue = xmlTemp->Name();
					if (tempValue.compare("y") == 0)
					{
						y = xmlTemp->FloatValue();
					}
					xmlTemp = (XMLAttribute *)xmlTemp->Next();
					tempValue = xmlTemp->Name();
					if (tempValue.compare("z") == 0)
					{
						z = xmlTemp->FloatValue();
					}
					pObject->SetTranslate(x, y, z);
				}
			}
		}
	}

	void cSceneParser::Attribute(const XMLAttribute * firstAttr, GameObject* pObject)
	{
		//float x, float y, float z;
		//for (XMLAttribute * attr = (XMLAttribute *)firstAttr; attr != 0; attr = (XMLAttribute *)attr->Next())
		//{
		//	std::string vlaue(attr->Name());
		//	if(vlaue.compare("x") == 0)
		//	{
		//		/*MessageBox(0, UTIL::CharToTCHAR(attr->Name()), 0, 0);
		//		MessageBox(0, UTIL::CharToTCHAR(attr->IntValue()), 0, 0);*/
		//		x = attr->FloatValue();
		//	}
		//	else if (vlaue.compare("y") == 0)
		//	{
		//		y = attr->FloatValue();
		//	}
		//	else if (vlaue.compare("z") == 0)
		//	{
		//		z = attr->FloatValue();
		//	}

		//}
	}
}