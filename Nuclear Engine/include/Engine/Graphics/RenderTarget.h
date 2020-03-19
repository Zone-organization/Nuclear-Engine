#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>

namespace NuclearEngine
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
		};

		class NEAPI RenderTarget
		{
		public:
			RenderTarget();
			~RenderTarget();

			void Create(const RenderTargetDesc& Desc);
			void SetActive(const float* RGBA);

			RefCntAutoPtr<ITextureView> mColorRTV;
			RefCntAutoPtr<ITextureView> mShaderRTV;
			RefCntAutoPtr<ITextureView> mDepthDSV;
		};
	}
}