#pragma once

#include <cstdint>

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

//Default!!  (D3D: LH , ClipSpace Z [0,1] -- OpenGL: RH , ClipSpace Z [-1,1])
#define COORDINATE_SYSTEM_LEFT_HANDED
#define CLIP_SPACE_DEPTH_ZERO_TO_ONE

#ifdef COORDINATE_SYSTEM_LEFT_HANDED
#define COORDSYSTEM_LH_ENABLED true
#else
#define COORDSYSTEM_LH_ENABLED false
#endif

namespace Diligent{
	typedef float Float32; ///< 32-bit float

	typedef int64_t Int64; ///< 64-bit signed integer
	typedef int32_t Int32; ///< 32-bit signed integer
	typedef int16_t Int16; ///< 16-bit signed integer
	typedef int8_t  Int8;  ///< 8-bit signed integer

	typedef uint64_t Uint64; ///< 64-bit unsigned integer
	typedef uint32_t Uint32; ///< 32-bit unsigned integer
	typedef uint16_t Uint16; ///< 16-bit unsigned integer
	typedef uint8_t  Uint8;  ///< 8-bit unsigned integer

	typedef size_t      SizeType;
	typedef void* PVoid;
	typedef const void* CPVoid;

	typedef bool Bool; ///< Boolean

	typedef char Char;
}
namespace Nuclear
{
	typedef unsigned char Byte;
	typedef double Float64;
	using namespace Diligent;
}