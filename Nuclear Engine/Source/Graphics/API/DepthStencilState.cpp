#include <Graphics/API/DepthStencilState.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {

		namespace API {
			DepthStencilState::DepthStencilState()
			{
			}

			DepthStencilState::~DepthStencilState()
			{

			}
			void DepthStencilState::Create(DepthStencilState* result, const DepthStencilStateDesc& type)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLDepthStencilState::Create(&result->GLObject, type);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11DepthStencilState::Create(&result->DXObject, type);
				}
			}
			void DepthStencilState::Delete(DepthStencilState * state)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLDepthStencilState::Delete(&state->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11DepthStencilState::Delete(&state->DXObject);
				}
			}
			void DepthStencilState::Bind()
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
		}
	}
}
#endif