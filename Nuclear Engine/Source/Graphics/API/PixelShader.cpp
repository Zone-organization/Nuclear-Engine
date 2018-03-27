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
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLPixelShader::Create(&result->GLObject, Desc);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11PixelShader::Create(&result->DXObject, Desc);
				}
			}

			void PixelShader::Delete(PixelShader * result)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					OpenGL::GLPixelShader::Delete(&result->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DirectX::DX11PixelShader::Delete(&result->DXObject);
				}
			}

			void PixelShader::SetConstantBuffer(ConstantBuffer * cbuffer)
			{
				if (Graphics::API::Context::isOpenGL3RenderAPI())
				{
					GLObject.SetConstantBuffer(&cbuffer->GLObject);
				}
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					DXObject.SetConstantBuffer(&cbuffer->DXObject);
				}
			}

			void PixelShader::Bind()
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