#pragma once
#include <Engine/Rendering/PBRCapture.h>
#include <Engine/Graphics/Texture.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{

		class NEAPI ImageBasedLighting
		{
		public:

			Rendering::PBRCapture EquirectangularToCubemap(Graphics::Texture* Tex);

		};
	}
}