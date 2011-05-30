#include "stdafx.h"
#include "evrythng.h"
#include "OutLib.h"


VOID ReportError(LPCTSTR UserMessage, DWORD ExitCode, BOOL PrintErrorMsg)
{
	DWORD eMsgLen, LaseErr = GetLastError();

	LPSTR lpSysMsg;

	HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
	PrintMsg(hStdErr,UserMessage);
	if(PrintErrorMsg)
	{
		eMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
								NULL,
								LaseErr,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								(LPCTSTR)&lpSysMsg,
								0,NULL);

		PrintStrings(hStdErr, _T("\n"), lpSysMsg, _T("\n"), NULL);
		HeapFree(GetProcessHeap(), 0, lpSysMsg);
	}
	if(ExitCode > 0)
		ExitProcess(ExitCode);
	else
		return;

}