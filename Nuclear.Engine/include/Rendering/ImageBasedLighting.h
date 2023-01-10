#pragma once
#include <Rendering/PBRCapture.h>
#include <Graphics/RenderTarget.h>
#include <Assets/MaterialTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Rendering/Skybox.h>

namespace Nuclear
{
	namespace Rendering
	{
		struct ImageBasedLightingDesc
		{
			TEXTURE_FORMAT mIrradianceCubeFmt = TEX_FORMAT_RGBA32_FLOAT;
			TEXTURE_FORMAT mPrefilteredEnvMapFmt = TEX_FORMAT_RGBA16_FLOAT;
			Uint32         mIrradianceCubeDim = 32;
			Uint32         mPrefilteredEnvMapDim = 128;
			Uint32         mBRDF_LUTDim = 512;
		};

		class NEAPI ImageBasedLighting
		{
		public:
			void Initialize(const ImageBasedLightingDesc& desc);

			//TODO Move to asset manager
			Assets::Texture EquirectangularToCubemap(Assets::MaterialTexture* Tex);

			Rendering::PBRCapture PrecomputePBRCapture(Assets::Texture* cubemap);

			void SetEnvironmentCapture(Rendering::PBRCapture* cap);
			Rendering::PBRCapture* GetEnvironmentCapture();

			Assets::Texture* GetBRDF_LUT();
		protected:
			RefCntAutoPtr<IPipelineState> pERectToCubemap_PSO;
			RefCntAutoPtr<IShaderResourceBinding> pERectToCubemap_SRB;

			RefCntAutoPtr<IPipelineState> pPrecomputeIrradiancePSO;
			RefCntAutoPtr<IShaderResourceBinding> pPrecomputeIrradiance_SRB;

			RefCntAutoPtr<IPipelineState> pPrecomputePrefilterPSO;
			RefCntAutoPtr<IShaderResourceBinding> pPrecomputePrefilter_SRB;

			RefCntAutoPtr<IPipelineState> pPrecomputeBRDF_PSO;
			RefCntAutoPtr<IShaderResourceBinding> pPrecomputeBRDF_SRB;

			RefCntAutoPtr<IBuffer> pCaptureCB;
			RefCntAutoPtr<IBuffer> pPrefilterRoughnessCB;

			Assets::Texture mBRDF_LUT_Image;

			Rendering::PBRCapture* pEnvCapture;
			ImageBasedLightingDesc mDesc;
		};
	}
}