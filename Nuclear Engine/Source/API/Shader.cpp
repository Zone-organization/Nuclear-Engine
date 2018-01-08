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

		void Shader::Create(Shader* result, ShaderDesc* Desc)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				OpenGL::GLShader::Create(&result->GLObject, Desc);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DirectX::DX11Shader::Create(&result->DXObject, Desc);
			}
		}

		void Shader::SetConstantBuffer(ConstantBuffer * cbuffer,API::ShaderType type)
		{
			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				GLObject.SetConstantBuffer(&cbuffer->GLObject,type);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
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
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				DXObject.Bind();
			}
		}
	}
}
#endif