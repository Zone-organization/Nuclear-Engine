#include <Graphics/API/Sampler.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			Sampler::Sampler()
			{
			}

			Sampler::~Sampler()
			{
			}
			void Sampler::Create(Sampler *result, const SamplerDesc& Desc)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLSampler::Create(&result->GLObject, Desc);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11Sampler::Create(&result->DXObject, Desc);
				}
			}

			void Sampler::Delete(Sampler * result)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLSampler::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11Sampler::Delete(&result->DXObject);
				}
			}
			void Sampler::VSBind(unsigned int index)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.VSBind(index);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.VSBind(index);
				}
			}

			void Sampler::PSBind(unsigned int index)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.PSBind(index);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.PSBind(index);
				}
			}
			void Sampler::GSBind(unsigned int index)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.GSBind(index);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.GSBind(index);
				}
			}
		}
	}
}
#endif