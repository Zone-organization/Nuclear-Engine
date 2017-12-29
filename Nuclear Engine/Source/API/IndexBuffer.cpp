#include <API\IndexBuffer.h>

#ifdef NE_USE_RUNTIME_RENDERER
#include <Core\Context.h>

namespace NuclearEngine {
	namespace API {
		IndexBuffer::IndexBuffer()
		{
		}

		IndexBuffer::~IndexBuffer()
		{
			
		}

		void IndexBuffer::Create(IndexBuffer* result,void * indices, unsigned int count)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLIndexBuffer::Create(&result->GLObject,indices,count);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DirectX::DX11IndexBuffer::Create(&result->DXObject,indices, count);
			}
		}

		void IndexBuffer::Bind()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Bind();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.Bind();
			}
		}
		void IndexBuffer::Unbind()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Unbind();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.Unbind();
			}
		}
		

	}
}

#endif