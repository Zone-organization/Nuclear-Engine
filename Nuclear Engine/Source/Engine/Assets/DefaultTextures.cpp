#include <Engine\Assets\DefaultTextures.h>

namespace NuclearEngine
{
	namespace Assets
	{

		Graphics::Texture DefaultTextures::DefaultBlackTex;
		Graphics::Texture DefaultTextures::DefaultGreyTex;
		Graphics::Texture DefaultTextures::DefaultWhiteTex;

		Graphics::Texture DefaultTextures::DefaultDiffuseTex;
		Graphics::Texture DefaultTextures::DefaultSpecularTex;
		Graphics::Texture DefaultTextures::DefaultNormalTex;

		void DefaultTextures::Initalize(Managers::AssetManager* AssetLoader)
		{
			DefaultBlackTex = AssetLoader->Import("Assets/NuclearEngine/DefaultTextures/Black32x32.png", Graphics::TextureUsageType::Unknown);
			DefaultGreyTex = AssetLoader->Import("Assets/NuclearEngine/DefaultTextures/Grey32x32.png", Graphics::TextureUsageType::Diffuse);
			DefaultWhiteTex = AssetLoader->Import("Assets/NuclearEngine/DefaultTextures/White32x32.png", Graphics::TextureUsageType::Specular);

			DefaultDiffuseTex = DefaultGreyTex;
			DefaultSpecularTex = DefaultWhiteTex;
			DefaultNormalTex = DefaultBlackTex;
		}
	}
}