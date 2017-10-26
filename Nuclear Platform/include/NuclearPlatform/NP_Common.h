#pragma once

#ifdef NUCLEARPLATFORM_EXPORTS
#define NPAPI __declspec(dllexport)
#else
#define NPAPI __declspec(dllimport)
#endif

#ifdef _WIN32
#define NUCLEAR_PLATFORM_WINDOWS32
#endif


// 8 bits integer types
typedef signed   char Int8;
typedef unsigned char Uint8;

// 16 bits integer types
typedef signed   short Int16;
typedef unsigned short Uint16;

// 32 bits integer types
typedef signed   int Int32;
typedef unsigned int Uint32;

// 64 bits integer types
#if defined(_MSC_VER)
typedef signed   __int64 Int64;
typedef unsigned __int64 Uint64;
#else
typedef signed   long long Int64;
typedef unsigned long long Uint64;
#endif