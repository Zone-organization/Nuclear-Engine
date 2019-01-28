#include "Engine\Graphics\ShaderReflector.h"
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"dxguid.lib")

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			void Check_D3DCompile_Errors(ID3D10Blob* Shader)
			{
				char* compileErrors;

				// Get a pointer to the error message text Buffer.
				compileErrors = (char*)(Shader->GetBufferPointer());

				Log.Error("[D3DCompiler]" + std::string(compileErrors));

				// Release the error message.
				Shader->Release();
				Shader = 0;

				return;
			}

			void ParseConstantBuffers(const D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* pReflector, std::vector<std::string> *result)
			{
				for (Uint32 i = 0; i < shaderDesc.ConstantBuffers; i++)
				{
					ID3D11ShaderReflectionConstantBuffer* reflectedcb = nullptr;
					reflectedcb = pReflector->GetConstantBufferByIndex(i);

					D3D11_SHADER_BUFFER_DESC CBDesc;
					if (FAILED(reflectedcb->GetDesc(&CBDesc)))
					{
						Log.Error("[Reflect_DXBC] [ParseConstantBuffers] Cannot get constant buffer desc with index: " + std::to_string(i) + "\n");
					}
					else
					{

						result->push_back(CBDesc.Name);
					}
				}
			}
			void ParseResources(const D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* pReflector, std::vector<std::string> *result)
			{
				for (UINT32 i = 0; i < shaderDesc.BoundResources; i++)
				{
					D3D11_SHADER_INPUT_BIND_DESC resource_desc;
					if (FAILED(pReflector->GetResourceBindingDesc(i, &resource_desc)))
					{
						Log.Error("[Reflect_DXBC] [ParseResources] Cannot get resource binding desc with index: " + std::to_string(i) + "\n");
					}
					else
					{
						for (UINT32 i = 0; i < resource_desc.BindCount; i++)
						{
							if (resource_desc.Type == D3D_SIT_TEXTURE)
								result->push_back(resource_desc.Name);
						}
					}
				}
			}

			std::vector<NuclearShaderVariableDesc> Reflect_DXBC(ID3D10Blob* blob)
			{
				std::vector<NuclearShaderVariableDesc> desc;
				ID3D11ShaderReflection* pReflector = NULL;

				D3DReflect(blob->GetBufferPointer(),
					blob->GetBufferSize(),
					IID_ID3D11ShaderReflection, (void**)&pReflector);

				D3D11_SHADER_DESC shaderDesc;
				pReflector->GetDesc(&shaderDesc);

				std::vector<std::string> constantbuffers;
				std::vector<std::string> textures;

				ParseResources(shaderDesc, pReflector, &textures);
				ParseConstantBuffers(shaderDesc, pReflector, &constantbuffers);

				for (auto name : textures)
				{
					NuclearShaderVariableDesc vardesc(name);
					if (name.find("NEStatic_") == 0)
					{
						vardesc.Type = SHADER_VARIABLE_TYPE_STATIC;
						desc.push_back(vardesc);
					}
					else if (name.find("NEDynamic_") == 0)
					{
						vardesc.Type = SHADER_VARIABLE_TYPE_DYNAMIC;
						desc.push_back(vardesc);
					}
					else if (name.find("NEMutable_") == 0 || name.find("NEMat_") == 0)
					{
						vardesc.Type = SHADER_VARIABLE_TYPE_MUTABLE;
						desc.push_back(vardesc);
					}
					else {
						Log.Error("[ShaderReflector] Use of undefined type Texture (" + name + ")\n");
					}
				}

				for (auto name : constantbuffers)
				{
					NuclearShaderVariableDesc vardesc(name);
					if (name.find("NEStatic_") == 0)
					{
						vardesc.Type = SHADER_VARIABLE_TYPE_STATIC;
						desc.push_back(vardesc);
					}
					else if (name.find("NEDynamic_") == 0)
					{
						vardesc.Type = SHADER_VARIABLE_TYPE_DYNAMIC;
						desc.push_back(vardesc);
					}
					else if (name.find("NEMutable_") == 0 || name.find("NEMat_") == 0)
					{
						vardesc.Type = SHADER_VARIABLE_TYPE_MUTABLE;
						desc.push_back(vardesc);
					}
					else {
						Log.Error("[ShaderReflector] Use of undefined type Constantbuffer (" + name + ")\n");
					}
				}
				return desc;
			}

			std::vector<NuclearShaderVariableDesc> ReflectHLSL(const std::string & SourceCode, SHADER_TYPE type)
			{
				const char* shadermodel;
				if (type == SHADER_TYPE_VERTEX)
				{
					shadermodel = "vs_4_1";
				}
				else if (type == SHADER_TYPE_PIXEL)
				{
					shadermodel = "ps_4_1";
				}
				else if (type == SHADER_TYPE_GEOMETRY)
				{
					shadermodel = "gs_4_1";
				}

				ID3D10Blob* ERRMSG = nullptr;
				ID3D10Blob* m_blob;

				if (FAILED(D3DCompile(SourceCode.c_str(), SourceCode.length(), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shadermodel, 0, 0, &m_blob, &ERRMSG)))
				{
					Log.Error("[ShaderCompiler] CompileHLSL2DXBC Failed\n");

					Check_D3DCompile_Errors(ERRMSG);

					return std::vector<NuclearShaderVariableDesc>();
				}

				auto result = Reflect_DXBC(m_blob);
				m_blob->Release();

				return result;
			}
		}
	}
}