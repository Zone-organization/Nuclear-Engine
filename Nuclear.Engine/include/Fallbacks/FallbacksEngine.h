#pragma once
#include <NE_Common.h>
#include <Assets/MaterialTypes.h>

namespace Nuclear
{
	namespace Fallbacks
	{
		class NEAPI FallbacksEngine
		{
		public:
			static FallbacksEngine& GetInstance();

			FallbacksEngine(const FallbacksEngine&) = delete;
			FallbacksEngine& operator= (const FallbacksEngine) = delete;

			void Initialize();
			
			Assets::Texture* GetDefaultBlackImage();
			Assets::Texture* GetDefaultGreyImage();
			Assets::Texture* GetDefaultWhiteImage();

			const Assets::MaterialTexture& GetDefaultDiffuseTex() const;
			const Assets::MaterialTexture& GetDefaultSpecularTex() const;
			const Assets::MaterialTexture& GetDefaultNormalTex() const;
		private:
			FallbacksEngine();

			Assets::Texture* DefaultBlackImage;
			Assets::Texture* DefaultGreyImage;
			Assets::Texture* DefaultWhiteImage;

			Assets::MaterialTexture DefaultDiffuseTex;
			Assets::MaterialTexture DefaultSpecularTex;
			Assets::MaterialTexture DefaultNormalTex;
		};
	}
}