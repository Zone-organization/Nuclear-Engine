#pragma once
#include <Diligent/Primitives/interface/BasicTypes.h>  //We dont really need this include but i added it to fix Diligent Namespace

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

typedef unsigned char Byte;
typedef double Float64;

namespace NuclearEngine
{
	using namespace Diligent;
}