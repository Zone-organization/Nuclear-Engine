#pragma once
#include <Core\NE_Common.h>

namespace Nuclear
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