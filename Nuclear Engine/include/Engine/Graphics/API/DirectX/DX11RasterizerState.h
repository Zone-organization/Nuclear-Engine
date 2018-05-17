#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct RasterizerStateDesc;
			namespace DirectX
			{
				class NEAPI DX11RasterizerState
				{
				public:
					DX11RasterizerState();
					~DX11RasterizerState();

					static void Create(DX11RasterizerState *state, const RasterizerStateDesc& type);
					static void Delete(DX11RasterizerState *state);

					void Bind();

				protected:
					ID3D11RasterizerState * RS_State;
				};
			}
		}
	}
}
#endif