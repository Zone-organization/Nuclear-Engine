#pragma once
#include <Graphics/API/DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11

namespace NuclearEngine
{
	namespace Graphics
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

					static void Create(DX11ConstantBuffer *buffer, const char *Nameinshader, unsigned int size);
					static void Delete(DX11ConstantBuffer *cbuffer);

					void Update(const void* data, unsigned int size);
					const char* GetName();

					ID3D11Buffer ** GetBufferPtr();
				protected:
					ID3D11Buffer * buffer;
					const char* name;
				};
			}
		}
	}
}
#endif