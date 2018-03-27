#include <Graphics/API/RasterizerState.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>

namespace NuclearEngine {
	namespace Graphics {

		namespace API {
			RasterizerState::RasterizerState()
			{
			}

			RasterizerState::~RasterizerState()
			{

			}
			void RasterizerState::Create(RasterizerState* result, const RasterizerStateDesc& type)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLRasterizerState::Create(&result->GLObject, type);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11RasterizerState::Create(&result->DXObject, type);
				}
			}
			void RasterizerState::Delete(RasterizerState * state)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLRasterizerState::Delete(&state->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11RasterizerState::Delete(&state->DXObject);
				}
			}
			void RasterizerState::Bind()
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
		}
	}
}

#endif