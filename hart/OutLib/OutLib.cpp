// OutLib.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "OutLib.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

//// This is an example of an exported variable
//OUTLIB_API int nOutLib=0;
//
//// This is an example of an exported function.
//OUTLIB_API int fnOutLib(void)
//{
//	return 42;
//}
//
//// This is the constructor of a class that has been exported.
//// see OutLib.h for the class definition
//COutLib::COutLib()
//{
//	return;
//}
