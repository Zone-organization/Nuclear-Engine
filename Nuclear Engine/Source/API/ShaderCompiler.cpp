#define NOMINMAX

#include <API\ShaderCompiler.h>
#include <API\Shader_Types.h>
#include <Core\Context.h>

#ifdef NE_COMPILE_XSHADERCOMPILER
#include <XShaderCompiler\inc\Xsc\Xsc.h>
#include <API\OpenGL\GLCommon.h>
#include <sstream>
#include <iostream>
#include <vector>
#pragma comment(lib,"XShaderCompiler.lib")
#endif

#ifdef NE_COMPILE_D3DCOMPILER
#include <d3dcompiler.h>
#pragma comment(lib,"D3DCompiler.lib")
#endif

namespace NuclearEngine {

	namespace API {

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
			void Get_Whole_Log()
			{
				for (uint i = 0; i < Infos.size(); i++)
				{
					NuclearEngine::Log.Info("[XShaderCompiler] " + Infos.at(i).Message() + '\n');
				}
				for (uint i = 0; i < Warnings.size(); i++)
				{
					NuclearEngine::Log.Warning("[XShaderCompiler] " + Warnings.at(i).Message() + '\n');
				}
				for (uint i = 0; i < Errors.size(); i++)
				{
					NuclearEngine::Log.Error("[XShaderCompiler] " + Errors.at(i).Message() + '\n');
				}
			}

		private:						
			std::vector<Xsc::Report> Infos;
			std::vector<Xsc::Report> Warnings;
			std::vector<Xsc::Report> Errors;
		};

		void Reflect_DXBC(BinaryShaderBlob *result)
		{
			ID3D11ShaderReflection* pReflector = NULL;
			
			D3DReflect(result->DXBC_SourceCode.Buffer,
				result->DXBC_SourceCode.Size,
				IID_ID3D11ShaderReflection, (void**)&pReflector);

			D3D11_SHADER_DESC shaderDesc;
			pReflector->GetDesc(&shaderDesc);


			for (uint i = 0; i < shaderDesc.ConstantBuffers; i++)
			{
				ID3D11ShaderReflectionConstantBuffer* reflectedcb = nullptr;
				reflectedcb = pReflector->GetConstantBufferByIndex(i);

				Reflected_Constantbuffer Constbuf;
				D3D11_SHADER_BUFFER_DESC Desc;
				reflectedcb->GetDesc(&Desc);
				Constbuf.BindingSlot = i;
				Constbuf.Size = Desc.Size;
				result->Reflection.ConstantBuffers[Desc.Name] = Constbuf;
			}

			return;
		}

		void CompileHLSL2DXBC(BinaryShaderBlob *result, std::string SourceCode, API::ShaderType type, API::ShaderLanguage language, bool reflect_p)
		{
			const char* shadermodel;
			if (type == API::ShaderType::Vertex)
			{
				shadermodel = "vs_4_1";
			}
			else if (type == API::ShaderType::Pixel)
			{
				shadermodel = "ps_4_1";
			}
			else if (type == API::ShaderType::Geometry)
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
			result->Language = API::ShaderLanguage::DXBC;
			
		/*	if (reflect_p == true)
			{
				Reflect_DXBC(result);
			}	*/		
			return;
		}
		void CompileDXBC2GLSL(BinaryShaderBlob *result, std::string SourceCode, API::ShaderType type, API::ShaderLanguage language, bool reflect_p)
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
			XSC_ERROR_LOG log;

			if (!Xsc::CompileShader(inputDesc, outputDesc, &log))
			{
				Log.Error("[ShaderCompiler] CompileDXBC2GLSL Failed!\n");
			}
			log.Get_Whole_Log();

			//std::cout << stream.str();
			result->GLSL_SourceCode = stream.str();
			result->Language = API::ShaderLanguage::GLSL;
			result->DXBC_SourceCode = DXBC_BLOB();
			result->Converted = true;
		}

		bool CompileShader(BinaryShaderBlob* blob, std::string SourceCode,API::ShaderType type,API::ShaderLanguage language, bool reflect_p)
		{
			if (language == API::ShaderLanguage::HLSL)
			{
				CompileHLSL2DXBC(blob, SourceCode, type, language, reflect_p);

				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					CompileDXBC2GLSL(blob, SourceCode, type, language, reflect_p);
				}
			}
			else if (language == API::ShaderLanguage::GLSL)
			{
				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					blob->GLSL_SourceCode = SourceCode;
					blob->Language = API::ShaderLanguage::GLSL;
				}
				else 
				{
					Exceptions::NotImplementedException();
				}
			}
			blob->Finished = true;
			return true;
		}

	}
}