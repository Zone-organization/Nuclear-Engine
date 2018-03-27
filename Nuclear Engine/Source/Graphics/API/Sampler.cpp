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
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLSampler::Create(&result->GLObject, Desc);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11Sampler::Create(&result->DXObject, Desc);
				}
			}

			void Sampler::Delete(Sampler * result)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLSampler::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11Sampler::Delete(&result->DXObject);
				}
			}
			void Sampler::VSBind(unsigned int index)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.VSBind(index);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.VSBind(index);
				}
			}

			void Sampler::PSBind(unsigned int index)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.PSBind(index);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.PSBind(index);
				}
			}
			void Sampler::GSBind(unsigned int index)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.GSBind(index);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.GSBind(index);
				}
			}
		}
	}
}
#endif