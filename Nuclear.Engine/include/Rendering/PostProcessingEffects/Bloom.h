#pragma once
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Rendering\PostProcessingEffects\PostProcessingEffect.h>
#include <vector>
#include <ECS\System.h>
#include <Rendering/BlurEffect.h>
#include <Graphics/ShaderPipeline.h>
#include <Rendering\Background.h>

namespace Nuclear
{
	namespace Rendering
	{
		namespace PostProcessingEffects
		{


			class NEAPI Bloom : public PostProcessingEffect
			{
			public:
				Bloom();

				void Bake(const PostProcessingEffectBakingDesc& desc) override;

				void Update(FrameRenderData* framedata) override;

				void ResizeRTs(Uint32 Width, Uint32 Height) override;

				std::string GetName() override;

				Uint32 GetHashedName() override;
			protected:
				Graphics::RenderTarget BloomRT;
				Rendering::BlurEffect mBloomBlur;
				Diligent::RefCntAutoPtr<Diligent::IPipelineState> pBloomExtractPSO;
				Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> pBloomExtractSRB;

				Uint32 mRTWidth = 0;
				Uint32 mRTHeight = 0;
			};

		}
	}
}