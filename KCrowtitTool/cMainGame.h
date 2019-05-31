#pragma once

class cGrid;
class cWorkingCamera;

void OnCollisionEnter(ENGINE::cCollisionShape* my, ENGINE::cCollisionShape* target);
void OnCollisionStay(ENGINE::cCollisionShape* my, ENGINE::cCollisionShape* target);


class cMainGame
{
public:
	cMainGame();
	~cMainGame();

	void Setup(HWND hWnd);
	void Update();
	void Render();
	
	cWorkingCamera* GetCamera() { return m_pWorkingCamera; }
private:
	ENGINE::D3DRenderer* _renderer;

	cWorkingCamera* m_pWorkingCamera;

	cGrid* m_grid;

	ENGINE::GameObject* player;

	bool isShadowMapRender;
};

