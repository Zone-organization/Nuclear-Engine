#define NOMINMAX

#include <API\ShaderCompiler.h>
#include <API\Shader_Types.h>
#include <Core\Context.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <XShaderCompiler\inc\Xsc\Xsc.h>
#include <sstream>
#include <iostream>
#pragma comment(lib,"XShaderCompiler.lib")
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

		BinaryShaderBlob CompileShader(std::string SourceCode,API::ShaderType type,API::ShaderLanguage language)
		{
			BinaryShaderBlob blob;

			const char* shadermodel;
			if (type ==API::ShaderType::Vertex)
			{
				shadermodel = "vs_4_1";
			}
			else if (type ==API::ShaderType::Pixel)
			{
				shadermodel = "ps_4_1";
			}
			else if (type ==API::ShaderType::Geometry)
			{
				shadermodel = "gs_4_1";
			}

			if (language ==API::ShaderLanguage::HLSL)
			{
				ID3D10Blob* ERRMSG = nullptr;

				if (FAILED(D3DCompile(SourceCode.c_str(), SourceCode.size(), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shadermodel, 0, 0, &blob.DXBC_SourceCode, &ERRMSG)))
				{
					Log->Info("[ShaderCompiler] Compiling Error -- \nInfo: ");
					CheckShaderErrors(ERRMSG);

					return blob;
				}

				blob.Language =API::ShaderLanguage::DXBC;

				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{

					// Initialize shader input descriptor structure
					auto input = std::make_shared<std::stringstream>();

					*input << SourceCode;

					Xsc::ShaderInput inputDesc;
					inputDesc.sourceCode = input;
					inputDesc.shaderVersion = Xsc::InputShaderVersion::HLSL4;
					inputDesc.entryPoint = "main";

					switch (type)
					{
					case ShaderType::Vertex:
						inputDesc.shaderTarget = Xsc::ShaderTarget::VertexShader;
						break;
					case ShaderType::Geometry:
						inputDesc.shaderTarget = Xsc::ShaderTarget::GeometryShader;
						break;				
					case ShaderType::Pixel:
						inputDesc.shaderTarget = Xsc::ShaderTarget::FragmentShader;
						break;					
					default:
						break;
					}

					// Initialize shader output descriptor structure
					Xsc::ShaderOutput outputDesc;
					std::ostringstream stream;
					outputDesc.sourceCode = &stream;
					outputDesc.shaderVersion = Xsc::OutputShaderVersion::GLSL330;					

					// Compile HLSL code into GLSL
					Xsc::StdLog log;
					bool result = Xsc::CompileShader(inputDesc, outputDesc, &log);

					if (result == true)
					{
						blob.GLSL_SourceCode = stream.str();
					}

					blob.Language = API::ShaderLanguage::GLSL;
				}
			}
			else if (language ==API::ShaderLanguage::GLSL)
			{
				Exceptions::NotImplementedException();
			}
			return blob;
		}

	}
}