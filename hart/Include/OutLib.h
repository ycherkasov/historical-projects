// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OUTLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OUTLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OUTLIB_EXPORTS
#define OUTLIB_API __declspec(dllexport)
#else
#define OUTLIB_API __declspec(dllimport)
#endif

// This class is exported from the OutLib.dll
//class OUTLIB_API COutLib {
//public:
//	COutLib(void);
//	// TODO: add your methods here.
//};
//
//extern OUTLIB_API int nOutLib;
//
//OUTLIB_API int fnOutLib(void);


/* support.h */
/* Definitions of all symbolic constants and common
utility functions used throughout the example programs. */

//#ifdef _STATICLIB
//#define LIBSPEC
//#else
//
//#ifdef UTILITY_EXPORTS
//#define LIBSPEC _declspec (dllexport)
//#else
//#define LIBSPEC _declspec (dllimport)
//#endif
//
//#endif

#define EMPTY _T ("")
#define YES _T ("y")
#define NO _T ("n")
#define CR 0x0D
#define LF 0x0A
#define TSIZE sizeof (TCHAR)

/* Limits and constants. */

#define TYPE_FILE 1		/* Used in ls, rm, and lsFP */
#define TYPE_DIR 2
#define TYPE_DOT 3

#define MAX_OPTIONS 20	/* Max # command line options */
#define MAX_ARG 1000	/* Max # of command line arguments */
#define MAX_COMMAND_LINE MAX_PATH+50 /* Max size of a command line */
#define MAX_NAME 256    /* Name length - users and groups */

/* Commonly used functions. */

OUTLIB_API BOOL ConsolePrompt (LPCTSTR, LPTSTR, DWORD, BOOL);
OUTLIB_API BOOL PrintStrings (HANDLE, ...);
OUTLIB_API BOOL PrintMsg (HANDLE, LPCTSTR);
OUTLIB_API VOID ReportError (LPCTSTR, DWORD, BOOL);
OUTLIB_API VOID ReportException (LPCTSTR, DWORD);
OUTLIB_API DWORD Options (int, LPCTSTR *, LPCTSTR, ...);
OUTLIB_API LPTSTR SkipArg (LPCTSTR);

/* Collection of generic string functions modeled after string.h.
Created as required - there was only one!
Implementations are derived from Plauger: The Standard C Library. */

//LIBSPEC LPCTSTR wmemchr (LPCTSTR, TCHAR, DWORD);
OUTLIB_API VOID GetArgs (LPCTSTR, int *, LPTSTR *);

/* Security Functions. */

LPSECURITY_ATTRIBUTES InitializeUnixSA (DWORD, LPTSTR, LPTSTR, LPDWORD, LPHANDLE);
DWORD ReadFilePermissions (LPTSTR, LPTSTR, LPTSTR);
BOOL ChangeFilePermissions (DWORD, LPTSTR, LPDWORD);
/*	Simpler forms available with Visual C++ Version 5.0 */
//PSECURITY_DESCRIPTOR InitializeSD (DWORD, LPTSTR, LPTSTR, LPDWORD);
/* Constants needed by the security functions. */

#define LUSIZE 1024
#define ACCT_NAME_SIZE LUSIZE

#ifdef _UNICODE /* This declaration had to be added. */
#define _tstrrchr wcsrchr
#else
#define _tstrrchr strrchr
#endif

#ifdef _UNICODE /* This declaration had to be added. */
#define _tstrstr wcsstr
#else
#define _tstrstr strstr
#endif

#ifdef _UNICODE /* This declaration had to be added. */
#define _memtchr wmemchr
#else
#define _memtchr memchr
#endif

/* 
* Define a macro that delays for an amount of time proportional
* to the integer parameter. The delay is a CPU delay and does not
* voluntarily yield the processor. This simulates computation.
*/

#define delay_cpu(n)  {\
	int i=0, j=0;\
	/* Do some wasteful computations that will not be optimized to nothing */\
	while (i < n) {\
	j = (int)(i*i + (float)(2*j)/(float)(i+1));\
	i++;\
	}\
}
