#include <Graphics/API/BlendState.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			BlendState::BlendState()
			{
			}

			BlendState::~BlendState()
			{

			}
			void BlendState::Create(BlendState* result, const BlendStateDesc& type)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLBlendState::Create(&result->GLObject, type);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11BlendState::Create(&result->DXObject, type);
				}
			}
			void BlendState::Delete(BlendState * state)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLBlendState::Delete(&state->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11BlendState::Delete(&state->DXObject);
				}
			}
			void BlendState::Bind(Graphics::Color blendfactor, unsigned int samplemask)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.Bind(blendfactor, samplemask);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.Bind(blendfactor, samplemask);
				}
			}

		}
	}
}

#endif