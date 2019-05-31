#include "Common.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager(void)
	: m_dwDeltaTime(0)
	, m_dwFPS(0)
	, m_dwFrameCount(0)
	, m_dwAccTime(0)
{
	m_dwLastUpdateTime = GetTickCount();
}


cTimeManager::~cTimeManager(void)
{
}

void cTimeManager::Update()
{
	DWORD dwCurrTime = GetTickCount();
	m_dwDeltaTime = dwCurrTime - m_dwLastUpdateTime;
	m_dwLastUpdateTime = dwCurrTime;

	++m_dwFrameCount;

	m_dwAccTime += m_dwDeltaTime;
	if(m_dwAccTime > 1000)
	{
		m_dwAccTime -= 1000;
		m_dwFPS = m_dwFrameCount;
		m_dwFrameCount = 0;
	}
}

float cTimeManager::GetDeltaTime()
{
	return m_dwDeltaTime / 1000.0f;
}
