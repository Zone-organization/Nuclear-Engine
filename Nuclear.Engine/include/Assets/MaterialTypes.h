#pragma once
#include <NE_Common.h>
#include <Graphics/Texture.h>
#include <vector>

namespace Nuclear
{
	namespace Assets
	{
		enum class ShaderTextureType : Uint8
		{
			Unknown = 0,

			MaterialTex = 1,
			RenderTarget = 2,
			IBL_Tex = 3,		
			ShadowTex
		};

		struct ShaderTexture
		{
			Uint32 mSlot = 0;
			Graphics::Texture mTex;

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