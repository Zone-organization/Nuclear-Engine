#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/RenderTarget.h>
#include <string>

namespace Nuclear
{
	namespace Rendering
	{
		struct ShadowMapDesc
		{
			Uint32 Width = 1024;
			Uint32 Height = 1024;
			TEXTURE_FORMAT DepthTexFormat = TEX_FORMAT_D32_FLOAT;
			OptimizedClearValue ClearValue;
			bool mCreateDepth = true;
		};

		class NEAPI DirShadowMap
		{
		public:

			void Initialize(const ShadowMapDesc& desc);
		private:
			Graphics::RenderTarget mDepthMapRT;
			RefCntAutoPtr<ITextureView> mDepthDSV;

		};
	}
}