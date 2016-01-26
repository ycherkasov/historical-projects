#pragma once

// atexit pointer type
#ifdef _MSC_VER
#define C_CALLING_CONVENTION_QUALIFIER __cdecl
#else
#define C_CALLING_CONVENTION_QUALIFIER
#endif

typedef void (C_CALLING_CONVENTION_QUALIFIER *atexit_pfn_t)();
