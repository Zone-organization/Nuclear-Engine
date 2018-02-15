#pragma once
#include <NE_Common.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <API\OpenGL\GLVertexBuffer.h>
#include <API\DirectX\DX11VertexBuffer.h>

namespace NuclearEngine {
	namespace API {
		class InputLayout;
		class Shader;
		class VertexShader;
		struct VertexBufferDesc;
		class NEAPI VertexBuffer
		{
		public:
			VertexBuffer();
			~VertexBuffer();

			static void Create(VertexBuffer* buffer, const VertexBufferDesc& desc);
			static void Delete(VertexBuffer* buffer);

			void Update(const void* data, unsigned int size);

			void SetInputLayout(InputLayout* layout, Shader* shader);
			void SetInputLayout(InputLayout* layout, VertexShader* shader);

			void Bind();
		protected:
			OpenGL::GLVertexBuffer GLObject;
			DirectX::DX11VertexBuffer DXObject;
		};
	}
}

#else
#ifdef NE_USE_CORE_OPENGL
#include <API\OpenGL\GLVertexBuffer.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef OpenGL::GLVertexBuffer VertexBuffer;
	}
}
#endif
#ifdef NE_USE_DIRECTX11
#include <API\DirectX\DX11VertexBuffer.h>
namespace NuclearEngine
{
	namespace API
	{
		typedef DirectX::DX11VertexBuffer VertexBuffer;
	}
}
#endif
#endif