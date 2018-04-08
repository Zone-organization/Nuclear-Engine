#include <Graphics/API/RenderTarget.h>
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

			void RenderTarget::Create(RenderTarget* obj)
			{
				STATIC_BASE_API_FUNC_CALL(Create)
			}

			void RenderTarget::Delete(RenderTarget * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
			}

			void RenderTarget::AttachDepthStencilBuffer(const Math::Vector2ui & size)
			{
				BASE_API_FUNC_CALL_ARGS(AttachDepthStencilBuffer,size)
			}

			void RenderTarget::AttachTexture(Texture * texture)
			{
				SPECIAL_BASE_API_FUNC_CALL(AttachTexture, texture)
			}

			void RenderTarget::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
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