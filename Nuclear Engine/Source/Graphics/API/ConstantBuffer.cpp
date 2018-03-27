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
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLConstantBuffer::Create(&result->GLObject, _nameinshaders, size);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11ConstantBuffer::Create(&result->DXObject, _nameinshaders, size);
				}
			}
			void ConstantBuffer::Delete(ConstantBuffer * buffer)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLConstantBuffer::Delete(&buffer->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11ConstantBuffer::Delete(&buffer->DXObject);
				}
			}
			void ConstantBuffer::Update(const void* data, unsigned int size)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.Update(data, size);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.Update(data, size);
				}

			}
		}
	}
}
#endif