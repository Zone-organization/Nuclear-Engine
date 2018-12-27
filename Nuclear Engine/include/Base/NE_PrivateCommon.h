#pragma once

#ifdef NUCLEARENGINE_EXPORTS
#define NEAPI __declspec(dllexport)
#else
#define NEAPI __declspec(dllimport)
#endif

// 8 bits integer types
typedef signed   char Int8;
typedef unsigned char Uint8;

typedef unsigned char Byte;

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
typedef unsigned long long Uin264;
#endif

typedef float Float32;
typedef double Float64;