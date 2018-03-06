#include "XShaderCompiler.h"

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <..\..\XShaderCompiler\inc\Xsc\Xsc.h>
#include <sstream>
#include <iostream>
#include <vector>
#pragma comment(lib,"XShaderCompiler.lib")

namespace NuclearEngine
{
	namespace API
	{
		namespace XShaderCompiler
		{
			ShaderVariableType Reflect_Vars(int foo)
			{
				switch (foo)
				{
				case (int)Xsc::Reflection::VarType::Bool:	return ShaderVariableType::Bool;
				case (int)Xsc::Reflection::VarType::Int:	return ShaderVariableType::Int1;
				case (int)Xsc::Reflection::VarType::Int2:	return ShaderVariableType::Int2;
				case (int)Xsc::Reflection::VarType::Int3:	return ShaderVariableType::Int3;
				case (int)Xsc::Reflection::VarType::Int4:	return ShaderVariableType::Int4;
				case (int)Xsc::Reflection::VarType::Float:	return ShaderVariableType::Float1;
				case (int)Xsc::Reflection::VarType::Float2:	return ShaderVariableType::Float2;
				case (int)Xsc::Reflection::VarType::Float3:	return ShaderVariableType::Float3;
				case (int)Xsc::Reflection::VarType::Float4:	return ShaderVariableType::Float4;
				case (int)Xsc::Reflection::VarType::Float2x2:	return ShaderVariableType::Matrix2x2;
				case (int)Xsc::Reflection::VarType::Float3x3:	return ShaderVariableType::Matrix3x3;
				case (int)Xsc::Reflection::VarType::Float4x4:	return ShaderVariableType::Matrix4x4;
				default:
					Log.Warning("[XShaderCompiler] [Reflection] Using Unsupported Variable type: " + std::to_string(foo) + " \n");
					return ShaderVariableType::Unknown;
				}
			}
			void ParseConstantBuffers(const std::vector<Xsc::Reflection::ConstantBufferRefl>& cbuffers, BinaryShaderBlob * result)
			{
				for (auto cb : cbuffers)
				{
					Reflected_Constantbuffer refl_cb;
					refl_cb.BindingSlot = cb.location;

					for (auto member : cb.members)
					{
						ShaderVariable variable;
						//special case
						if (member.type == Xsc::Reflection::UniformType::Struct)
							variable.Type = ShaderVariableType::Struct;
						else
							variable.Type = Reflect_Vars(member.baseType);
						
						refl_cb.Variables[member.ident] = variable;
					}
					result->Reflection.ConstantBuffers[cb.ident] = refl_cb;
				}
			}
			void ParseTextures(const std::vector<Xsc::Reflection::BindingSlot>& textures, BinaryShaderBlob * result)
			{
				for (Xsc::Reflection::BindingSlot tex : textures)
				{
					Reflected_Texture refl_tex;
					refl_tex.BindingSlot = tex.location;

					result->Reflection.Textures[tex.ident] = refl_tex;
				}
			}
			void Reflect(Xsc::Reflection::ReflectionData* reflection, BinaryShaderBlob * result)
			{
				ParseTextures(reflection->textures, result);
				ParseConstantBuffers(reflection->constantBuffers,result);
			}

			class XSC_ERROR_LOG : public Xsc::Log
			{
			public:
				void SubmitReport(const Xsc::Report& report) override
				{
					switch (report.Type())
					{
					case Xsc::ReportTypes::Info:
						Infos.push_back(report);
						break;
					case Xsc::ReportTypes::Warning:
						Warnings.push_back(report);
						break;
					case Xsc::ReportTypes::Error:
						Errors.push_back(report);
						break;
					}
				}
				std::string Get_Whole_Log()
				{
					std::string result;
					for (uint i = 0; i < Infos.size(); i++)
					{
						result += std::string("[XShaderCompiler] " + Infos.at(i).Message() + '\n');
					}
					for (uint i = 0; i < Warnings.size(); i++)
					{
						result += std::string("[XShaderCompiler] " + Warnings.at(i).Message() + '\n');
					}
					for (uint i = 0; i < Errors.size(); i++)
					{
						result += std::string("[XShaderCompiler] " + Errors.at(i).Message() + '\n');
					}
					return result;
				}

			private:
				std::vector<Xsc::Report> Infos;
				std::vector<Xsc::Report> Warnings;
				std::vector<Xsc::Report> Errors;
			};


			void CompileHLSL2GLSL(BinaryShaderBlob * result, std::string SourceCode, API::ShaderType type)
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
				outputDesc.options.allowExtensions = true;

				Xsc::Reflection::ReflectionData reflection;
				if (result->convertedshaderrowmajor == true)
				{
					outputDesc.options.rowMajorAlignment = true;
				}

				//Seperate shaders requirements
				outputDesc.options.autoBinding = true;
				outputDesc.options.separateShaders = true;

				// Compile HLSL code into GLSL
				XSC_ERROR_LOG log;

				if (!Xsc::CompileShader(inputDesc, outputDesc, &log, &reflection))
				{
					Log.Error("[ShaderCompiler] CompileDXBC2GLSL Failed! Info: \n" + log.Get_Whole_Log());

				}
				result->GLSL_SourceCode = stream.str();
				Reflect(&reflection, result);

				result->Converted = true;
			}
		}
	}
}
#endif