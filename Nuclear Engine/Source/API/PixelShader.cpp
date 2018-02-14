#include <API\PixelShader.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Core\Context.h>
#include <API\ConstantBuffer.h>
namespace NuclearEngine {
	namespace API {

		PixelShader::PixelShader()
		{

		}

		PixelShader::~PixelShader()
		{

		}

		void PixelShader::Create(PixelShader* result, BinaryShaderBlob* Desc)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLPixelShader::Create(&result->GLObject, Desc);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11PixelShader::Create(&result->DXObject, Desc);
			}
		}

		void PixelShader::Delete(PixelShader * result)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLPixelShader::Delete(&result->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11PixelShader::Delete(&result->DXObject);
			}
		}

		void PixelShader::SetConstantBuffer(ConstantBuffer * cbuffer)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.SetConstantBuffer(&cbuffer->GLObject);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.SetConstantBuffer(&cbuffer->DXObject);
			}
		}

		void PixelShader::Bind()
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
	}
}
#endif