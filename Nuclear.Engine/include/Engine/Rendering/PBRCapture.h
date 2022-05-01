#pragma once
#include <Core\NE_Common.h>
#include <Engine/Assets/Image.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI PBRCapture
		{
		public:
			Assets::Image* mIrradiance = nullptr;
			Assets::Image* mPrefiltered = nullptr;
		};
	}
}