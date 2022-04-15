#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace Nuclear
{
	namespace Graphics
	{
		struct RenderTargetDesc
		{
			Uint32 Width = 800;
			Uint32 Height = 600;
			TEXTURE_FORMAT ColorTexFormat = TEX_FORMAT_RGBA8_UNORM;
			TEXTURE_FORMAT DepthTexFormat = TEX_FORMAT_D32_FLOAT;
			OptimizedClearValue ClearValue;
			bool mCreateDepth = true;
		};

		class NEAPI RenderTarget
		{
		public:
			RenderTarget();
			~RenderTarget();

			void Create(const RenderTargetDesc& Desc);
			//void SetActive(const float* RGBA);
			Uint32 GetWidth() const;
			Uint32 GetHeight() const;

			RefCntAutoPtr<ITextureView> mColorRTV;
			RefCntAutoPtr<ITextureView> mShaderRTV;
			RefCntAutoPtr<ITextureView> mDepthDSV;
		private:
			Uint32 mWidth, mHeight;
			bool mDepthCreated =false;
		};
	}
}