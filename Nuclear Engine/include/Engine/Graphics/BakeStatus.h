#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		enum class BakeStatus 
		{
			NotInitalized = 0,
			RequiresBaking = 1,
			Baked = 2
		};
	}
}