#pragma once
#include <NE_Common.h>
#include <Graphics/Texture.h>

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
			
			Assets::Image* GetDefaultBlackImage();
			Assets::Image* GetDefaultGreyImage();
			Assets::Image* GetDefaultWhiteImage();

			const Graphics::Texture& GetDefaultDiffuseTex() const;
			const Graphics::Texture& GetDefaultSpecularTex() const;
			const Graphics::Texture& GetDefaultNormalTex() const;
		private:
			FallbacksEngine();

			Assets::Image* DefaultBlackImage;
			Assets::Image* DefaultGreyImage;
			Assets::Image* DefaultWhiteImage;

			Graphics::Texture DefaultDiffuseTex;
			Graphics::Texture DefaultSpecularTex;
			Graphics::Texture DefaultNormalTex;
		};
	}
}