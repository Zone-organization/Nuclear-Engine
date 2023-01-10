#pragma once
#include <NE_Common.h>
#include <vector>
#include <string>
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
			IBL_Tex = 3,		
			ShadowTex
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

			Unknown = 255
		};

		struct MaterialTexture
		{
			Texture* pTexture;
			TextureUsageType mUsageType;
		};

		struct ShaderTexture
		{
			Uint32 mSlot = 0;
			MaterialTexture mTex;

			ShaderTextureType mType = ShaderTextureType::Unknown;
		};

		struct TextureSet
		{
			TextureSet()
				: mData(std::vector<ShaderTexture>()), mHashedName(0), mName(std::string())
			{

			}
			~TextureSet()
			{
				mData.clear();
				mHashedName = 0;
				mName = std::string();
			}
			std::vector<ShaderTexture> mData;
			Uint32 mHashedName;
			std::string mName;
		};
	}
}