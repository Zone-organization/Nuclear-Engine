#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/include/BasicShaderSourceStreamFactory.h>

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
		IShader * ShaderManager::CreateShader(std::string source, SHADER_TYPE type)
		{
			ShaderCreationAttribs CreationAttribs;
			IShader* result;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
			CreationAttribs.Desc.ShaderType = type;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Source = source.c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &result);

			return result;
		}
		IShader* ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc)
		{
			ShaderCreationAttribs CreationAttribs;
			IShader* pVS;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Desc.Name = "AutoVertexShader";
			
			std::vector<std::string> defines;

			if (desc.InTexCoords)
				defines.push_back("NE_USE_UV");

			if (desc.InNormals)
				defines.push_back("NE_USE_NORMALS");

			if (desc.InTangents)
				defines.push_back("NE_USE_TANGENTS");

			if (desc.Use_Camera)
				defines.push_back("NE_USE_DEF_CAMERA");

			if (desc.OutFragPos)
				defines.push_back("NE_OUT_FRAG_POS");


			CreationAttribs.Source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true).c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pVS);
			
			return pVS;
		}
		IShader* ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			ShaderCreationAttribs CreationAttribs;
			IShader* pPS;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Desc.Name = "AutoPixelShader";

			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");


			CreationAttribs.Source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true).c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pPS);

			return pPS;
		}
	}
}