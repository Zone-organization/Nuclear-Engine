#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Engine/Assets/Image.h>

namespace Nuclear
{
	namespace Rendering
	{
		class NEAPI PBRCapture
		{
		public:
			Assets::Image mIrradiance;
			Assets::Image mPrefiltered;
		};
	}
}