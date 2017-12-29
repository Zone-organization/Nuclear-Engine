#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			class NEAPI DX11IndexBuffer
			{
			public:
				DX11IndexBuffer();
				~DX11IndexBuffer();

				static void Create(DX11IndexBuffer* buffer,const void* data, unsigned int size);

				void Bind();

				void Unbind();
			protected:
				ID3D11Buffer* buffer;
			};
		}
	}
}
#endif