#pragma once
#include <Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <Graphics/Color.h>

namespace NuclearEngine
{
	namespace Graphics
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

					void Bind(Graphics::Color blendfactor = Graphics::Color(0.0f), unsigned int samplemask = 0xffffffff);

				protected:
					ID3D11BlendState * Stateobj;
				};
			}
		}
	}
}
#endif