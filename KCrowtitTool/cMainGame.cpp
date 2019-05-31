#include "stdafx.h"
#include "cMainGame.h"
#include "cGrid.h"
#include "cInput.h"
#include "cWorkingCamera.h"

void OnCollisionEnter(ENGINE::cCollisionShape* my, ENGINE::cCollisionShape* target)
{
	g_pDebug->Log("CollisionEnter!!!! %s : %s", my->GetName().c_str(), target->GetName().c_str());
}
void OnCollisionStay(ENGINE::cCollisionShape* my, ENGINE::cCollisionShape* target)
{
	g_pDebug->Log("CollisionStay!!!!! %s : %s", my->GetName().c_str(), target->GetName().c_str());
}

//이게 씬이 됡 ㅓ같은데??? 
cMainGame::cMainGame()
	: m_grid(NULL)
	, m_pWorkingCamera(NULL)
{
	ENGINE::Start();
	_renderer = new ENGINE::D3DRenderer;
	
}
cMainGame::~cMainGame()
{
	ENGINE::Shutdown();
	SAFE_DELETE(_renderer);
	SAFE_DELETE(m_grid);
	SAFE_DELETE(m_pWorkingCamera);
	
}

void cMainGame::Setup(HWND hWnd)
{
	_renderer->CreateDevice(hWnd);

	// 조명 세팅
	D3DLIGHT9 DirLight;
	memset(&DirLight, 0, sizeof(D3DLIGHT9));

	DirLight.Type = D3DLIGHT_DIRECTIONAL;

	DirLight.Ambient.r = 0.8f;
	DirLight.Ambient.g = 0.8f;
	DirLight.Ambient.b = 0.8f;
	DirLight.Ambient.a = 1.0f;

	DirLight.Diffuse.r = 0.8f;
	DirLight.Diffuse.g = 0.8f;
	DirLight.Diffuse.b = 0.8f;
	DirLight.Diffuse.a = 1.0f;

	D3DXVECTOR3 Dir = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&Dir, &Dir);
	DirLight.Direction = Dir;

	g_pD3DDevice->SetLight(0, &DirLight);
	g_pD3DDevice->LightEnable(0, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//카메라
	//m_pCamera = new cCamera;
	//m_pCamera->Setup();
	m_pWorkingCamera = new cWorkingCamera(g_hwndGame);
	ENGINE::g_pCameraManager->ChangeCamera(m_pWorkingCamera);


	//그리드
	m_grid = new cGrid;
	m_grid->Setup(3, 20);


	player = ENGINE::Instantiate("FWR_ALL_0014.prefab");
	HTREEITEM rootNode = HierarchyView->GetTreeNode(ENGINE::GetSceneRoot());
	HierarchyView->InsertTreeNode(rootNode, player);
	HierarchyView->SelectTreeNode(player);

	//player->SetCallback_OnCollisionEnter(OnCollisionEnter);
	//player->SetCallback_OnCollisionStay(OnCollisionStay);

}
void cMainGame::Update()
{
	m_pWorkingCamera->Update(0.0f, NULL);

	
	if (cInput::GetKey('I'))
	{
		//ENGINE::g_pDamageNumberManager->Play(512, 3.0f, D3DXVECTOR3(0, 0, 0));
		if (isShadowMapRender) isShadowMapRender = false;
		else isShadowMapRender = true;
	}

	if (InspectorView && InspectorView->m_pSelectGameObject){
		D3DXVECTOR3 move(0, 0, 0);
		D3DXVECTOR3 rot(0, 0, 0);
		if (cInput::GetKey('W'))
		{
			move.z += 10.0f;
		}
		if (cInput::GetKey('S'))
		{
			move.z -= 10.0f;
		}
		if (cInput::GetKey('A'))
		{
			move.x -= 10.0f;
		}
		if (cInput::GetKey('D'))
		{
			move.x += 10.0f;
		}
		if (cInput::GetKey('R'))
		{
			move.y += 10.0f;
		}
		if (cInput::GetKey('F'))
		{
			move.y -= 10.0f;
		}
		if (cInput::GetKey('Q'))
		{
			rot.y -= 0.01f;
		}
		if (cInput::GetKey('E'))
		{
			rot.y += 0.01f;
		}
		InspectorView->m_pSelectGameObject->GetTransform()->Translation_World(move.x, move.y, move.z);
		InspectorView->m_pSelectGameObject->GetTransform()->Rotate_World(rot.x, rot.y, rot.z);

		
	}
	


}
void cMainGame::Render()
{
	_renderer->BeginFrame();

	if (m_grid)
		m_grid->Render();

	if (InspectorView->m_pSelectGameObject)
		InspectorView->m_pSelectGameObject->RenderGizmo(g_pD3DDevice);

	//g_pSceneManager->Render();

	if (isShadowMapRender)
	{
		ENGINE::g_pShadowManager->ShadowMapRender(g_pD3DDevice);
	}

	_renderer->EndFrame();
	_renderer->DisplayFrame();
}

