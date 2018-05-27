#include "DXBC_Compiler.h"

#ifdef NE_COMPILE_D3DCOMPILER
#include <d3dcompiler.h>
#pragma comment(lib,"D3DCompiler.lib")

namespace NuclearEngine
{
	namespace Graphics {

		namespace API
		{
			namespace DXBC_Compiler
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


				void ParseVariables(const D3D11_SHADER_BUFFER_DESC& CBDesc, ID3D11ShaderReflectionConstantBuffer* reflectedcb, Reflected_Constantbuffer* Constbuf)
				{
					for (UINT32 i = 0; i < CBDesc.Variables; i++)
					{
						ID3D11ShaderReflectionVariable* VariableReflection = reflectedcb->GetVariableByIndex(i);
						D3D11_SHADER_VARIABLE_DESC VariableDesc;

						if (FAILED(VariableReflection->GetDesc(&VariableDesc)))
						{
							Log.Error("[Reflect_DXBC] [ParseConstantBuffers] [ParseVariables] Cannot get constant buffer variable desc with index: " + std::to_string(i) + "\n");
						}
						else
						{
							ID3D11ShaderReflectionType* VarType = VariableReflection->GetType();
							D3D11_SHADER_TYPE_DESC VarTypeDesc;
							VarType->GetDesc(&VarTypeDesc);
							ShaderVariableType type = ShaderVariableType::Unknown;

							switch (VarTypeDesc.Class)
							{
							case D3D_SVC_SCALAR:
							{
								switch (VarTypeDesc.Type)
								{
								case D3D_SVT_BOOL:
									type = ShaderVariableType::Bool;
									break;
								case D3D_SVT_INT:
								case D3D_SVT_UINT:
									type = ShaderVariableType::Int1;
									break;
								case D3D_SVT_FLOAT:
									type = ShaderVariableType::Float1;
									break;
								default:
									Log.Warning("[Reflect_DXBC] [ParseConstantBuffers] [ParseVariables] Using scalar variable with unsupported type:  " + std::to_string(VarTypeDesc.Type) + " \n");
								}
							}
							break;
							case D3D_SVC_VECTOR:
							{
								switch (VarTypeDesc.Type)
								{
								case D3D_SVT_UINT:
								case D3D_SVT_INT:
								{
									switch (VarTypeDesc.Columns)
									{
									case 1:
										type = ShaderVariableType::Int1;
										break;
									case 2:
										type = ShaderVariableType::Int2;
										break;
									case 3:
										type = ShaderVariableType::Int3;
										break;
									case 4:
										type = ShaderVariableType::Int4;
										break;
									}
								}

								break;
								case D3D_SVT_FLOAT:
								{
									switch (VarTypeDesc.Columns)
									{
									case 1:
										type = ShaderVariableType::Float1;
										break;
									case 2:
										type = ShaderVariableType::Float2;
										break;
									case 3:
										type = ShaderVariableType::Float3;
										break;
									case 4:
										type = ShaderVariableType::Float4;
										break;
									}
								}

								break;
								}
							}
							break;
							case D3D_SVC_MATRIX_COLUMNS:
							case D3D_SVC_MATRIX_ROWS:
								switch (VarTypeDesc.Rows)
								{
								case 2:
									switch (VarTypeDesc.Columns)
									{
									case 2:
										type = ShaderVariableType::Matrix2x2;
										break;
									}
									break;
								case 3:
									switch (VarTypeDesc.Columns)
									{
									case 3:
										type = ShaderVariableType::Matrix3x3;
										break;
									}
									break;
								case 4:
									switch (VarTypeDesc.Columns)
									{
									case 4:
										type = ShaderVariableType::Matrix4x4;
										break;
									}
									break;
								}
								break;
							case D3D_SVC_STRUCT:
								type = ShaderVariableType::Struct;
								break;
							default:
								Log.Warning("[Reflect_DXBC] [ParseConstantBuffers] [ParseVariables] Using variable with unsupported class: " + std::to_string(VarTypeDesc.Class) + " type: " + std::to_string(VarTypeDesc.Type) + " \n");
							}

							ShaderVariable reflected_var;
							reflected_var.Type = type;
							reflected_var.Size = VariableDesc.Size;
							Constbuf->Variables[VariableDesc.Name] = reflected_var;

						}
					}

				}
				void ParseConstantBuffers(const D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* pReflector, BinaryShaderBlob *result)
				{
					for (Uint32 i = 0; i < shaderDesc.ConstantBuffers; i++)
					{
						ID3D11ShaderReflectionConstantBuffer* reflectedcb = nullptr;
						reflectedcb = pReflector->GetConstantBufferByIndex(i);

						Reflected_Constantbuffer Constbuf;
						D3D11_SHADER_BUFFER_DESC CBDesc;
						if (FAILED(reflectedcb->GetDesc(&CBDesc)))
						{
							Log.Error("[Reflect_DXBC] [ParseConstantBuffers] Cannot get constant buffer desc with index: " + std::to_string(i) + "\n");
						}
						else
						{
							Constbuf.BindingSlot = i;
							Constbuf.Size = CBDesc.Size;
							ParseVariables(CBDesc, reflectedcb, &Constbuf);

							result->Reflection.ConstantBuffers[CBDesc.Name] = Constbuf;
						}
					}
				}
				void ParseResources(const D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* pReflector, BinaryShaderBlob *result)
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
								switch (resource_desc.Type)
								{
								case D3D_SIT_TEXTURE:
								{
									bool validtexture = true;
									Reflected_Texture texture;
									texture.BindingSlot = i;
									switch (resource_desc.Dimension)
									{
									case D3D_SRV_DIMENSION_TEXTURE1D:
										texture.Type = Graphics::API::TextureType::Texture1D;
										break;
									case D3D_SRV_DIMENSION_TEXTURE2D:
										texture.Type = Graphics::API::TextureType::Texture2D;
										break;
									case D3D_SRV_DIMENSION_TEXTURE3D:
										texture.Type = Graphics::API::TextureType::Texture3D;
										break;
									case D3D_SRV_DIMENSION_TEXTURECUBE:
										texture.Type = Graphics::API::TextureType::TextureCube;
										break;
									default:
										Log.Warning("[Reflect_DXBC] [ParseResources] Unsupported Texture type used, therefore not added to reflection!\n");
										validtexture = false;
									}
									if (validtexture == true)
									{
										result->Reflection.Textures[resource_desc.Name] = texture;
									}

								}

								}

							}
						}

					}
				}
				void Reflect_DXBC(BinaryShaderBlob *result)
				{
					ID3D11ShaderReflection* pReflector = NULL;

					D3DReflect(result->DXBC_SourceCode.Buffer,
						result->DXBC_SourceCode.Size,
						IID_ID3D11ShaderReflection, (void**)&pReflector);

					D3D11_SHADER_DESC shaderDesc;
					pReflector->GetDesc(&shaderDesc);

					ParseResources(shaderDesc, pReflector, result);
					ParseConstantBuffers(shaderDesc, pReflector, result);

					return;
				}
				void CompileHLSL2DXBC(BinaryShaderBlob * result, std::string SourceCode, Graphics::API::ShaderType type)
				{
					const char* shadermodel;
					if (type == Graphics::API::ShaderType::Vertex)
					{
						shadermodel = "vs_4_1";
					}
					else if (type == Graphics::API::ShaderType::Pixel)
					{
						shadermodel = "ps_4_1";
					}
					else if (type == Graphics::API::ShaderType::Geometry)
					{
						shadermodel = "gs_4_1";
					}

					ID3D10Blob* ERRMSG = nullptr;
					ID3D10Blob* m_blob;

					if (FAILED(D3DCompile(SourceCode.c_str(), SourceCode.length(), 0, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", shadermodel, 0, 0, &m_blob, &ERRMSG)))
					{
						Log.Error("[ShaderCompiler] CompileHLSL2DXBC Failed\n");

						Check_D3DCompile_Errors(ERRMSG);

						return;
					}

					result->DXBC_SourceCode.Buffer = m_blob->GetBufferPointer();
					result->DXBC_SourceCode.Size = m_blob->GetBufferSize();
					Reflect_DXBC(result);

					return;
				}
			}
		}
	}
}
#endif