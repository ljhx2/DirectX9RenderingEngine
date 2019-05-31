#pragma once





namespace ENGINE
{

	enum ECOMPONENT_TYPE
	{
		eSTATIC_MESH_TYPE = 0,
		eSKINNED_MESH_TYPE,
		eFBX_MESH_TYPE,
		eTERRAIN_TYPE,
		eANIMATION_TYPE,
		eCOLLIDER_TYPE,
		eSKYBOX_TYPE,
		eLIGHT_TYPE,
		eCAMERA_TYPE,
		eSCRIPT_TYPE,

		eEnd_COMPONENT_TYPE
	};

	struct ST_AnimClip
	{
		std::string name;
		int			setIndex;
		DOUBLE		start;
		DOUBLE		end;
		bool		isLoop;
	};

	class GameObject;
	
	class IComponent
	{
	public:
		virtual void			Update(float deltaTime, D3DXMATRIXA16* matWorld) = 0;
		virtual void			PrevRender() {}
		virtual void			Render() = 0;
		virtual ECOMPONENT_TYPE GetType() { return eComponent_Type; }
		virtual GameObject*		GetOwnerObject() { return m_pOwnerObject; }

		IComponent();
		IComponent(GameObject* pOwnerObject);
		virtual ~IComponent();


	protected:
		ECOMPONENT_TYPE eComponent_Type;
		GameObject* m_pOwnerObject;

	};

}