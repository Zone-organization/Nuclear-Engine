#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;

		enum class ShaderTextureType : Uint8
		{
			Unknown = 0,

			MaterialTex = 1,
			RenderTarget = 2,
			RenderingEffect = 3,
			ShadowTex = 4,
			Sprite = 5,

			UNKNOWN_MAX = 255
		};

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

			Sprite = 9,

			AmbientOcclusion = 10,

			Unknown = 255
		};

		struct TextureLoadingData
		{
			void* mMemData;

			Uint32 mMemSize;

			//Internal
			bool mEngineAllocMem;

			TextureLoadingData() :			
				mMemData(nullptr),
				mMemSize(0),
				mEngineAllocMem(false)
			{
			}
		};

	}
}