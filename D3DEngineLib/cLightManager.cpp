#include "Common.h"
#include "cLightManager.h"


namespace ENGINE
{


	cLightManager::cLightManager()
	{
	}


	cLightManager::~cLightManager()
	{
	}

	void cLightManager::AddLight(cLightComponent* light)
	{
		for (int i = 0; i < m_vecLight.size(); i++)
		{
			if (m_vecLight[i] == light)
				return;
		}

		m_vecLight.push_back(light);
	}

	void cLightManager::EraseLight(cLightComponent* light)
	{
		for (int i = 0; i < m_vecLight.size(); i++)
		{
			if (m_vecLight[i] == light)
			{
				m_vecLight.erase(m_vecLight.begin() + i);
			}
		}
	}

}
