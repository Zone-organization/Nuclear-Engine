#include <Engine/Graphics/API/RenderTarget.h>
#include <Engine\Graphics\API\Context.h>
#include <Engine/Graphics/API/Texture.h>
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
				SPECIAL_BASE_API_FUNC_CALL_ARGS(AttachTexture, texture, texture->GetTextureDesc())
			}

			void RenderTarget::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
			void RenderTarget::Bind_Default()
			{
				STATIC_BASE_API_FUNC_CALL_SPECIAL_FUNC(RenderTarget, Bind_Default)
			}
		}
	}
}