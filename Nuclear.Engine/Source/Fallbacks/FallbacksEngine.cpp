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
			DefaultBlackImage = Assets::Importer::GetInstance().ImportTextureST("@NuclearAssets@/DefaultTextures/Black32x32.png");
			DefaultGreyImage = Assets::Importer::GetInstance().ImportTextureST("@NuclearAssets@/DefaultTextures/Grey32x32.png");
			DefaultWhiteImage = Assets::Importer::GetInstance().ImportTextureST("@NuclearAssets@/DefaultTextures/White32x32.png");


			//Textures
			DefaultNormalTex.pTexture = Assets::Importer::GetInstance().ImportTextureST("@NuclearAssets@/DefaultTextures/Normal32x32.png");
			DefaultNormalTex.mUsageType = Assets::TextureUsageType::Normal;

			DefaultDiffuseTex.pTexture = DefaultGreyImage;
			DefaultDiffuseTex.mUsageType = Assets::TextureUsageType::Diffuse;

			DefaultSpecularTex.pTexture = DefaultWhiteImage;
			DefaultSpecularTex.mUsageType = Assets::TextureUsageType::Specular;
		}
		Assets::Texture* FallbacksEngine::GetDefaultBlackImage()
		{
			return DefaultBlackImage;
		}
		Assets::Texture* FallbacksEngine::GetDefaultGreyImage()
		{
			return DefaultGreyImage;
		}
		Assets::Texture* FallbacksEngine::GetDefaultWhiteImage()
		{
			return nullptr;
		}
		const Assets::MaterialTexture& FallbacksEngine::GetDefaultDiffuseTex() const
		{
			return DefaultDiffuseTex;
		}
		const Assets::MaterialTexture& FallbacksEngine::GetDefaultSpecularTex() const
		{
			return DefaultSpecularTex;
		}
		const Assets::MaterialTexture& FallbacksEngine::GetDefaultNormalTex() const
		{
			return DefaultNormalTex;
		}
	}
}