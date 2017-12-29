#include <API\Shader.h>
#ifdef NE_USE_RUNTIME_RENDERER
#include <Core\Context.h>
#include <API\ConstantBuffer.h>
namespace NuclearEngine {
	namespace API {

		Shader::Shader()
		{

		}
		
		Shader::~Shader()
		{
		
		}

		void Shader::Create(Shader* result,BinaryShaderBlob * VertexShader, BinaryShaderBlob * PixelShader, BinaryShaderBlob * GeometryShader)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLShader::Create(&result->GLObject, VertexShader, PixelShader, GeometryShader);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DirectX::DX11Shader::Create(&result->DXObject, VertexShader, PixelShader, GeometryShader);
			}
		}

		void Shader::SetConstantBuffer(ConstantBuffer * cbuffer, ShaderType type)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.SetConstantBuffer(&cbuffer->GLObject,type);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.SetConstantBuffer(&cbuffer->DXObject, type);
			}
		}
	
		void Shader::Bind()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Bind();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.Bind();
			}
		}
		void Shader::Unbind()
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.Unbind();
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				DXObject.Unbind();
			}
		}

	}
}
#endif