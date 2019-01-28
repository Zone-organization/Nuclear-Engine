#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/include/BasicShaderSourceStreamFactory.h>
#include <Engine\Graphics\ShaderReflector.h>

namespace NuclearEngine
{
	namespace Managers
	{
		ShaderManager::ShaderManager()
		{
		}
		ShaderManager::~ShaderManager()
		{
		}
		IShader * ShaderManager::CreateShader(const std::string& source, SHADER_TYPE type, SHADER_VARIABLE_TYPE DefaultVariableType)
		{
			ShaderCreationAttribs CreationAttribs;
			IShader* result;
			auto shaderreflection = Graphics::API::ReflectHLSL(source, type);
			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
			CreationAttribs.Desc.ShaderType = type;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Source = source.c_str();
			std::vector<ShaderVariableDesc> _ref;

			for (auto I : shaderreflection)
			{
				auto iii = I.Name.c_str();
				_ref.push_back({ iii, I.Type });
			}
			CreationAttribs.Desc.VariableDesc = _ref.data();
			CreationAttribs.Desc.NumVariables = shaderreflection.size();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &result);

			return result;
		}
		IShader* ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc, std::vector<LayoutElement>* Layout)
		{
			ShaderCreationAttribs CreationAttribs;
			IShader* pVS = nullptr;
			Layout->clear();

			Layout->push_back(LayoutElement(0, 0, 3, VT_FLOAT32, False));

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Desc.Name = "AutoVertexShader";
			

			std::vector<std::string> defines;

			if (desc.InTexCoords)
			{
				defines.push_back("NE_USE_UV");
				Layout->push_back(LayoutElement(1, 0, 2, VT_FLOAT32, False));
			}
			if (desc.InNormals)
			{
				defines.push_back("NE_USE_NORMALS");
				Layout->push_back(LayoutElement(2, 0, 3, VT_FLOAT32, False));
			}
			if (desc.InTangents)
			{
				defines.push_back("NE_USE_TANGENTS");
				Layout->push_back(LayoutElement(3, 0, 3, VT_FLOAT32, False));
			}
			if (desc.Use_Camera)
				defines.push_back("NE_USE_DEF_CAMERA");

			if (desc.OutFragPos)
				defines.push_back("NE_OUT_FRAG_POS");

			auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true);
			CreationAttribs.Source = source.c_str();
			auto shaderreflection = Graphics::API::ReflectHLSL(source, SHADER_TYPE_VERTEX);
			std::vector<ShaderVariableDesc> _ref;

			for (auto I : shaderreflection)
			{
				auto iii = I.Name.c_str();
				_ref.push_back({ iii, I.Type });
			}
			CreationAttribs.Desc.VariableDesc = _ref.data();
			CreationAttribs.Desc.NumVariables = shaderreflection.size();
			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pVS);
			
			return pVS;
		}
		IShader* ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			ShaderCreationAttribs CreationAttribs;
			IShader* pPS = nullptr;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Desc.Name = "AutoPixelShader";

			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");

			auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true);
			CreationAttribs.Source = source.c_str();
			auto shaderreflection = Graphics::API::ReflectHLSL(source, SHADER_TYPE_PIXEL);
			std::vector<ShaderVariableDesc> _ref;

			for (auto I : shaderreflection)
			{
				auto iii = I.Name.c_str();
				_ref.push_back({ iii, I.Type });
			}
			CreationAttribs.Desc.VariableDesc = _ref.data();
			CreationAttribs.Desc.NumVariables = shaderreflection.size();
			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pPS);

			return pPS;
		}
	}
}