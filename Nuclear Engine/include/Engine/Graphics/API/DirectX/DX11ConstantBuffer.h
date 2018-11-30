#pragma once
#include <Engine/Graphics/API/DirectX\DX11Common.h>

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

					void VSBind(Uint8 slot);
					void GSBind(Uint8 slot);
					void PSBind(Uint8 slot);

					void Update(const void* data, unsigned int size);

					ID3D11Buffer * mCBuffer;
					const char* mName;
				};
			}
		}
	}
}
#endif