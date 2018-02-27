#pragma once
#include <API\DirectX\DX11Common.h>
#ifdef NE_COMPILE_DIRECTX11
namespace NuclearEngine
{
	namespace API
	{
		class InputLayout;
		struct VertexBufferDesc;

		namespace DirectX
		{
			class DX11VertexShader;

			class NEAPI DX11VertexBuffer
			{
			public:
				DX11VertexBuffer();
				~DX11VertexBuffer();

				static void Create(DX11VertexBuffer* buffer, const VertexBufferDesc& desc);
				static void Delete(DX11VertexBuffer* buffer);

				void Update(const void* data, unsigned int size);

				void SetInputLayout(InputLayout * layout, DX11VertexShader *shader);

				void Bind();
			private:
				ID3D11Buffer* VertexBuffer;
				ID3D11InputLayout* inputLayout;

				unsigned int offset, stride;
			};
		}
	}
}

#endif