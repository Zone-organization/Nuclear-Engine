#include <API\Shader.h>
#include <Core\Context.h>
#include <NuclearRendererBase/NRBShader.h>
#include <NuclearRendererOGL3/GLShader.h>
#include "..\Thirdparty\HLSLCC\include\hlslcc.h"
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

		Shader::Shader(const char *name, const char* VertexShader, const char* PixelShader, const char* GeometryShader, ShaderLanguage inputtype)
		{
			Log->Info("[Shader] Initializing Shader: ");
			Log->Info(name);
			Log->Info("\n");


			shader = Core::Context::ConstructShader(shader);

			bool result = true;
			if (inputtype == ShaderLanguage::HLSL)
			{
				if (VertexShader != nullptr)
				{
					ID3D10Blob* ERRMSG = nullptr;

					if (FAILED(D3DCompile(VertexShader, lstrlenA(VertexShader) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_1", 0, 0, &m_VSBL, &ERRMSG)))
					{
						Log->Info("[Shader] Compiling Error -- In Vertex Shader.\nInfo: ");
						CheckShaderErrors(ERRMSG);
						result = false;
					}
				}
				if (PixelShader != nullptr)
				{
					ID3D10Blob* ERRMSG = nullptr;

					if (FAILED(D3DCompile(PixelShader, lstrlenA(PixelShader) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_1", 0, 0, &m_PSBL, &ERRMSG)))
					{
						Log->Info("[Shader] Compiling Error -- In Pixel Shader.\nInfo: ");
						CheckShaderErrors(ERRMSG);

						result = false;
					}
				}
				if (GeometryShader != nullptr)
				{
					ID3D10Blob* ERRMSG = nullptr;

					if (FAILED(D3DCompile(GeometryShader, lstrlenA(GeometryShader) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_4_1", 0, 0, &m_GSBL, &ERRMSG)))
					{
						Log->Info("[Shader] Compiling Error -- In Geometry Shader.\nInfo: ");
						CheckShaderErrors(ERRMSG);

						result = false;
					}
				}
			}		
				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLSLShader shader;
					HLSLccReflection reflection;
					HLSLccSamplerPrecisionInfo info;
					TranslateHLSLFromMem((const char*)m_VSBL->GetBufferPointer(), HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT, LANG_330,
						NULL, NULL, info, reflection, &shader);

					std::cout << shader.sourceCode;
				}


			shader->Create(VertexShader, PixelShader, GeometryShader, inputtype);
		}

		Shader::~Shader()
		{
			shader->Delete();

			delete shader;
		}

		void Shader::SetUniformBuffer(UniformBuffer* cbuffer, unsigned int slot, ShaderType type)
		{
			shader->SetUniformBuffer(cbuffer->GetBase(), slot, type);
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

		NuclearRenderer::NRBShader * Shader::GetBase()
		{
			return shader;
		}

	}
}