#include <API\ConstantBuffer.h>
#ifdef NE_USE_RUNTIME_RENDERER
#include <Core\Context.h>

namespace NuclearEngine {
	namespace API {
		ConstantBuffer::ConstantBuffer()
		{

		}
		
		ConstantBuffer::~ConstantBuffer()
		{
			
		}
		void ConstantBuffer::Create(ConstantBuffer* result,const char * _nameinshaders, unsigned int size)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLConstantBuffer::Create(&result->GLObject,_nameinshaders,size);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DirectX::DX11ConstantBuffer::Create(&result->DXObject, _nameinshaders, size);
			}
		}
		void ConstantBuffer::Update(const void* data, unsigned int size)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Update(data, size);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.Update(data, size);
			}

		}
		
	}
}
#endif