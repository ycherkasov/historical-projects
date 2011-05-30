// cp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <cstdio>

// ---------------  -----------------
// --------------- Copying files using C-libraries -----------------
int _tmain(int argc, const TCHAR* argv[])
{
	// --------------- Set file objects -----------------
	FILE* in_file = 0;
	FILE* out_file = 0;
	
	// --------------- Set recording buffer -----------------
	TCHAR rec[BUFSIZ];
	ZeroMemory(rec,BUFSIZ);

	size_t bytes_in, bytes_out;

	// if not 2 arguments (3 - exepath)
	if(argc != 3)
	{
		printf("Using cp FILE1 FILE2\n");
		return 1;
	}
	//open 1-st argument
	errno_t et = _wfopen_s(&in_file, argv[1], _T("rb"));
	if((in_file == NULL) || (et))
	{
		_wperror(argv[1]);
		return 2;
	}

	//open 2-nd argument
	et = _wfopen_s(&out_file, argv[2], _T("wb"));
	if((out_file == NULL) || (et))
	{
		_wperror(argv[2]);
		return 3;
	}

	//Files processing
	while((bytes_in = fread(rec,1,BUFSIZ,in_file)) > 0)
	{
		//Write file with BUFSIZ parts
		bytes_out = fwrite(rec,1,bytes_in,out_file);
		if(bytes_out != bytes_in)
		{
			_wperror(L"Write error");
			return 4;
		}
	}
	fclose(in_file);
	fclose(out_file);
	return 0;
}

