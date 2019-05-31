#pragma once

#define g_pCameraManager cCameraManager::GetInstance()

namespace ENGINE
{
	class cCameraComponent;

	class cCameraManager
	{
	private:
		cCameraManager();
		~cCameraManager();

	public:
		static cCameraManager* GetInstance() { static cCameraManager instance; return &instance; }

	private:
		std::vector<cCameraComponent*> m_vecCamera;
		cCameraComponent* m_currentCamera;

	public:
		cCameraComponent* GetCurrentCamera() { return m_currentCamera; }
		cCameraComponent* GetCamera(int i) { return m_vecCamera[i]; }

		void AddCamera(cCameraComponent* camera);
		void EraseCamera(cCameraComponent* camera);
		void ChangeCamera(cCameraComponent* camera);
	};

}