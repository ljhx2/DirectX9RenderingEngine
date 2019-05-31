#include "stdafx.h"
#include "cDebugLog.h"

extern HWND g_hWndDebugList;

cDebugLog::cDebugLog()
{

}

cDebugLog::~cDebugLog()
{

}

int cDebugLog::Log(char * fmt, ...)
{
	char	buff[1024];
	char	date[128];
	char	time[128];

	memset(szStr, 0, sizeof(szStr));

	_strdate(date);
	_strtime(time);
	vsprintf(buff, fmt, (char *)(&fmt + 1));

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buff, strlen(buff), szStr, strlen(buff));

	// Log File에 출력할 경우
	FILE*	fp = NULL;

	fopen_s(&fp, "Debug.log", "a+");

	if (fp)
	{
		fprintf(fp, "(Time[%s][%s]) : %s\n", date, time, buff);
		fclose(fp);
	}


	// Log Window에 출력할 경우 
	{
		SendMessage(g_hWndDebugList, LB_ADDSTRING, 0, (LPARAM)szStr);
		UINT32 n = SendMessage(g_hWndDebugList, LB_GETCOUNT, 0, 0L) - 1;
		SendMessage(g_hWndDebugList, LB_SETCURSEL, (WPARAM)n, 0L);
	}
	return 1;
}