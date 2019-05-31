#pragma once

#define g_pDebug cDebugLog::GetInstance()

class cDebugLog
{
	TCHAR szStr[1024];
public:
	SINGLETONE(cDebugLog);
	int Log(char * fmt, ...);

	TCHAR* GetLog() { return szStr; }

};