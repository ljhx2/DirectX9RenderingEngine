#include "Common.h"
#include "cShadowManager.h"
#include "include\D3DRenderer.h"
#include "cFrustum.h"

namespace ENGINE
{

	cShadowManager::cShadowManager()
	{
		for (int i = 0; i < 3; i++)
		{
			m_pShadowRenderTarget[i] = NULL;
			m_pShadowDepthStencil[i] = NULL;
			m_pSprite[i] = NULL;

			m_pSplitFrustum[i] = new cFrustum;
			m_pLightFrustum[i] = new cFrustum;
			m_pCollisionFrustum[i] = new cFrustum;

			D3DXCreateSphere(g_pD3DDevice, 10.0f, 6U, 6U, &mesh[i], 0);
		}

		m_pCompleteFrustum = new cFrustum;

	}


	cShadowManager::~cShadowManager()
	{
	}

	void cShadowManager::Destroy()
	{
		for (int i = 0; i < 3; i++)
		{
			SAFE_RELEASE(m_pShadowRenderTarget[i]);
			SAFE_RELEASE(m_pShadowDepthStencil[i]);
			SAFE_RELEASE(m_pSprite[i]);

			SAFE_DELETE(m_pSplitFrustum[i]);
			SAFE_DELETE(m_pLightFrustum[i]);
			SAFE_DELETE(m_pCollisionFrustum[i]);

			SAFE_RELEASE(mesh[i]);
		}

		SAFE_DELETE(m_pCompleteFrustum);
	}

	void cShadowManager::CreateShadowMap(LPDIRECT3DDEVICE9 device)
	{
		for (int i = 0; i < 3; i++)
		{
			device->CreateTexture(
				shadowMapSize,
				shadowMapSize,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_R32F,			//��� �÷� ���۸� Red �� �ٻ�� ( 4byte float �Ǽ��� ��� )
				D3DPOOL_DEFAULT,
				&m_pShadowRenderTarget[i],
				NULL);


			// �׸��� �ʰ� ������ ũ���� ���̹��۵� �������� �Ѵ�.
			device->CreateDepthStencilSurface(
				shadowMapSize,
				shadowMapSize,
				D3DFMT_D24S8,
				D3DMULTISAMPLE_NONE,
				0,
				TRUE,
				&m_pShadowDepthStencil[i],
				NULL);


			D3DXCreateSprite(device, &m_pSprite[i]);
		}
	}

	void cShadowManager::AllocGameObjectToShadowMap(D3DXVECTOR3& center, float radius, GameObject* obj)
	{
		
		for (int i = 0; i < 3; i++)
		{
			if (m_pCollisionFrustum[i]->IsInSphere(&center, radius))
			{
				m_listGameObject[i].push_back(obj);
				obj->SetIsRender(true);
				//return;
			}
		}
		
	}

