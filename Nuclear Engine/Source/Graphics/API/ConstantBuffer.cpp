#include <Graphics/API/ConstantBuffer.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			ConstantBuffer::ConstantBuffer()
			{

			}

			ConstantBuffer::~ConstantBuffer()
			{

			}
			void ConstantBuffer::Create(ConstantBuffer* result, const char * _nameinshaders, unsigned int size)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLConstantBuffer::Create(&result->GLObject, _nameinshaders, size);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11ConstantBuffer::Create(&result->DXObject, _nameinshaders, size);
				}
			}
			void ConstantBuffer::Delete(ConstantBuffer * buffer)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLConstantBuffer::Delete(&buffer->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11ConstantBuffer::Delete(&buffer->DXObject);
				}
			}
			void ConstantBuffer::Update(const void* data, unsigned int size)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.Update(data, size);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.Update(data, size);
				}

			}
		}
	}
}
#endif