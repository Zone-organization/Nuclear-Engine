#pragma once
#include <API\DepthStencilState.h>
#include <API\RasterizerState.h>
#include <API\BlendState.h>
#include <API\Sampler.h>
#include <API\RenderStates_Types.h>
namespace NuclearEngine
{
	namespace API 
	{
		//Common States
		class NEAPI CommonStates
		{
		public:
			CommonStates()
			{
				//Create Common States
				API::DepthStencilStateDesc DS_Desc;
				DS_Desc.DepthEnabled = true;
				DS_Desc.StencilEnabled = false;
				DS_Desc.DepthMaskEnabled = true;
				API::DepthStencilState::Create(&EnabledDepth_DisabledStencil, DS_Desc);
				API::DepthStencilState::Create(&DefaultDepthStencil, DS_Desc);
				DS_Desc.DepthEnabled = true;
				DS_Desc.StencilEnabled = true;
				API::DepthStencilState::Create(&EnabledDepthStencil, DS_Desc);
				DS_Desc.DepthEnabled = false;
				DS_Desc.StencilEnabled = false;
				API::DepthStencilState::Create(&DisabledDepthStencil, DS_Desc);
				DS_Desc.DepthEnabled = false;
				DS_Desc.StencilEnabled = true;
				API::DepthStencilState::Create(&DisabledDepth_EnabledStencil, DS_Desc);

				API::RasterizerStateDesc RS_Desc;
				API::RasterizerState::Create(&DefaultRasterizer, RS_Desc);
				API::BlendStateDesc BS_Desc;
				API::BlendState::Create(&DefaultBlendState, BS_Desc);

				//Create sampler
				API::SamplerDesc Samplerdesc;
				Samplerdesc.Filter = API::TextureFilter::Trilinear;
				API::Sampler::Create(&DefaultSampler, Samplerdesc);
			}
			~CommonStates()
			{
				API::DepthStencilState::Delete(&DefaultDepthStencil);
				API::DepthStencilState::Delete(&EnabledDepth_DisabledStencil);
				API::DepthStencilState::Delete(&EnabledDepthStencil);
				API::DepthStencilState::Delete(&DisabledDepthStencil);
				API::DepthStencilState::Delete(&DisabledDepth_EnabledStencil);
				API::RasterizerState::Delete(&DefaultRasterizer);
				API::BlendState::Delete(&DefaultBlendState);
				API::Sampler::Delete(&DefaultSampler);
			}
			DepthStencilState DisabledDepthStencil;
			DepthStencilState EnabledDepthStencil;
			DepthStencilState DisabledDepth_EnabledStencil;
			DepthStencilState EnabledDepth_DisabledStencil;
			DepthStencilState DefaultDepthStencil;
			RasterizerState DefaultRasterizer;
			BlendState DefaultBlendState;
			Sampler DefaultSampler;
		};
	}
}
