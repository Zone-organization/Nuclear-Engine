#pragma once
#include <Graphics/API/GraphicsAPICommon.h>
#include <Graphics/API/OpenGL\GLVertexBuffer.h>
#include <Graphics/API/DirectX\DX11VertexBuffer.h>

namespace NuclearEngine {
	namespace Graphics {

		namespace API {
			class InputLayout;
			class VertexShader;
			struct VertexBufferDesc;
			class NEAPI VertexBuffer : public CBaseAPI<OpenGL::GLVertexBuffer, DirectX::DX11VertexBuffer>
			{
			public:
				VertexBuffer();
				~VertexBuffer();

				static void Create(VertexBuffer* buffer, const VertexBufferDesc& desc);
				static void Delete(VertexBuffer* buffer);

				void Update(const void* data, unsigned int size);

				void SetInputLayout(InputLayout* layout, VertexShader* shader);

				void Bind();
			};
		}
	}
}
