#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
namespace NuclearEngine
{
	struct DepthStencilStateDesc;

	namespace API
	{
		namespace DirectX
		{
			class NEAPI DX11DepthStencilState
			{
			public:
				DX11DepthStencilState();
				~DX11DepthStencilState();

				static void Create(DX11DepthStencilState *state,DepthStencilStateDesc *type);
				static void Delete(DX11DepthStencilState *state);

				void Bind();

				void Unbind();
			protected:
				ID3D11DepthStencilState* DS_State;
			};
		}
	}
}
#endif