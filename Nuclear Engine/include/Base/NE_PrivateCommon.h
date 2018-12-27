#pragma once
#include <Diligent/Primitives/interface/BasicTypes.h>  //We dont really need this include but i added it to fix Diligent Namespace

#ifdef NUCLEARENGINE_EXPORTS
#define NEAPI __declspec(dllexport)
#else
#define NEAPI __declspec(dllimport)
#endif

typedef unsigned char Byte;
typedef double Float64;

namespace NuclearEngine
{
	using namespace Diligent;
}