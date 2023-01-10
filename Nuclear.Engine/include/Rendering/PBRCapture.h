#pragma once
#include <NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Assets/Texture.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI PBRCapture
		{
		public:
			Assets::Texture mIrradiance;
			Assets::Texture mPrefiltered;
		};
	}
}