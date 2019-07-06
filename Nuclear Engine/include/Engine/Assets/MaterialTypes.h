#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets\Texture.h>
#include <vector>

namespace NuclearEngine
{
	namespace Assets
	{
		struct ShaderTexture
		{
			Uint32 mSlot = 0;
			Assets::Texture mTex;
		};

		struct TextureSet
		{
			std::vector<ShaderTexture> mData = std::vector<ShaderTexture>();
			Uint32 mHashedName = 0;
			std::string mName = std::string();
		};
	}
}