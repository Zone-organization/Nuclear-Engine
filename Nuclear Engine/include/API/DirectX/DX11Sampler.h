#pragma once
#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\Texture_Types.h>
#include <array>

namespace NuclearEngine
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

				void VSBind(unsigned int slot);
				void GSBind(unsigned int slot);
				void PSBind(unsigned int slot);

			protected:
				ID3D11SamplerState* samplerState;
			};
		}
	}
}

#endif