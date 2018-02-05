#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\Color.h>

namespace NuclearEngine
{
	namespace API
	{
		struct BlendStateDesc;
		namespace DirectX
		{
			class NEAPI DX11BlendState
			{
			public:
				DX11BlendState();
				~DX11BlendState();

				static void Create(DX11BlendState *state, const BlendStateDesc& type);
				static void Delete(DX11BlendState *state);

				void Bind(API::Color blendfactor = API::Color(0.0f), unsigned int samplemask = 0xffffffff);

				static void Bind_Default();
			protected:
				ID3D11BlendState * Stateobj;
			};
		}
	}
}
#endif