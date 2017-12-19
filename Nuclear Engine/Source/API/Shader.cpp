#include "..\..\..\Nuclear Renderer Base\include\NuclearRendererBase\NRBShader.h"
#include <API\Shader.h>
#include <Core\Context.h>
#include <NuclearRendererBase/NRBShader.h>
#include <NuclearRendererOGL3/GLShader.h>
#include <hlslcc.h>
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")

namespace NuclearEngine {
	namespace API {


		void CheckShaderErrors(ID3D10Blob* Shader)
		{
			char* compileErrors;

			// Get a pointer to the error message text Buffer.
			compileErrors = (char*)(Shader->GetBufferPointer());

			Log->Info(compileErrors);

			// Release the error message.
			Shader->Release();
			Shader = 0;

			return;
		}

		Shader::Shader()
		{

		}
		Shader::Shader(const char *name, BinaryShaderBlob* VertexShader, BinaryShaderBlob* PixelShader, BinaryShaderBlob* GeometryShader)
		{
			Log->Info("[Shader] Initializing Shader: ");
			Log->Info(name);
			Log->Info("\n");

			this->vblob = VertexShader;
			this->pblob = PixelShader;
			this->gblob = GeometryShader;


			shader = Core::Context::ConstructShader(shader);

			shader->Create(VertexShader, PixelShader, GeometryShader);
		}

		Shader::~Shader()
		{
			if (shader != nullptr)
			{
				shader->Delete();
				delete shader;
				shader = nullptr;
			}
		}

		void Shader::SetUniformBuffer(UniformBuffer* cbuffer, unsigned int slot, ShaderType type)
		{
			shader->SetUniformBuffer(cbuffer->GetBase(), slot, type);
		}
		void Shader::SetUniformBuffer(UniformBuffer * cbuffer, ShaderType type)
		{
			shader->SetUniformBuffer(cbuffer->GetBase(), shader->GetUniformBufferSlot(cbuffer->GetBase(), type), type);
		}
		unsigned int Shader::GetUniformBufferSlot(UniformBuffer * cbuffer, ShaderType type)
		{
			return shader->GetUniformBufferSlot(cbuffer->GetBase(), type);
		}
		void Shader::Bind()
		{
			shader->Bind();
		}
		void Shader::Unbind()
		{
			shader->Unbind();
		}

		ShaderReflection Shader::GetReflection(ShaderType type)
		{
			if (type == ShaderType::Vertex)
			{
				return vblob->reflection;
			}
			else if (type == ShaderType::Pixel)
			{
				return pblob->reflection;
			}
			else if (type == ShaderType::Geometry)
			{
				return gblob->reflection;
			}
			return ShaderReflection();
		}

		NuclearRenderer::NRBShader * Shader::GetBase()
		{
			return shader;
		}

		BinaryShaderBlob CompileShader(const char * SourceCode, ShaderType type, ShaderLanguage language)
		{
			BinaryShaderBlob blob;

			const char* shadermodel;
			if (type == ShaderType::Vertex)
			{
				shadermodel = "vs_4_1";
			}
			else if (type == ShaderType::Pixel)
			{
				shadermodel = "ps_4_1";
			}
			else if (type == ShaderType::Geometry)
			{
				shadermodel = "gs_4_1";
			}
		
			if (language == ShaderLanguage::HLSL)
			{
				ID3D10Blob* ERRMSG = nullptr;
				ID3DBlob* hlslblob;

				if (FAILED(D3DCompile(SourceCode, lstrlenA(SourceCode) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shadermodel, 0, 0, &hlslblob, &ERRMSG)))
				{
					Log->Info("[ShaderCompiler] Compiling Error -- \nInfo: ");
					CheckShaderErrors(ERRMSG);

					return blob;
				}
				blob.hlslsourcecode = hlslblob->GetBufferPointer();
				blob.hlslsize = hlslblob->GetBufferSize();
				blob.Language = ShaderLanguage::DXBC;

				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GlExtensions extensions;
					GLSLCrossDependencyData deps;
					extensions.ARB_shading_language_420pack = false;
					HLSLccSamplerPrecisionInfo samplerPrecisions;
					HLSLccReflection reflectionCallbacks;
					GLSLShader glshader;

					TranslateHLSLFromMem((const char*)blob.hlslsourcecode, HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT,
						LANG_330, &extensions, &deps, samplerPrecisions, reflectionCallbacks, &glshader);

					blob.glslsourcecode = glshader.sourceCode;				
					blob.Language = ShaderLanguage::GLSL;
				}				
			}	
			else if (language == ShaderLanguage::GLSL)
			{
				Exceptions::NotImplementedException();
			}
			return blob;
		}

	}
}