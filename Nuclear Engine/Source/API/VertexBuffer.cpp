#include <API\VertexBuffer.h>
#include <API\InputLayout.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Core\Context.h>

namespace NuclearEngine {
	namespace API {
		VertexBuffer::VertexBuffer()
		{
		}
	
		VertexBuffer::~VertexBuffer()
		{
		}

		void VertexBuffer::Create(VertexBuffer * buffer, VertexBufferDesc * desc)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLVertexBuffer::Create(&buffer->GLObject,desc);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11VertexBuffer::Create(&buffer->DXObject,desc);
			}
		}

		void VertexBuffer::Update(const void * data, unsigned int size)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Update(data,size);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.Update(data,size);
			}
		}

		void VertexBuffer::SetInputLayout(InputLayout* layout, Shader* shader)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.SetInputLayout(layout,&shader->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.SetInputLayout(layout,&shader->DXObject);
			}
		}

		void VertexBuffer::Bind()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Bind();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.Bind();
			}
		}
			
	}
}
#endif