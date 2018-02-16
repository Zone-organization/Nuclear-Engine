#include <API\RenderTarget.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Core\Context.h>
#include <API\Texture.h>
namespace NuclearEngine {
	namespace API {

		RenderTarget::RenderTarget()
		{

		}

		RenderTarget::~RenderTarget()
		{

		}

		void RenderTarget::Create(RenderTarget* result)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLRenderTarget::Create(&result->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11RenderTarget::Create(&result->DXObject);
			}
		}

		void RenderTarget::Delete(RenderTarget * result)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLRenderTarget::Delete(&result->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11RenderTarget::Delete(&result->DXObject);
			}
		}

		void RenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.AttachDepthStencilBuffer(size);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.AttachDepthStencilBuffer(size);
			}
		}

		void RenderTarget::AttachTexture(Texture * texture)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.AttachTexture(&texture->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.AttachTexture(&texture->DXObject);
			}
		}

		void RenderTarget::Bind()
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
		void RenderTarget::Bind_Default()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLRenderTarget::Bind_Default();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11RenderTarget::Bind_Default();
			}
		}
	}
}
#endif