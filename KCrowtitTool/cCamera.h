#pragma once


class cCamera
{
public:
	void Setup(D3DXVECTOR3* pTarget = NULL);
	void Update();
	
	void UpdateProj();
	void SetFovy(float value) { m_fovy = value; UpdateProj(); }
	void SetAspect(float value) { m_aspect = value; UpdateProj(); }
	void SetZNear(float value) { m_z_near = value; UpdateProj(); }
	void SetZFar(float value) { m_z_far = value; UpdateProj(); }
	void SetViewThisCamera(bool b) { viewThisCamera = b; UpdateProj(); }
	float GetFovy() { return m_fovy; }
	float GetAspect() { return m_aspect; }
	float GetZNear() { return m_z_near; }
	float GetZFar() { return m_z_far; }
	bool GetViewThisCamera() { return viewThisCamera; }
	
	cCamera();
	~cCamera();

private:
	D3DXVECTOR3*		m_pTarget;

	D3DXMATRIXA16			m_mView;
	D3DXMATRIXA16			m_mProj;
	D3DXVECTOR3				m_vEye;		//카메라 위치
	D3DXVECTOR3				m_vLook;	//바라보는 ㅊ점
	D3DXVECTOR3				m_vUp;		//월드좌표의 업벡터

	float m_fovy;
	float m_aspect;
	float m_z_near;
	float m_z_far;

	bool viewThisCamera;

	bool					m_isLButtonDown;
	POINT					m_ptPrevMouse;
	float					m_fDist;
	float					m_fAngleX;
	float					m_fAngleY;
	D3DXVECTOR3				m_vOffset;
};

