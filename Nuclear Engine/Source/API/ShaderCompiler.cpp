#include <API\ShaderCompiler.h>
#include <API\API_Types.h>
#include <Core\Context.h>

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