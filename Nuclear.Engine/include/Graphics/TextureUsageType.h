#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Graphics
	{
		enum TextureUsageType : Uint8
		{
			Diffuse = 0,
			Albedo = 0,

			Specular = 1,
			Metallic = 1,

			Normal = 2,

			Roughness = 3,
			AO = 4,

			IrradianceMap = 5,
			PreFilterMap = 6,
			BRDF_LUT = 7,

			ShadowTex = 8,

			Unknown = 255
		};

	}
}