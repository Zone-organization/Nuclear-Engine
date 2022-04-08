#pragma once
#include <Engine/Graphics/Texture.h>
#include <Engine\Managers\AssetManager.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI DefaultTextures
		{
		public:

			static void Initalize(Managers::AssetManager* Manager);

			static Graphics::Texture DefaultBlackTex;
			static Graphics::Texture DefaultGreyTex;
			static Graphics::Texture DefaultWhiteTex;

			static Graphics::Texture DefaultDiffuseTex;
			static Graphics::Texture DefaultSpecularTex;
			static Graphics::Texture DefaultNormalTex;

		};
	}
}