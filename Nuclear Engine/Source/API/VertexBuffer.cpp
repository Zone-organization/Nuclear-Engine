#include <API\VertexBuffer.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Core\Context.h>
#include <API\InputLayout.h>
#include <API\Shader.h>
#include <API\VertexShader.h>

namespace NuclearEngine {
	namespace API {
		VertexBuffer::VertexBuffer()
		{
		}
	
		VertexBuffer::~VertexBuffer()
		{
		}

		void VertexBuffer::Create(VertexBuffer * buffer, const VertexBufferDesc& desc)
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

		void VertexBuffer::Delete(VertexBuffer * buffer)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLVertexBuffer::Delete(&buffer->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11VertexBuffer::Delete(&buffer->DXObject);
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

		void VertexBuffer::SetInputLayout(InputLayout * layout, VertexShader * shader)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.SetInputLayout(layout, &shader->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.SetInputLayout(layout, &shader->DXObject);
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