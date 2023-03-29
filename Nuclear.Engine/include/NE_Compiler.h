#pragma once


#ifdef NUCLEARENGINE_EXPORTS
#define NEAPI __declspec(dllexport)
#else
#define NEAPI __declspec(dllimport)
#endif

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _DEBUG
#define DEBUG_TRUE_BOOL true
#define DEBUG_FALSE_BOOL false
#else
#define DEBUG_TRUE_BOOL false
#define DEBUG_FALSE_BOOL true
#endif

#if defined(_MSC_VER)
#define INLINE __inline
#define FORCE_INLINE __forceinline
#define FORCE_NOINLINE __declspec(noinline)
#else
#define INLINE inline
#define FORCE_INLINE inline
#define FORCE_NOINLINE __attribute__((noinline))
#endif