	void cShadowManager::Update()
	{
		for (int i = 0; i < 3; i++)
		{
			m_listGameObject[i].clear();
		}

		cCameraComponent* camera = g_pCameraManager->GetCurrentCamera();
		if (camera == NULL)	return;
		//test
		//cCameraComponent* camera = g_pCameraManager->GetCamera(0);


		D3DXMATRIXA16 view = camera->GetMatView();
		D3DXMATRIXA16 proj = camera->GetMatProj();
		//test
		//static D3DXMATRIXA16 view = view2;
		//static D3DXMATRIXA16 proj = proj2;
		D3DXMATRIXA16 viewProj;
		m_pCompleteFrustum->Make(&viewProj);

		float fovy = camera->GetFovy();
		float width = camera->GetWidth();
		float height = camera->GetHeight();
		float z_near = camera->GetZNear();
		float z_far = camera->GetZFar();
		D3DXVECTOR3 cameraPos = camera->GetPosition();


		D3DXMatrixPerspectiveFovLH(&proj, fovy, width / height, z_near, z_near + 1000);
		viewProj = view * proj;
		m_pSplitFrustum[0]->Make(&viewProj);

		D3DXMatrixPerspectiveFovLH(&proj, fovy, width / height, z_near + 500, z_near + 1800);
		viewProj = view * proj;
		m_pSplitFrustum[1]->Make(&viewProj);

		D3DXMatrixPerspectiveFovLH(&proj, fovy, width / height, z_near + 2000, z_far);
		viewProj = view * proj;
		m_pSplitFrustum[2]->Make(&viewProj);


		//collisionFrustum

		width = m_pSplitFrustum[0]->GetFarPlanWidth();
		height = m_pSplitFrustum[0]->GetFarPlanHeight();
		D3DXMatrixOrthoLH(&proj, width * 1.5f, height * 1.5f, z_near - 200, z_near + 700);
		viewProj = view * proj;
		m_pCollisionFrustum[0]->Make(&viewProj);

		width = m_pSplitFrustum[1]->GetFarPlanWidth();
		height = m_pSplitFrustum[1]->GetFarPlanHeight();
		D3DXMatrixOrthoLH(&proj, width * 1.5f, height * 1.5f, z_near, z_near + 1500);
		viewProj = view * proj;
		m_pCollisionFrustum[1]->Make(&viewProj);

		width = m_pSplitFrustum[2]->GetFarPlanWidth();
		height = m_pSplitFrustum[2]->GetFarPlanHeight();
		D3DXMatrixOrthoLH(&proj, width * 1.5f, height * 1.5f, z_near + 1000, z_far);
		viewProj = view * proj;
		m_pCollisionFrustum[2]->Make(&viewProj);


		g_pShaderManager->SetCamera(cameraPos, z_near + 700, z_near + 1500);
	}

	void cShadowManager::RenderShadowMap(LPDIRECT3DDEVICE9 device)
	{
		//���� ī�޶�� matLightProj�� shader�� set���־���Ѵ�
		cLightComponent* light = g_pLightManager->GetLight(0);
		cCameraComponent* camera = g_pCameraManager->GetCurrentCamera();
		if (light != NULL && camera != NULL)
		{
			//----- ��
			D3DXVECTOR3 lightPos = light->GetPosition();
			D3DXVECTOR3 lightDir = light->GetDirection();
			D3DXVECTOR4 lightColor = light->GetDiffuseColor();
			//----- ī�޶�
			D3DXVECTOR3 cameraPos = camera->GetPosition();
			D3DXVECTOR3 cameraFront = camera->GetFront();

			for (int i = 0; i < 3; i++)
			{
				//frustum0
				D3DXMATRIXA16 matDirLightView;
				D3DXVECTOR3 frustumCenter = m_pCollisionFrustum[i]->GetCenter();
				D3DXVECTOR3 shadowMapPosition = frustumCenter - (lightDir * 10000);
				D3DXMatrixLookAtLH(&matDirLightView, &shadowMapPosition, &frustumCenter, &D3DXVECTOR3(0, 1, 0));

				D3DXMATRIXA16 matDirLightProj;

				/*
				������������� �ʺ���̰��� ���庯ȯ���� ��ģ ����ü ��������
				�������� ����ķ� ��ȯ���Ѽ� 8�������� xy���� ����ū���� ��������
				�����Ͽ� ���ϸ� �ȴ�
				*/
				D3DXVECTOR3 point[8];
				for (int j = 0; j < 8; j++)
				{
					point[j] = *(m_pCollisionFrustum[i]->GetPoint(j));
					D3DXVec3TransformCoord(&point[j], &point[j], &matDirLightView);
				}
				float minX, minY, maxX, maxY;
				minX = maxX = point[0].x;
				minY = maxY = point[0].y;
				for (int j = 0; j < 8; j++)
				{
					if (minX > point[j].x) minX = point[j].x;
					if (maxX < point[j].x) maxX = point[j].x;
					if (minY > point[j].y) minY = point[j].y;
					if (maxY < point[j].y) maxY = point[j].y;
				}

				float width = maxX - minX;
				float height = maxY - minY;

				D3DXMatrixOrthoLH(&matDirLightProj, width, height, 0.1f, 15000.f);

				m_matDirLightViewProj[i] = matDirLightView * matDirLightProj;

				m_pLightFrustum[i]->Make(&m_matDirLightViewProj[i]);
			}

			g_pShaderManager->SetLight(lightPos, lightDir, lightColor,
				&m_matDirLightViewProj[0], &m_matDirLightViewProj[1], &m_matDirLightViewProj[2]);

		}


		//���� Device Surface ���� Target ���� ���
		LPDIRECT3DSURFACE9		pDeviceTarget = NULL;
		LPDIRECT3DSURFACE9		pDeviceDepthStencil = NULL;
		device->GetRenderTarget(0, &pDeviceTarget);
		device->GetDepthStencilSurface(&pDeviceDepthStencil);

		for (int i = 0; i < 3; i++)
		{
			//Shadow �׸� Texture �� ���� Ÿ�� ����
			LPDIRECT3DSURFACE9	pTargetSurface;
			if (SUCCEEDED(m_pShadowRenderTarget[i]->GetSurfaceLevel(0, &pTargetSurface)))
			{
				device->SetRenderTarget(0, pTargetSurface);
				SAFE_RELEASE(pTargetSurface);
			}
			device->SetDepthStencilSurface(m_pShadowDepthStencil[i]);

			//���� �����ӿ� �׷ȴ� �׸��� ������ ����
			device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				0xffffffff, 1.0f, 0);

			g_pShaderManager->SetCreateShadowMatrix(&m_matDirLightViewProj[i]);

			//��������� �׷��ش�
			std::list<GameObject*>::iterator iter;
			for (iter = m_listGameObject[i].begin(); iter != m_listGameObject[i].end(); ++iter)
			{
				(*iter)->PrevRender();
			}



		}

