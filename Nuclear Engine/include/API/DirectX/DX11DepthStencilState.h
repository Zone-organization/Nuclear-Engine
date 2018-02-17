#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
namespace NuclearEngine
{
	namespace API
	{
		struct DepthStencilStateDesc;
		namespace DirectX
		{
			class NEAPI DX11DepthStencilState
			{
			public:
				DX11DepthStencilState();
				~DX11DepthStencilState();

				static void Create(DX11DepthStencilState *state,const DepthStencilStateDesc& type);
				static void Delete(DX11DepthStencilState *state);

				void Bind();

			protected:
				ID3D11DepthStencilState* DS_State;
			};
		}
	}
}
#endif