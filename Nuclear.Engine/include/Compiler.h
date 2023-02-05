#pragma once


#if defined(_MSC_VER)
#define INLINE __inline
#define FORCE_INLINE __forceinline
#define FORCE_NOINLINE __declspec(noinline)
#else
#define INLINE inline
#define FORCE_INLINE inline
#define FORCE_NOINLINE __attribute__((noinline))
#endif
