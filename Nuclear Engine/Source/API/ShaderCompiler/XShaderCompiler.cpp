#include "XShaderCompiler.h"

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <XShaderCompiler\inc\Xsc\Xsc.h>
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
			void ParseConstantBuffers(const std::vector<Xsc::Reflection::BindingSlot>& cbuffers, BinaryShaderBlob * result)
			{
				for (Xsc::Reflection::BindingSlot cb : cbuffers)
				{
					Reflected_Constantbuffer refl_cb;
					refl_cb.BindingSlot = cb.location;

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


			void CompileHLSL2GLSL(BinaryShaderBlob * result, std::string SourceCode, API::ShaderType type, API::ShaderLanguage language)
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

				//XSC doesn't play nicely with separate shaders extension so we have to include it ourselves			
				std::vector<std::string> MergedCode;
				std::string firstLine = result->GLSL_SourceCode.substr(85, result->GLSL_SourceCode.find("\n"));

				MergedCode.push_back(std::string("#version 330\n #extension GL_ARB_separate_shader_objects : enable\n"));

				MergedCode.push_back(result->GLSL_SourceCode.substr(99));
				std::string str;
				for (unsigned int i = 0; i < MergedCode.size(); ++i)
					str = str + MergedCode[i].c_str();

				result->GLSL_SourceCode = str;

				result->Language = API::ShaderLanguage::GLSL;
				result->DXBC_SourceCode = DXBC_BLOB();
				result->Converted = true;
			}
		}
	}
}
#endif