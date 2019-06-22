#include <Engine\Assets\DefaultTextures.h>

namespace NuclearEngine
{
	namespace Assets
	{

		Texture DefaultTextures::DefaultBlackTex;
		Texture DefaultTextures::DefaultGreyTex;
		Texture DefaultTextures::DefaultWhiteTex;

		Texture DefaultTextures::DefaultDiffuseTex;
		Texture DefaultTextures::DefaultSpecularTex;
		Texture DefaultTextures::DefaultNormalTex;

		void DefaultTextures::Initalize(Managers::AssetManager* AssetLoader)
		{
			DefaultBlackTex = AssetLoader->Import("Assets/NuclearEngine/DefaultTextures/Black32x32.png", Assets::TextureUsageType::Unknown);
			DefaultGreyTex = AssetLoader->Import("Assets/NuclearEngine/DefaultTextures/Grey32x32.png", Assets::TextureUsageType::Unknown);
			DefaultWhiteTex = AssetLoader->Import("Assets/NuclearEngine/DefaultTextures/White32x32.png", Assets::TextureUsageType::Unknown);

			DefaultDiffuseTex = DefaultGreyTex;
			DefaultSpecularTex = DefaultBlackTex;
			DefaultNormalTex = DefaultBlackTex;
		}
	}
}