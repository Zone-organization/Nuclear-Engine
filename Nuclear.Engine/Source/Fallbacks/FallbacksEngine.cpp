#include <Fallbacks/FallbacksEngine.h>
#include <Assets/AssetManager.h>

namespace Nuclear
{
	namespace Fallbacks
	{
		FallbacksEngine::FallbacksEngine()
		{
			Initialize();
		}
		FallbacksEngine& FallbacksEngine::GetInstance()
		{
			static FallbacksEngine instance;
			return instance;
		}
		void FallbacksEngine::Initialize()
		{
			//Images
			DefaultBlackImage = Assets::Importer::GetInstance().ImportImageST("@NuclearAssets@/DefaultTextures/Black32x32.png", &Assets::AssetManager::GetInstance().GetDefaultLibrary());
			DefaultGreyImage = Assets::Importer::GetInstance().ImportImageST("@NuclearAssets@/DefaultTextures/Grey32x32.png", &Assets::AssetManager::GetInstance().GetDefaultLibrary());
			DefaultWhiteImage = Assets::Importer::GetInstance().ImportImageST("@NuclearAssets@/DefaultTextures/White32x32.png", &Assets::AssetManager::GetInstance().GetDefaultLibrary());


			//Textures
			DefaultNormalTex.Set(Assets::Importer::GetInstance().ImportImageST("@NuclearAssets@/DefaultTextures/Normal32x32.png", &Assets::AssetManager::GetInstance().GetDefaultLibrary()), Graphics::TextureUsageType::Normal);
			DefaultDiffuseTex.Set(DefaultGreyImage, Graphics::TextureUsageType::Diffuse);
			DefaultSpecularTex.Set(DefaultWhiteImage, Graphics::TextureUsageType::Specular);
		}
		Assets::Image* FallbacksEngine::GetDefaultBlackImage()
		{
			return DefaultBlackImage;
		}
		Assets::Image* FallbacksEngine::GetDefaultGreyImage()
		{
			return DefaultGreyImage;
		}
		Assets::Image* FallbacksEngine::GetDefaultWhiteImage()
		{
			return nullptr;
		}
		const Graphics::Texture& FallbacksEngine::GetDefaultDiffuseTex() const
		{
			return DefaultDiffuseTex;
		}
		const Graphics::Texture& FallbacksEngine::GetDefaultSpecularTex() const
		{
			return DefaultSpecularTex;
		}
		const Graphics::Texture& FallbacksEngine::GetDefaultNormalTex() const
		{
			return DefaultNormalTex;
		}
	}
}