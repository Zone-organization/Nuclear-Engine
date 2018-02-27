#include <API\DirectX\DX11Sampler.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Context.h>
#include <API\DirectX\DX11Types.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{

			DX11Sampler::DX11Sampler() : samplerState(nullptr)
			{
			}

			DX11Sampler::~DX11Sampler()
			{
				samplerState = nullptr;
			}

			void DX11Sampler::Create(DX11Sampler* result, const SamplerDesc& Desc)
			{
				D3D11_SAMPLER_DESC samplerDesc;
				ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

				if (Desc.AnisoFilter == AnisotropicFilter::None)
					samplerDesc.Filter = DXTypeMap(Desc.Filter);				
				else
					samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
				
				samplerDesc.AddressU = DXTypeMap(Desc.WrapU);
				samplerDesc.AddressV = DXTypeMap(Desc.WrapV);
				samplerDesc.AddressW = DXTypeMap(Desc.WrapW);
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

				samplerDesc.MaxAnisotropy = static_cast<unsigned int>(Desc.AnisoFilter);
	
				if (FAILED(DX11Context::GetDevice()->CreateSamplerState(&samplerDesc, &result->samplerState)))
				{
					Log.Error("[DirectX] SamplerState Creation Failed!\n");
					return;
				}
			}


			void DX11Sampler::Delete(DX11Sampler * texture)
			{
				if (texture->samplerState != nullptr)
				{
					texture->samplerState->Release();
				}
				texture->samplerState = nullptr;
			}
			void DX11Sampler::PSBind(unsigned int slot)
			{
				DX11Context::GetContext()->PSSetSamplers(slot, 1, &samplerState);
			}

			void DX11Sampler::VSBind(unsigned int slot)
			{
				DX11Context::GetContext()->VSSetSamplers(slot, 1, &samplerState);
			}
			void DX11Sampler::GSBind(unsigned int slot)
			{
				DX11Context::GetContext()->GSSetSamplers(slot, 1, &samplerState);
			}

		}
	}
}
#endif