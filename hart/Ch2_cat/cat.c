#include "evrythng.h"

#define BUF_SIZE 200

// --------------------------------------------------------------
// Unix command 'cat' analogue
// Send command arguments to output

static VOID CatFile(HANDLE, HANDLE);

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hInFile = 0, hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	BOOL DashS;
	int iArg, iFirstFile;

	iFirstFile = Options(argc, (LPCTSTR*)argv, _T("s"), &DashS, NULL);
	if(iFirstFile == argc)
	{
		CatFile(hStdIn, hStdOut);
		return 0;
	}
	for(iArg=iFirstFile; iArg < argc; iArg++)
	{
		hInFile = CreateFile(argv[iArg],
							GENERIC_READ,
							0, NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
		if(hInFile == INVALID_HANDLE_VALUE && !DashS)
			ReportError(_T("Cat - error opening file"),1,TRUE);
		CatFile(hInFile,hStdOut);
		CloseHandle(hInFile);
	}
	return 0;
}


static VOID CatFile(HANDLE hInFile, HANDLE hOutFile)
{
	DWORD nIn, nOut;
	BYTE Buffer[BUF_SIZE];
	while(ReadFile(hInFile,Buffer,BUF_SIZE,&nIn,NULL) 
		&& (nIn != 0) 
		&& WriteFile(hOutFile,Buffer,nIn,&nOut,NULL));
	return;
}