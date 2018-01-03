#include <API\ShaderCompiler.h>
#include <API\Shader_Types.h>
#include <Core\Context.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <Xsc\Xsc.h>
#endif

#ifdef NE_COMPILE_D3DCOMPILER
#include <d3dcompiler.h>
#pragma comment(lib,"D3DCompiler.lib")
#endif

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

		BinaryShaderBlob CompileShader(std::string SourceCode, ShaderType type, ShaderLanguage language)
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

				if (FAILED(D3DCompile(SourceCode.c_str(), SourceCode.size(), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shadermodel, 0, 0, &blob.DXBC_SourceCode, &ERRMSG)))
				{
					Log->Info("[ShaderCompiler] Compiling Error -- \nInfo: ");
					CheckShaderErrors(ERRMSG);

					return blob;
				}

				blob.Language = ShaderLanguage::DXBC;

				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					// Initialize shader input descriptor structure
					Xsc::ShaderInput inputDesc;
					{
						//inputDesc.sourceCode = SourceCode;
						inputDesc.shaderVersion = Xsc::InputShaderVersion::HLSL4;
						inputDesc.entryPoint = "main";
						inputDesc.shaderTarget = Xsc::ShaderTarget::VertexShader;
					}

					// Initialize shader output descriptor structure
					Xsc::ShaderOutput outputDesc;
					{
						//outputDesc.sourceCode = &outputStream;
						outputDesc.shaderVersion = Xsc::OutputShaderVersion::GLSL330;
					}

					// Compile HLSL code into GLSL
					Xsc::StdLog log;
					bool result = Xsc::CompileShader(inputDesc, outputDesc, &log);

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