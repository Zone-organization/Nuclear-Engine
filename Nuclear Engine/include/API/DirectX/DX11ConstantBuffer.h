#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\DirectX\DX11Shader.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			class NEAPI DX11ConstantBuffer
			{
				friend class DX11Shader;
			public:
				DX11ConstantBuffer();
				~DX11ConstantBuffer();
				
				static void Create(DX11ConstantBuffer *buffer,const char *Nameinshader, unsigned int size);
				void Update(const void* data, unsigned int size);

			protected:
				ID3D11Buffer * buffer;

				const char* name;
			};
		}
	}
}
#endif