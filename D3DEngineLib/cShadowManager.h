#pragma once
#include <list>

#define g_pShadowManager cShadowManager::GetInstance()


namespace ENGINE
{
	class GameObject;
	class cFrustum;

	class cShadowManager
	{
	private:
		cShadowManager();
		~cShadowManager();
	public:
		static cShadowManager* GetInstance() { static cShadowManager instance; return &instance; }

		void Destroy();

	private:
		LPDIRECT3DTEXTURE9		m_pShadowRenderTarget[3];
		LPDIRECT3DSURFACE9		m_pShadowDepthStencil[3];
		LPD3DXSPRITE			m_pSprite[3];

		D3DXMATRIXA16			m_matDirLightViewProj[3];


		std::list<GameObject*> m_listGameObject[3];

		const int shadowMapSize = 2048;

		cFrustum* m_pCompleteFrustum;
		cFrustum* m_pSplitFrustum[3];
		cFrustum* m_pLightFrustum[3];

		cFrustum* m_pCollisionFrustum[3];

		ID3DXMesh* mesh[3];//�������� Ȯ�ο� �޽�

	public:
		LPDIRECT3DTEXTURE9 GetShadowMap(int i) { return m_pShadowRenderTarget[i]; }

		void CreateShadowMap(LPDIRECT3DDEVICE9 device);

		void AllocGameObjectToShadowMap(D3DXVECTOR3& center, float radius, GameObject* obj);	//���ӿ�����Ʈ�� � ������ʿ� ���������� ���Ѵ�.


		void Update();	//����Ʈ�� Update���� ���� ����Ǿ����

		void RenderShadowMap(LPDIRECT3DDEVICE9 device);
		void ShadowMapRender(LPDIRECT3DDEVICE9 device);


	};

}