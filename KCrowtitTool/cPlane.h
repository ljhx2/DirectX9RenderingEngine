#pragma once 

#define G_SIZE 51
#define GRID_SPACE 1

#define GRID_START	-G_SIZE * GRID_SPACE

#define GRID_MAX_NUM G_SIZE

#define GRID_SIZE_X G_SIZE 
#define GRID_SIZE_Y G_SIZE 
#define GRID_SIZE_Z G_SIZE 
class cPlane
{
	std::vector<ST_PC_VERTEX>	m_VertexPC;

	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vAngle;

	D3DXMATRIXA16	m_Scale;
	D3DXMATRIXA16	m_Rotation;
	D3DXMATRIXA16	m_Translation;

	D3DXMATRIXA16	m_World;

	//미래를 위해 ㅋ
	D3DLIGHT9		m_Light;
	D3DMATERIAL9	m_Matrial;

public:
	cPlane();
	~cPlane();

	void Setup(int count);
	void Resize(float x, float y, float z);
	void Update();
	void Render();
};