#pragma once

namespace ENGINE
{

	class cCameraComponent;

	class cNumberBillBoard
	{
	private:
		struct ST_PT_VERTEX
		{
			D3DXVECTOR3 p;
			D3DXVECTOR2 t;
			static const DWORD FVF;
		};

	public:
		void Init(LPDIRECT3DTEXTURE9 tex);	//이 클래스가 담당할 텍스쳐

		void Render(cCameraComponent* camera, D3DXVECTOR3 pos);

		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }

		cNumberBillBoard();
		~cNumberBillBoard();

		
	private:

		const int m_width = 10;
		const int m_height = 10;

		D3DXVECTOR3				m_Origin;
		LPDIRECT3DTEXTURE9		m_pTex;
		std::vector<ST_PT_VERTEX>	m_Vertex;



	};

}