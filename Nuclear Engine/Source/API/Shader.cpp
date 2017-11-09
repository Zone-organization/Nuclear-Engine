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

		Shader::Shader(const char *name, BinaryShaderBlob* VertexShader, BinaryShaderBlob* PixelShader, BinaryShaderBlob* GeometryShader)
		{
			Log->Info("[Shader] Initializing Shader: ");
			Log->Info(name);
			Log->Info("\n");

			shader = Core::Context::ConstructShader(shader);

			shader->Create(VertexShader, PixelShader, GeometryShader);
		}

		Shader::Shader(const char * name, const char * VertexShader, const char * PixelShader, const char * GeometryShader, ShaderLanguage language)
		{
			Log->Info("[Shader] Initializing Shader: ");
			Log->Info(name);
			Log->Info("\n");
						
			ID3DBlob* vhlslblob;
			ID3DBlob* phlslblob;
			ID3DBlob* ghlslblob;

			if (language == ShaderLanguage::HLSL)
			{			
				if (VertexShader != nullptr)
				{
					ID3D10Blob* ERRMSG = nullptr;

					if (FAILED(D3DCompile(VertexShader, lstrlenA(VertexShader) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_1", 0, 0, &vhlslblob, &ERRMSG)))
					{
						Log->Info("[Shader] Compiling Error In Vertex Shader-- \nInfo: ");
						CheckShaderErrors(ERRMSG);
					}
					vblob.hlslsourcecode = vhlslblob->GetBufferPointer();
					vblob.hlslsize = vhlslblob->GetBufferSize();
					vblob.Language = ShaderLanguage::DXBC;

					
					if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
					{
						GlExtensions extensions;
						GLSLCrossDependencyData deps;
						extensions.ARB_shading_language_420pack = false;
						HLSLccSamplerPrecisionInfo samplerPrecisions;
						HLSLccReflection reflectionCallbacks;
						GLSLShader glshader;

						TranslateHLSLFromMem((const char*)vhlslblob->GetBufferPointer(), HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT,
							LANG_330, &extensions, &deps, samplerPrecisions, reflectionCallbacks, &glshader);

						vblob.glslsourcecode = glshader.sourceCode;
						vblob.Language = ShaderLanguage::GLSL;
					}
				}
				if (PixelShader != nullptr)
				{
					ID3D10Blob *ERRMSG = nullptr;

					if (FAILED(D3DCompile(PixelShader, lstrlenA(PixelShader) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_1", 0, 0, &phlslblob, &ERRMSG)))
					{
						Log->Info("[Shader] Compiling Error In Pixel Shader-- \nInfo: ");
						CheckShaderErrors(ERRMSG);
					}
					pblob.hlslsourcecode = phlslblob->GetBufferPointer();
					pblob.hlslsize = phlslblob->GetBufferSize();
					pblob.Language = ShaderLanguage::DXBC;

					if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
					{
						GlExtensions _extensions;
						GLSLCrossDependencyData _deps;
						_extensions.ARB_shading_language_420pack = false;
						HLSLccSamplerPrecisionInfo _samplerPrecisions;
						HLSLccReflection _reflectionCallbacks;
						GLSLShader _glshader;


						TranslateHLSLFromMem((const char*)phlslblob->GetBufferPointer(), HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT,
							LANG_330, &_extensions, &_deps, _samplerPrecisions, _reflectionCallbacks, &_glshader);
						pblob.glslsourcecode = _glshader.sourceCode;
						pblob.Language = ShaderLanguage::GLSL;
					}
				}
				
				if (GeometryShader != nullptr)
				{
					ID3D10Blob* ERRMSG = nullptr;

					if (FAILED(D3DCompile(GeometryShader, lstrlenA(GeometryShader) + 1, 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_4_1", 0, 0, &ghlslblob, &ERRMSG)))
					{
						Log->Info("[Shader] Compiling Error In Geometry Shader-- \nInfo: ");
						CheckShaderErrors(ERRMSG);
					}
					gblob.hlslsourcecode = ghlslblob->GetBufferPointer();
					gblob.hlslsize = ghlslblob->GetBufferSize();
					gblob.Language = ShaderLanguage::DXBC;

					if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
					{
						GlExtensions extensions;
						GLSLCrossDependencyData deps;
						extensions.ARB_shading_language_420pack = false;
						HLSLccSamplerPrecisionInfo samplerPrecisions;
						HLSLccReflection reflectionCallbacks;
						GLSLShader glshader;

						TranslateHLSLFromMem((const char*)ghlslblob->GetBufferPointer(), HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT,
							LANG_330, &extensions, &deps, samplerPrecisions, reflectionCallbacks, &glshader);

						gblob.glslsourcecode = glshader.sourceCode;
						gblob.Language = ShaderLanguage::GLSL;
					}
				}
			}
			
			shader = Core::Context::ConstructShader(shader);

			shader->Create(&vblob, &pblob, nullptr);
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

		BinaryShaderBlob* CompileShader(const char * SourceCode, ShaderType type, ShaderLanguage language)
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

					//return blob;
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
			return &blob;
		}

	}
}