#pragma once
#include <API\DirectX\DX11Common.h>

#ifdef NE_COMPILE_DIRECTX11
#include <API\InputLayout.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace DirectX
		{
			class DX11Shader;
			class NEAPI DX11VertexBuffer
			{
			public:
				DX11VertexBuffer();
				~DX11VertexBuffer();

				static void Create(DX11VertexBuffer* buffer,VertexBufferDesc* desc);

				void Update(const void* data, unsigned int size);

				void SetInputLayout(InputLayout * layout, DX11Shader *shader);

				void Bind();
				void Unbind();
			private:
				ID3D11Buffer* VertexBuffer;
				ID3D11InputLayout* inputLayout;

				unsigned int offset, stride;
			};
		}
	}
}

#endif