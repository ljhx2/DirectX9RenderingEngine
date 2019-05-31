#pragma once
#include "cNumberBillBoard.h"
#include "cDamageNumber.h"

#define g_pDamageNumberManager cDamageNumberManager::GetInstance()

namespace ENGINE
{
	class cCameraComponent;

	class cDamageNumberManager
	{
	private:
		cDamageNumberManager();
		~cDamageNumberManager();

	public:
		static cDamageNumberManager* GetInstance() { static cDamageNumberManager instance; return &instance; }


	public:
		void CreateTexture();

		void Play(int num, float destroyTime, D3DXVECTOR3& pos);

		void Update();
		void Render(cCameraComponent* camera);

		cNumberBillBoard* GetBillBoard(int i) { return m_num[i]; }

	private:
		LPDIRECT3DTEXTURE9 numberTex[10];

		cNumberBillBoard* m_num[10];

		std::vector<cDamageNumber*> m_vecDamage;
	};

}