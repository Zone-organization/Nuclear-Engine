#include <API\BlendState.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Core\Context.h>

namespace NuclearEngine {
	namespace API {
		BlendState::BlendState()
		{
		}

		BlendState::~BlendState()
		{

		}
		void BlendState::Create(BlendState* result, const BlendStateDesc& type)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLBlendState::Create(&result->GLObject, type);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11BlendState::Create(&result->DXObject, type);
			}
		}
		void BlendState::Delete(BlendState * state)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLBlendState::Delete(&state->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11BlendState::Delete(&state->DXObject);
			}
		}
		void BlendState::Bind(API::Color blendfactor, unsigned int samplemask)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Bind(blendfactor, samplemask);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.Bind(blendfactor, samplemask);
			}
		}
		void BlendState::Bind_Default()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLBlendState::Bind_Default();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11BlendState::Bind_Default();
			}
		}
	}
}

#endif