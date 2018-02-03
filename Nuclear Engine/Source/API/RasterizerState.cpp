#include <API\RasterizerState.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Core\Context.h>

namespace NuclearEngine {
	namespace API {
		RasterizerState::RasterizerState()
		{
		}

		RasterizerState::~RasterizerState()
		{

		}
		void RasterizerState::Create(RasterizerState* result, const RasterizerStateDesc& type)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLRasterizerState::Create(&result->GLObject, type);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11RasterizerState::Create(&result->DXObject, type);
			}
		}
		void RasterizerState::Delete(RasterizerState * state)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLRasterizerState::Delete(&state->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11RasterizerState::Delete(&state->DXObject);
			}
		}
		void RasterizerState::Bind()
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
		void RasterizerState::Bind_Default()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLRasterizerState::Bind_Default();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11RasterizerState::Bind_Default();
			}
		}
	}
}

#endif