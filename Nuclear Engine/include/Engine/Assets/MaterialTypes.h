#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/Texture.h>
#include <vector>

namespace NuclearEngine
{
	namespace Assets
	{
		struct ShaderTexture
		{
			Uint32 mSlot = 0;
			Graphics::Texture mTex;
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