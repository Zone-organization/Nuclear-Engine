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
		FallbacksModule& FallbacksModule::GetInstance()
		{
			static FallbacksModule instance;
			return instance;
		}
		void FallbacksModule::Initialize()
		{
			//Images
			Assets::TextureImportingDesc desc;
			desc.mCommonOptions.mAsyncImport = false;
			desc.mCommonOptions.mLoadOnly = true;
			DefaultBlackImage = Assets::Importer::GetInstance().ImportTexture("@NuclearAssets@/DefaultTextures/Black32x32.png", desc);
			DefaultGreyImage = Assets::Importer::GetInstance().ImportTexture("@NuclearAssets@/DefaultTextures/Grey32x32.png", desc);
			DefaultWhiteImage = Assets::Importer::GetInstance().ImportTexture("@NuclearAssets@/DefaultTextures/White32x32.png", desc);


			//Textures
			DefaultNormalTex.pTexture = Assets::Importer::GetInstance().ImportTexture("@NuclearAssets@/DefaultTextures/Normal32x32.png", desc);
			DefaultNormalTex.mUsageType = Assets::TextureUsageType::Normal;

			DefaultDiffuseTex.pTexture = DefaultGreyImage;
			DefaultDiffuseTex.mUsageType = Assets::TextureUsageType::Diffuse;

			DefaultSpecularTex.pTexture = DefaultWhiteImage;
			DefaultSpecularTex.mUsageType = Assets::TextureUsageType::Specular;
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