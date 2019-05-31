#pragma once

namespace ENGINE
{

	class cNumberBillBoard;
	class cCameraComponent;

	class cDamageNumber
	{
	public:
		void Play(int num, float destroyTime, D3DXVECTOR3& pos);
		void Update();
		void Render(cCameraComponent* camera);

		bool GetIsPlay() { return m_isPlay; }

		cDamageNumber(cNumberBillBoard* bill);
		~cDamageNumber();

	private:
		bool m_isPlay;
		float m_passedTime;
		float m_destroyTime;
		D3DXVECTOR3 m_pos;
		int m_num;

		float m_width;

		std::vector<int> vec_num;

		cNumberBillBoard* billboard[10];

	};

}