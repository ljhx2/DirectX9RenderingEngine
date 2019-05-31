#pragma once

namespace ENGINE
{
	enum ESHAPE_TYPE
	{
		eBOX_SHAPE,
		eSPHERE_SHAPE
	};
	class cColliderComponent;
	class GameObject;

	class cCollisionShape
	{
	public:
		virtual bool CheckCollision(cCollisionShape& other) = 0;
		virtual bool CheckCollision(cCollisionShape* other) = 0;

		void SetShapeType(ESHAPE_TYPE type) { m_shapeTpye = type; }
		void SetName(std::string name) { m_name = name; }
		void SetLocalScale(D3DXVECTOR3 scale) { m_localScale = scale; UpdateLocalWorld(); }
		void SetLocalRotation(D3DXVECTOR3 rotation) { m_localRotation = rotation; UpdateLocalWorld(); }
		void SetLocalPosition(D3DXVECTOR3 position) { m_localPosition = position; UpdateLocalWorld(); }
		void SetParentMatrix(D3DXMATRIXA16* pParnet) { m_pParentWorld = pParnet; }
		void SetParentName(std::string name) { m_parentName = name; }
		ESHAPE_TYPE GetShapeType() { return m_shapeTpye; }
		std::string GetName() { return m_name; }
		D3DXVECTOR3 GetLocalScale() { return m_localScale; }
		D3DXVECTOR3 GetLocalRotation() { return m_localRotation; }
		D3DXVECTOR3 GetLocalPosition() { return m_localPosition; }
		std::string GetParentName() { return m_parentName; }
		cColliderComponent* GetOwnerColliderComponent() { return m_pOwnerColliderComponent; }
		GameObject* GetOwnerGameObject();

		void SetIsRender(bool isRender) { m_isRender = isRender; }
		bool GetIsRender() { return m_isRender; }

		void UpdateLocalWorld();	//S, R, T 에 변경이 있으면 호출

		void Render();

		void Init();

		void Clashed(cCollisionShape* col);
		void ClearClashedVec();

		cCollisionShape();
		virtual ~cCollisionShape();



	protected:
		ESHAPE_TYPE		m_shapeTpye;

		std::string		m_name;

		D3DXVECTOR3		m_localScale;
		D3DXVECTOR3		m_localRotation;
		D3DXVECTOR3		m_localPosition;
		D3DXMATRIXA16	m_localWorld;

		D3DXMATRIXA16*	m_pParentWorld;
		std::string		m_parentName;

		ID3DXMesh*		m_mesh;

		bool			m_isRender;

		cColliderComponent* m_pOwnerColliderComponent;

		std::vector<cCollisionShape*> m_vecPrevClashed;
		std::vector<cCollisionShape*> m_vecClashed;
		int vecPoint;
		int Max_vecPoint;

	};


}