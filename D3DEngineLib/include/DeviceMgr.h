#pragma once

#define g_pD3DDevice DeviceMgr::GetInstance().GetDevice()

class DeviceMgr
{
	DeviceMgr(void);

	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pD3DDevice;

public:
	~DeviceMgr(void);

	static DeviceMgr& GetInstance();
	void CreateDevice(HWND hWnd);
	LPDIRECT3DDEVICE9 GetDevice();
	void Destroy();
};
