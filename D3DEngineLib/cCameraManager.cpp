#include "Common.h"
#include "cCameraManager.h"
#include "cCameraComponent.h"


namespace ENGINE
{

	cCameraManager::cCameraManager()
	{
		m_currentCamera = NULL;
		m_vecCamera.clear();
	}


	cCameraManager::~cCameraManager()
	{
	}

	void cCameraManager::AddCamera(cCameraComponent* camera)
	{
		m_vecCamera.push_back(camera);
	}


	void cCameraManager::EraseCamera(cCameraComponent* camera)
	{
		std::vector<cCameraComponent*>::iterator iter;
		for (iter = m_vecCamera.begin(); iter != m_vecCamera.end(); ++iter)
		{
			if (*iter == camera)
			{
				m_vecCamera.erase(iter);
				return;
			}
		}
	}

	void cCameraManager::ChangeCamera(cCameraComponent* camera)
	{
		if (m_currentCamera) 
			m_currentCamera->SetViewThisCamera(false);
		m_currentCamera = camera;
		m_currentCamera->SetViewThisCamera(true);
	}
}