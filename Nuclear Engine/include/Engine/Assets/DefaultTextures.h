#pragma once
#include <Engine/Assets/Texture.h>
#include <Engine\Managers\AssetManager.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI DefaultTextures
		{
		public:

			static void Initalize(Managers::AssetManager* Manager);

			static Texture DefaultBlackTex;
			static Texture DefaultGreyTex;
			static Texture DefaultWhiteTex;

			static Texture DefaultDiffuseTex;
			static Texture DefaultSpecularTex;
			static Texture DefaultNormalTex;

		};
	}
}