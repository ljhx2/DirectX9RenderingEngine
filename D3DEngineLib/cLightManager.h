#pragma once

#define g_pLightManager cLightManager::GetInstance()

namespace ENGINE
{
	class cLightComponent;

	class cLightManager
	{
	private:
		cLightManager();
		~cLightManager();

	public:
		static cLightManager* GetInstance() { static cLightManager instance; return &instance; }

	private:
		std::vector<cLightComponent*> m_vecLight;

	public:
		cLightComponent* GetLight(int i) 
		{ 
			if (i < m_vecLight.size() && i >= 0) 
				return m_vecLight[i]; 
			return NULL;
		}
		void AddLight(cLightComponent* light);
		void EraseLight(cLightComponent* light);

	};

}

