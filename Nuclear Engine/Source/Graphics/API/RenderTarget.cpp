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
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLRenderTarget::Create(&result->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11RenderTarget::Create(&result->DXObject);
				}
			}

			void RenderTarget::Delete(RenderTarget * result)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLRenderTarget::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11RenderTarget::Delete(&result->DXObject);
				}
			}

			void RenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.AttachDepthStencilBuffer(size);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.AttachDepthStencilBuffer(size);
				}
			}

			void RenderTarget::AttachTexture(Texture * texture)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.AttachTexture(&texture->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.AttachTexture(&texture->DXObject);
				}
			}

			void RenderTarget::Bind()
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.Bind();
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.Bind();
				}
			}
			void RenderTarget::Bind_Default()
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLRenderTarget::Bind_Default();
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11RenderTarget::Bind_Default();
				}
			}
		}
	}
}
#endif