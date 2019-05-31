#pragma once

#include "../D3DEngineLib/cCameraComponent.h"

class cWorkingCamera : public ENGINE::cCameraComponent
{
public:

	virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
	virtual void Render();
	void UpdateProj();

	void SetAspect(float value) { m_aspect = value; UpdateProj(); }
	float GetAspect() { return m_aspect; }


	cWorkingCamera(HWND hWnd);
	~cWorkingCamera();

private:
	float m_aspect;

	
	bool m_isLButtonDown;
	POINT m_ptPrevMouse;
	float m_fDist;
};

