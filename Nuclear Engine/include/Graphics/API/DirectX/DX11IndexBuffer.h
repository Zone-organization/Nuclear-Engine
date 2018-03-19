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
				class NEAPI DX11IndexBuffer
				{
				public:
					DX11IndexBuffer();
					~DX11IndexBuffer();

					static void Create(DX11IndexBuffer* buffer, const void* data, unsigned int size);
					static void Delete(DX11IndexBuffer* buffer);

					void Bind();
				protected:
					ID3D11Buffer * buffer;
				};
			}
		}
	}
}
#endif