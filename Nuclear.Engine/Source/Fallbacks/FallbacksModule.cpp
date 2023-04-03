#include <Fallbacks/FallbacksModule.h>
#include <Assets/AssetManager.h>

namespace Nuclear
{
	namespace Fallbacks
	{
		FallbacksModule::FallbacksModule()
		{
			Initialize();
		}

		void FallbacksModule::Initialize()
		{
			//Images
			Assets::TextureImportingDesc desc;
			desc.mCommonOptions.mAsyncImport = false;
			desc.mCommonOptions.mLoadOnly = true;
			DefaultBlackImage = Assets::Importer::Get().ImportTexture("@NuclearAssets@/DefaultTextures/Black32x32.png", desc);
			DefaultGreyImage = Assets::Importer::Get().ImportTexture("@NuclearAssets@/DefaultTextures/Grey32x32.png", desc);
			DefaultWhiteImage = Assets::Importer::Get().ImportTexture("@NuclearAssets@/DefaultTextures/White32x32.png", desc);


			//Textures
			DefaultNormalTex.pTexture = Assets::Importer::Get().ImportTexture("@NuclearAssets@/DefaultTextures/Normal32x32.png", desc);
			DefaultNormalTex.mUsageType = Assets::TextureUsageType::Normal;

			DefaultDiffuseTex.pTexture = DefaultGreyImage;
			DefaultDiffuseTex.mUsageType = Assets::TextureUsageType::Diffuse;

			DefaultSpecularTex.pTexture = DefaultWhiteImage;
			DefaultSpecularTex.mUsageType = Assets::TextureUsageType::Specular;
		}
		void FallbacksModule::Shutdown()
		{
		}
		Assets::Texture* FallbacksModule::GetDefaultBlackImage()
		{
			return DefaultBlackImage;
		}
		Assets::Texture* FallbacksModule::GetDefaultGreyImage()
		{
			return DefaultGreyImage;
		}
		Assets::Texture* FallbacksModule::GetDefaultWhiteImage()
		{
			return nullptr;
		}
		const Assets::MaterialTexture& FallbacksModule::GetDefaultDiffuseTex() const
		{
			return DefaultDiffuseTex;
		}
		const Assets::MaterialTexture& FallbacksModule::GetDefaultSpecularTex() const
		{
			return DefaultSpecularTex;
		}
		const Assets::MaterialTexture& FallbacksModule::GetDefaultNormalTex() const
		{
			return DefaultNormalTex;
		}
	}
}