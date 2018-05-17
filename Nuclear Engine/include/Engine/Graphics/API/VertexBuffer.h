#pragma once
#include <Engine/Graphics/API/GraphicsAPICommon.h>
#include <Engine/Graphics/API/OpenGL\GLVertexBuffer.h>
#include <Engine/Graphics/API/DirectX\DX11VertexBuffer.h>

namespace NuclearEngine {
	namespace Graphics {

		namespace API {
			class InputLayout;
			class VertexShader;
			struct VertexBufferDesc;
			class NEAPI VertexBuffer //<OpenGL::GLVertexBuffer, DirectX::DX11VertexBuffer>
			{
			public:
				VertexBuffer();
				~VertexBuffer();

				static void Create(VertexBuffer* buffer, const VertexBufferDesc& desc);
				static void Delete(VertexBuffer* buffer);

				void SetInputLayout(InputLayout* layout, VertexShader* shader);
				void Update(const void* data, unsigned int size);
				void Bind();
				void* Map();
				void Unmap();

				BASE_API(VertexBuffer)
			};
		}
	}
}
