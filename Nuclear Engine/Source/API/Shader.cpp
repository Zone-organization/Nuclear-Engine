#include <API\Shader.h>
#include <Core\Context.h>
#include <NuclearRendererBase/NRBShader.h>
#include <NuclearRendererOGL3/GLShader.h>

namespace NuclearEngine {
	namespace API {

		Shader::Shader(const char *name, const char* VertexShader, const char* PixelShader, const char* GeometryShader, ShaderType inputtype)
		{
			Log->Info("[Shader] Initializing Shader: ");
			Log->Info(name);
			Log->Info("\n");

			shader = Core::Context::ConstructShader(shader);

			shader->Create(VertexShader, PixelShader, GeometryShader, inputtype);
		}

		Shader::~Shader()
		{
			shader->Delete();

			delete shader;
		}

		void Shader::SetUniformBuffer(UniformBuffer * cbuffer)
		{
			shader->SetUniformBuffer(cbuffer->GetBase());
		}
		void Shader::Bind()
		{
			shader->Bind();
		}
		void Shader::Unbind()
		{
			shader->Unbind();
		}

		NuclearRenderer::NRBShader * Shader::GetBase()
		{
			return shader;
		}

	}
}