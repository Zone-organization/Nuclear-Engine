#include <Graphics/API/RenderTarget.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>
#include <Graphics/API/Texture.h>
namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			RenderTarget::RenderTarget()
			{

			}

			RenderTarget::~RenderTarget()
			{

			}

			void RenderTarget::Create(RenderTarget* result)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLRenderTarget::Create(&result->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11RenderTarget::Create(&result->DXObject);
				}
			}

			void RenderTarget::Delete(RenderTarget * result)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLRenderTarget::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11RenderTarget::Delete(&result->DXObject);
				}
			}

			void RenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.AttachDepthStencilBuffer(size);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.AttachDepthStencilBuffer(size);
				}
			}

			void RenderTarget::AttachTexture(Texture * texture)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.AttachTexture(&texture->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.AttachTexture(&texture->DXObject);
				}
			}

			void RenderTarget::Bind()
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.Bind();
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.Bind();
				}
			}
			void RenderTarget::Bind_Default()
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLRenderTarget::Bind_Default();
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11RenderTarget::Bind_Default();
				}
			}
		}
	}
}
#endif