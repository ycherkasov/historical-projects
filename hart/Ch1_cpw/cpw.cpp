// cpw.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <cstdio>


// --------------- Copying files using WinAPI -----------------
int _tmain(int argc, LPCTSTR argv[])
{
	HANDLE hIn, hOut;
	HANDLE hLog;
	DWORD nIn,nOut;

	TCHAR buffer[BUFSIZ];

	// if not 2 arguments (3 - exepath)
	if(argc != 3)
	{
		wprintf(_T("Using cpw FILE1 FILE2\n"));
		return 1;
	}

	hIn = CreateFile(argv[1],GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hIn == INVALID_HANDLE_VALUE)
	{
		wprintf(_T("Unable to open input file. Errno:%x\n"),GetLastError());
		return 2;
	}

	hOut = CreateFile(argv[2],GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE)
	{
		wprintf(_T("Unable to open output file. Errno:%x\n"),GetLastError());
		return 3;
	}

	hLog = CreateFile(_T("cp.log"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hLog == INVALID_HANDLE_VALUE)
	{
		wprintf(_T("Error creating logfile. Errno:%x\n"),GetLastError());
		return 5;
	}

	while(ReadFile(hIn,buffer,BUFSIZ,&nIn,NULL) && nIn > 0)
	{
		WriteFile(hOut,buffer,nIn,&nOut,NULL);


		if(nIn != nOut)
		{
			wprintf(_T("Write error. Errno:%x\n"), GetLastError());
			CloseHandle(hLog);
			return 4;
		}
		// --------------- Write to log -----------------
		ZeroMemory(buffer,BUFSIZ);
		DWORD logIn,logOut;
		TCHAR tmp[10] = {0};
		_itot_s(nIn,tmp,10);
		_tcscpy_s(buffer,_T("Read "));
		_tcscat_s(buffer,tmp);
		_tcscat_s(buffer,_T(" bytes"));
		logIn = (DWORD)_tcslen(buffer)*sizeof(TCHAR);
		WriteFile(hLog,buffer,logIn,&logOut,NULL);
		// ---------------  -----------------
	}
	CloseHandle(hIn);
	CloseHandle(hOut);
	CloseHandle(hLog);
	return 0;
}

