#include "Common.h"
#include "include\D3DRenderer.h"
#include "IComponent.h"

namespace ENGINE
{
	IComponent::IComponent()
	{
		eComponent_Type = eEnd_COMPONENT_TYPE;
		m_pOwnerObject = NULL;
	}

	IComponent::IComponent(GameObject* pOwnerObject)
	{
		eComponent_Type = eEnd_COMPONENT_TYPE;
		m_pOwnerObject = pOwnerObject;
	}


	IComponent::~IComponent()
	{
	}
}