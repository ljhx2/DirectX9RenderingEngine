#pragma once
#include "tinyxml2\tinyxml2.h"

using namespace tinyxml2;
namespace ENGINE
{
	class GameObject;
	class cStaticMeshComponent;
	class cSkinnedMesh;
	class cFbxComponent;
	class cColliderComponent;
	class cAnimComponent;
	class cScriptComponent;
	class cSkyBoxComponent;
	class cTerrainComponent;

	class cSceneParser
	{
	public:
		void SaveScene(char* fileName);

		void AttachBipNameSave(tinyxml2::XMLElement * GameObjectElement, GameObject* pObject);
		void TranslateComponentSave(tinyxml2::XMLElement * GameObjectElement, GameObject* pObject);
		void StaticMeshComponentSave(tinyxml2::XMLElement * GameObjectElement, cStaticMeshComponent* pComponent);
		void SkinnedMeshComponentSave(tinyxml2::XMLElement * GameObjectElement, cSkinnedMesh* pComponen);
		void FbxComponentSave(tinyxml2::XMLElement * GameObjectElement, cFbxComponent* pComponent);
		void TerrainComponentSave(tinyxml2::XMLElement * GameObjectElement, cTerrainComponent* pComponent);
		void SkyBoxComponentSave(tinyxml2::XMLElement * GameObjectElement, cSkyBoxComponent* pComponent);
		void ColliderComponentSave(tinyxml2::XMLElement * GameObjectElement, cColliderComponent* pComponent);
		void LightComponentSave(tinyxml2::XMLElement * xmlGameObjectElement, cLightComponent* pComponent);

		void AnimationComponentSave(tinyxml2::XMLElement * GameObjectElement, cAnimComponent* pComponent);
		void ScriptComponentSave(tinyxml2::XMLElement * GameObjectElement, cScriptComponent* pComponent);

		void GameObjectSave(tinyxml2::XMLElement * xmlGameObjectElement, GameObject* pObject);

	public :
		//여기서 씬 자체를 로드 해야 하는 문제
		void LoadScene(char* fileName);
		void GameObjectLoad(const XMLNode * parent, GameObject* pObject);

		void Attribute(const XMLAttribute * firstAttr, GameObject* pObject);
		void TranslateComponentLoad(const XMLNode * parent, GameObject* pObject);
		void FbxComponentLoad(const XMLNode * parent, GameObject* pObject);
		void TerrainComponentLoad(const XMLNode * parent, GameObject* pObject);
		void ColliderComponentLoad(const XMLNode * parent, GameObject* pObject);
		void ColliderLocalLoad(const XMLElement * parent, GameObject* pObject, cColliderComponent* pColliderComp);
		void LightComponentLoad(const XMLNode * parent, GameObject* pObject);
		void SkyBoxComponentLoad(const XMLNode * parent, GameObject* pObject);
	public :
		cSceneParser();
		~cSceneParser();

	private :
		GameObject * m_pRootGameObject;
		tinyxml2::XMLDocument m_XMLDoc;
		GameObject* m_pObjTemp = NULL;

	};
}