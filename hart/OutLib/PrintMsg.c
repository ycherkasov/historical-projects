#include "stdafx.h"
#include "EVRYTHNG.H"
#include "OutLib.h"
#include <stdarg.h>

BOOL PrintStrings(HANDLE hOut, ...)
{
	DWORD msgLen = 0, Count = 0;
	LPCTSTR pMsg;
	va_list pMsgList;
	va_start(pMsgList, hOut);

	while((pMsg=va_arg(pMsgList,LPCTSTR)) != NULL)
	{
		if(!WriteConsole(hOut, pMsg, msgLen, &Count, NULL) &&
			!WriteFile(hOut, pMsg, msgLen*sizeof(TCHAR), &Count, NULL))
		return FALSE;
	}
	va_end(pMsgList);
	return TRUE;
}

BOOL PrintMsg(HANDLE hOut, LPCTSTR pMsg)
{
	return PrintStrings(hOut,pMsg,NULL);
}

BOOL ConsolePromt(LPCTSTR pPromtMsg, LPTSTR pResponse, DWORD maxTChar, BOOL echo)
{
	HANDLE hStdIn, hStdOut;
	DWORD TcharIn, EchoFlag;
	BOOL Success;

	hStdIn = CreateFile(_T("CONIN$"),				//name
						GENERIC_READ|GENERIC_WRITE,	//access
						0,							//share mode
						NULL,						//security
						CREATE_ALWAYS,				//create disposition
						FILE_ATTRIBUTE_NORMAL,		//attributes
						NULL);						//template

	hStdOut = CreateFile(_T("CONOUT$"),
						GENERIC_WRITE,
						0,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	EchoFlag = echo?ENABLE_ECHO_INPUT:0;

	Success = SetConsoleMode(hStdIn, ENABLE_LINE_INPUT | EchoFlag | ENABLE_PROCESSED_INPUT) &&
		SetConsoleMode(hStdOut, ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT) &&
		PrintStrings(hStdOut,pPromtMsg,NULL) &&
		ReadConsole(hStdIn, pResponse, maxTChar, &TcharIn, NULL);

	if(Success) pResponse[TcharIn - 2] = '\0';

	CloseHandle(hStdIn);
	CloseHandle(hStdOut);
	return Success;
}