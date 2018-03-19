#include <Graphics/API/PixelShader.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>
#include <Graphics/API/ConstantBuffer.h>
namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			PixelShader::PixelShader()
			{

			}

			PixelShader::~PixelShader()
			{

			}

			void PixelShader::Create(PixelShader* result, BinaryShaderBlob* Desc)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLPixelShader::Create(&result->GLObject, Desc);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11PixelShader::Create(&result->DXObject, Desc);
				}
			}

			void PixelShader::Delete(PixelShader * result)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLPixelShader::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11PixelShader::Delete(&result->DXObject);
				}
			}

			void PixelShader::SetConstantBuffer(ConstantBuffer * cbuffer)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.SetConstantBuffer(&cbuffer->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.SetConstantBuffer(&cbuffer->DXObject);
				}
			}

			void PixelShader::Bind()
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