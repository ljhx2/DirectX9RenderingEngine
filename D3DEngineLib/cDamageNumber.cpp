#include "Common.h"
#include "cDamageNumber.h"
#include "cNumberBillBoard.h"
#include "cCameraComponent.h"

namespace ENGINE
{

	cDamageNumber::cDamageNumber(cNumberBillBoard* bill)
	{
		for (int i = 0; i < 10; i++)
		{
			billboard[i] = bill + i;
		}
		m_passedTime = 0.0f;

	}


	cDamageNumber::~cDamageNumber()
	{
	}

	void cDamageNumber::Play(int num, float destroyTime, D3DXVECTOR3& pos)
	{
		m_num = num;
		m_destroyTime = destroyTime;
		m_pos = pos;

		m_isPlay = true;


		while (m_num >= 1)
		{
			int temp = m_num % 10;
			vec_num.push_back(temp);
			m_num = m_num / 10;
		}

		m_width = vec_num.size() * billboard[0]->GetWidth();

	}

	void cDamageNumber::Update()
	{
		m_passedTime += g_pTimeManager->GetDeltaTime();

		m_pos.y += 0.5f;

		if (m_passedTime >= m_destroyTime)
		{
			m_isPlay = false;
		}
	}

	void cDamageNumber::Render(cCameraComponent* camera)
	{
		D3DXVECTOR3 vToCamera = camera->GetPosition() - m_pos;

		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &(D3DXVECTOR3(0.0f, 1.0f, 0.0f)), &vToCamera);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVECTOR3 startPos = m_pos + (vRight * m_width / 2.0f) - (vRight * billboard[0]->GetWidth() / 2.0f);

		for (int i = vec_num.size(); i > 0; i--)
		{
			g_pDamageNumberManager->GetBillBoard(vec_num[i - 1])->Render(camera, startPos);
			//billboard[vec_num[i-1]]->Render(camera, startPos);
			startPos -= (vRight * billboard[0]->GetHeight());
		}
	}
}