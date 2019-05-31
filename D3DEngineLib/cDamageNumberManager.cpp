#include "Common.h"
#include "cDamageNumberManager.h"


namespace ENGINE
{

	cDamageNumberManager::cDamageNumberManager()
	{
		for (int i = 0; i < 10; i++)
		{
			numberTex[i] = NULL;
			m_num[i] = new cNumberBillBoard;
		}

		m_vecDamage.clear();
		CreateTexture();
	}


	cDamageNumberManager::~cDamageNumberManager()
	{
		for (int i = 0; i < 10; i++)
		{
			SAFE_DELETE(m_num[i]);
			SAFE_RELEASE(numberTex[i]);
		}

		for (int i = 0; i < m_vecDamage.size(); i++)
		{
			SAFE_DELETE(m_vecDamage[i]);
		}
	}

	void cDamageNumberManager::CreateTexture()
	{
		wchar_t fileName[256];
		for (int i = 0; i < 10; i++)
		{
			wsprintf(fileName, L"Number/damage_%d.bmp", i);

			D3DXCreateTextureFromFileEx(
				g_pD3DDevice,
				fileName,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				NULL,
				NULL,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_XRGB(255, 0, 255),
				NULL,
				NULL,
				&numberTex[i]);

			m_num[i]->Init(numberTex[i]);
		}

		
	}

	void cDamageNumberManager::Play(int num, float destroyTime, D3DXVECTOR3& pos)
	{
		cDamageNumber* damage = new cDamageNumber(m_num[0]);
		m_vecDamage.push_back(damage);
		damage->Play(num, destroyTime, pos);

	}

	void cDamageNumberManager::Update()
	{

		for (int i = 0; i < m_vecDamage.size(); i++)
		{
			if (m_vecDamage[i]->GetIsPlay())
			{
				m_vecDamage[i]->Update();
			}
			else
			{
				SAFE_DELETE(m_vecDamage[i]);
				m_vecDamage.erase(m_vecDamage.begin() + i);
			}
		}

	}

	void cDamageNumberManager::Render(cCameraComponent* camera)
	{
		g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, D3DCOLOR_XRGB(0, 0, 0));
		g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);

		g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		for (int i = 0; i < m_vecDamage.size(); i++)
		{
			m_vecDamage[i]->Render(camera);
		}

		g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

}