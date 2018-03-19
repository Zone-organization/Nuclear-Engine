#pragma once
#include <Graphics/API/DepthStencilState.h>
#include <Graphics/API/RasterizerState.h>
#include <Graphics/API/BlendState.h>
#include <Graphics/API/Sampler.h>
#include <Graphics/API/RenderStates_Types.h>
namespace NuclearEngine
{
	namespace Graphics
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
					Graphics::API::DepthStencilStateDesc DS_Desc;
					DS_Desc.DepthEnabled = true;
					DS_Desc.StencilEnabled = false;
					DS_Desc.DepthMaskEnabled = true;
					Graphics::API::DepthStencilState::Create(&EnabledDepth_DisabledStencil, DS_Desc);
					Graphics::API::DepthStencilState::Create(&DefaultDepthStencil, DS_Desc);
					DS_Desc.DepthEnabled = true;
					DS_Desc.StencilEnabled = true;
					Graphics::API::DepthStencilState::Create(&EnabledDepthStencil, DS_Desc);
					DS_Desc.DepthEnabled = false;
					DS_Desc.StencilEnabled = false;
					Graphics::API::DepthStencilState::Create(&DisabledDepthStencil, DS_Desc);
					DS_Desc.DepthEnabled = false;
					DS_Desc.StencilEnabled = true;
					Graphics::API::DepthStencilState::Create(&DisabledDepth_EnabledStencil, DS_Desc);

					Graphics::API::RasterizerStateDesc RS_Desc;
					Graphics::API::RasterizerState::Create(&DefaultRasterizer, RS_Desc);
					Graphics::API::BlendStateDesc BS_Desc;
					Graphics::API::BlendState::Create(&DefaultBlendState, BS_Desc);

					//Create sampler
					Graphics::API::SamplerDesc Samplerdesc;
					Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
					Graphics::API::Sampler::Create(&DefaultSampler, Samplerdesc);
				}
				~CommonStates()
				{
					Graphics::API::DepthStencilState::Delete(&DefaultDepthStencil);
					Graphics::API::DepthStencilState::Delete(&EnabledDepth_DisabledStencil);
					Graphics::API::DepthStencilState::Delete(&EnabledDepthStencil);
					Graphics::API::DepthStencilState::Delete(&DisabledDepthStencil);
					Graphics::API::DepthStencilState::Delete(&DisabledDepth_EnabledStencil);
					Graphics::API::RasterizerState::Delete(&DefaultRasterizer);
					Graphics::API::BlendState::Delete(&DefaultBlendState);
					Graphics::API::Sampler::Delete(&DefaultSampler);
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
}