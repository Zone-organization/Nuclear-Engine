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

			this->vblob = VertexShader;
			this->pblob = PixelShader;
			this->gblob = GeometryShader;


			shader = Core::Context::ConstructShader(shader);

			shader->Create(VertexShader, PixelShader, GeometryShader);
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

					for (unsigned int i = 0; i < glshader.reflection.psResourceBindings.size(); i++)
					{
						if (glshader.reflection.psResourceBindings.at(i).eType == RTYPE_TEXTURE)
						{
							ReflectedTextureDesc tex;
							tex.Name = glshader.reflection.psResourceBindings.at(i).name.c_str();

							switch (glshader.reflection.psResourceBindings.at(i).eDimension)
							{
							case REFLECT_RESOURCE_DIMENSION_TEXTURE1D:
								tex.type = ReflectedTextureType::Texture1D;
								break;
							case REFLECT_RESOURCE_DIMENSION_TEXTURE2D:
								tex.type = ReflectedTextureType::Texture2D;
								break;
							case REFLECT_RESOURCE_DIMENSION_TEXTURE3D:
								tex.type = ReflectedTextureType::Texture3D;
								break;
							case REFLECT_RESOURCE_DIMENSION_TEXTURECUBE:
								tex.type = ReflectedTextureType::TextureCube;
								break;
							}

							blob.reflection.textures.push_back(tex);
						}
					}

					blob.Language = ShaderLanguage::GLSL;
				}
				else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					// We are using HLSLCC as reflection engine as we can't get texture names by ourselves through Dx reflection
					GlExtensions extensions;
					GLSLCrossDependencyData deps;
					extensions.ARB_shading_language_420pack = false;
					HLSLccSamplerPrecisionInfo samplerPrecisions;
					HLSLccReflection reflectionCallbacks;
					GLSLShader glshader;
					
					TranslateHLSLFromMem((const char*)blob.hlslsourcecode, HLSLCC_FLAG_UNIFORM_BUFFER_OBJECT,
						LANG_330, &extensions, &deps, samplerPrecisions, reflectionCallbacks, &glshader);

					for (unsigned int i = 0; i < glshader.reflection.psResourceBindings.size(); i++)
					{

						if (glshader.reflection.psResourceBindings.at(i).eType == RTYPE_TEXTURE)
						{
							ReflectedTextureDesc tex;
							tex.Name = glshader.reflection.psResourceBindings.at(i).name.c_str();

							switch (glshader.reflection.psResourceBindings.at(i).eDimension)
							{
							case REFLECT_RESOURCE_DIMENSION_TEXTURE1D:
								tex.type = ReflectedTextureType::Texture1D;
								break;
							case REFLECT_RESOURCE_DIMENSION_TEXTURE2D:
								tex.type = ReflectedTextureType::Texture2D;
								break;
							case REFLECT_RESOURCE_DIMENSION_TEXTURE3D:
								tex.type = ReflectedTextureType::Texture3D;
								break;
							case REFLECT_RESOURCE_DIMENSION_TEXTURECUBE:
								tex.type = ReflectedTextureType::TextureCube;
								break;
							}
			
							blob.reflection.textures.push_back(tex);
						}
					}
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