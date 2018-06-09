#pragma once
#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Engine/Graphics/API/Texture_Types.h>
#include <array>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace DirectX
			{
				class NEAPI DX11Sampler
				{
				public:
					DX11Sampler();
					~DX11Sampler();

					static void Create(DX11Sampler* texture, const SamplerDesc& Desc);
					static void Delete(DX11Sampler* texture);

					void VSBind(Uint8 slot);
					void GSBind(Uint8 slot);
					void PSBind(Uint8 slot);

				protected:
					ID3D11SamplerState * samplerState;
				};
			}
		}
	}
}

#endif