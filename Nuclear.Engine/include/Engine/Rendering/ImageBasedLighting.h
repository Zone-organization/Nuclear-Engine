#pragma once
#include <Engine/Rendering/PBRCapture.h>
#include <Engine/Graphics/RenderTarget.h>
#include <Engine/Graphics/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Engine/Rendering/Skybox.h>

namespace Nuclear
{
	namespace Rendering
	{
		struct ImageBasedLightingDesc
		{
			TEXTURE_FORMAT mIrradianceCubeFmt = TEX_FORMAT_RGBA32_FLOAT;
			TEXTURE_FORMAT mPrefilteredEnvMapFmt = TEX_FORMAT_RGBA16_FLOAT;
			Uint32         mIrradianceCubeDim = 32;
			Uint32         mPrefilteredEnvMapDim = 123;


			//TEST
			IBuffer* cameraCB;
		};

		class NEAPI ImageBasedLighting
		{
		public:
			void Initialize(const ImageBasedLightingDesc& desc);

			Rendering::PBRCapture EquirectangularToCubemap(Graphics::Texture* Tex);

			void TestRender(Rendering::PBRCapture*);
		protected:
			RefCntAutoPtr<IPipelineState> pERectToCubemap_PSO;
			RefCntAutoPtr<IShaderResourceBinding> pERectToCubemap_SRB;
			RefCntAutoPtr<IBuffer> pCaptureCB;

			Skybox mSkybox;

			//Graphics::RenderTarget mCaptureRT;
			Graphics::RenderTarget mCaptureDepthRT;

			ImageBasedLightingDesc mDesc;
			Math::Matrix4 mCaptureProjection;
			Math::Matrix4 mCaptureViews[6];
		};
	}
}