#pragma once
#include <cstdint>
#include <NE_Compiler.h>

//Default!!  (D3D: LH , ClipSpace Z [0,1] -- OpenGL: RH , ClipSpace Z [-1,1])
#define COORDINATE_SYSTEM_LEFT_HANDED
#define CLIP_SPACE_DEPTH_ZERO_TO_ONE

#ifdef COORDINATE_SYSTEM_LEFT_HANDED
#define COORDSYSTEM_LH_ENABLED true
#else
#define COORDSYSTEM_LH_ENABLED false
#endif

namespace Nuclear
{
	typedef float Float32; ///< 32-bit float            [4 byte]

	typedef int64_t Int64; ///< 64-bit signed integer            [8 byte]
	typedef int32_t Int32; ///< 32-bit signed integer            [4 byte]
	typedef int16_t Int16; ///< 16-bit signed integer            [2 byte]
	typedef int8_t  Int8;  ///< 8-bit signed integer             [1 byte]

	typedef uint64_t Uint64; ///< 64-bit unsigned integer        [8 byte]
	typedef uint32_t Uint32; ///< 32-bit unsigned integer        [4 byte]
	typedef uint16_t Uint16; ///< 16-bit unsigned integer        [2 byte]
	typedef uint8_t  Uint8;  ///< 8-bit unsigned integer         [1 byte]

	typedef size_t      SizeType;
	typedef void* PVoid;
	typedef const void* CPVoid;

	typedef bool Bool;				///< Boolean		    [1 byte]

	typedef char Char;				//			[1 byte]
	typedef unsigned char Byte;      ///<						[1 byte]
	typedef double Float64;			 ///< 64-bit float          [8 byte]

	//using namespace Diligent;
}