#include <Graphics/API/IndexBuffer.h>

#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			IndexBuffer::IndexBuffer()
			{
			}

			IndexBuffer::~IndexBuffer()
			{

			}

			void IndexBuffer::Create(IndexBuffer* result, void * indices, unsigned int count)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLIndexBuffer::Create(&result->GLObject, indices, count);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11IndexBuffer::Create(&result->DXObject, indices, count);
				}
			}

			void IndexBuffer::Delete(IndexBuffer * buffer)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLIndexBuffer::Delete(&buffer->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11IndexBuffer::Delete(&buffer->DXObject);
				}
			}

			void IndexBuffer::Bind()
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.Bind();
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.Bind();
				}
			}
		}
	}
}

#endif