		//�ٽ� ������ ����ۿ� ���� ���۸� ����Ѵ�.
		device->SetRenderTarget(0, pDeviceTarget);
		device->SetDepthStencilSurface(pDeviceDepthStencil);
		SAFE_RELEASE(pDeviceTarget);
		SAFE_RELEASE(pDeviceDepthStencil);



	}

	void cShadowManager::ShadowMapRender(LPDIRECT3DDEVICE9 device)
	{
		//test
		//m_pLightFrustum[1]->Draw();
		//m_pSplitFrustum[0]->Draw();
		//m_pSplitFrustum[2]->Draw();


		for (int i = 0; i < 3; i++)
		{
			//test ��������
			//D3DXMATRIXA16 old;
			//device->GetTransform(D3DTS_WORLD, &old);
			//D3DXVECTOR3 frustumCenter = m_pSplitFrustum[i]->GetCenter();
			//D3DXMATRIXA16 world;
			//D3DXMatrixTranslation(&world, frustumCenter.x, frustumCenter.y, frustumCenter.z);
			//device->SetTransform(D3DTS_WORLD, &world);
			//mesh[i]->DrawSubset(0);
			//device->SetTransform(D3DTS_WORLD, &old);


			m_pSprite[i]->Begin(D3DXSPRITE_ALPHABLEND);
			RECT rcSour = { 0, 0, shadowMapSize, shadowMapSize };
			RECT rcDest = { i * 205, 0, 200 + (i * 205), 200 };
			float destWidth = rcDest.right - rcDest.left;
			float destHeight = rcDest.bottom - rcDest.top;
			float sourWidth = rcSour.right - rcSour.left;
			float sourHeight = rcSour.bottom - rcSour.top;

			//���� ������ ���� ������ ��
			float scaleX = destWidth / sourWidth;
			float scaleY = destHeight / sourHeight;

			//������ ���
			D3DXMATRIXA16 matS;
			D3DXMatrixScaling(&matS,
				scaleX, scaleY, 1.0f);

			//�̵����
			D3DXMATRIXA16 matTrans;
			D3DXMatrixTranslation(
				&matTrans,
				rcDest.left, rcDest.top, 0.0f);

			D3DXMATRIXA16 matFinal = matS * matTrans;
			m_pSprite[i]->SetTransform(&matFinal);

			//�̹��� �׷���
			m_pSprite[i]->Draw(
				m_pShadowRenderTarget[i],
				&rcSour,
				&D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(0, 0, 0),	//�׸� ��ġ�� 0 �̳�....
				0xffffffff);
			m_pSprite[i]->End();
		}
	}

}