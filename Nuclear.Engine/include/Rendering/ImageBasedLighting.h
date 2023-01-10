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
			Diligent::TEXTURE_FORMAT mIrradianceCubeFmt = Diligent::TEX_FORMAT_RGBA32_FLOAT;
			Diligent::TEXTURE_FORMAT mPrefilteredEnvMapFmt = Diligent::TEX_FORMAT_RGBA16_FLOAT;
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
			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pERectToCubemap_PSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pERectToCubemap_SRB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pPrecomputeIrradiancePSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pPrecomputeIrradiance_SRB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pPrecomputePrefilterPSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pPrecomputePrefilter_SRB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> pPrecomputeBRDF_PSO;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pPrecomputeBRDF_SRB;

			Diligent::RefCntAutoPtr<Diligent::IBuffer> pCaptureCB;
			Diligent::RefCntAutoPtr<Diligent::IBuffer> pPrefilterRoughnessCB;

			Assets::Texture mBRDF_LUT_Image;

			Rendering::PBRCapture* pEnvCapture;
			ImageBasedLightingDesc mDesc;
		};
	}
}