// cpcf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <cstdio>

// --------------- Copying files using Copy File function-----------------
int _tmain(int argc, LPCTSTR argv[])
{
	// if not 2 arguments (3 - exepath)
	if(argc != 3)
	{
		wprintf(_T("Using cpw FILE1 FILE2\n"));
		return 1;
	}

	if(!CopyFile(argv[1],argv[2],FALSE))
	{
		wprintf(_T("Error Copy File. Errno:%x\n"),GetLastError());
		return 2;
	}
	return 0;
}

