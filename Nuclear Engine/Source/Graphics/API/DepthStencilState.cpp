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
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLDepthStencilState::Create(&result->GLObject, type);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11DepthStencilState::Create(&result->DXObject, type);
				}
			}
			void DepthStencilState::Delete(DepthStencilState * state)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLDepthStencilState::Delete(&state->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11DepthStencilState::Delete(&state->DXObject);
				}
			}
			void DepthStencilState::Bind()
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