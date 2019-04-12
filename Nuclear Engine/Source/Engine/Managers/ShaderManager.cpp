#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>

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
		IShader * ShaderManager::CreateShader(const std::string& source, SHADER_TYPE type)
		{
			ShaderCreateInfo CreationAttribs;
			IShader* result;
			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = type;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Source = source.c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &result);

			return result;
		}
		IShader* ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc, std::vector<LayoutElement>* Layout)
		{
			ShaderCreateInfo CreationAttribs;
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

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pVS);
			
			return pVS;
		}
		IShader* ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			ShaderCreateInfo CreationAttribs;
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
			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &pPS);

			return pPS;
		}

		IPipelineState * ShaderManager::CreatePipelineState(IShader * VertexShader, IShader * PixelShader)
		{
			return nullptr;
		}
		
		std::vector<ShaderResourceVariableDesc> ShaderManager::ReflectShaderVariables(IShader* VShader, IShader* PShader)
		{
			std::vector<ShaderResourceVariableDesc> resources;
			for (int i = 0; i < VShader->GetResourceCount(); i++)
			{
				ShaderResourceVariableDesc Desc;
				Desc.Name = VShader->GetResource(i).Name;
				Desc.Type = ParseNameToGetType(VShader->GetResource(i).Name);
				Desc.ShaderStages = VShader->GetDesc().ShaderType;
				resources.push_back(Desc);
			}
			for (int i = 0; i < PShader->GetResourceCount(); i++)
			{
				ShaderResourceVariableDesc Desc;
				Desc.Name = PShader->GetResource(i).Name;
				Desc.Type = ParseNameToGetType(PShader->GetResource(i).Name);
				Desc.ShaderStages = PShader->GetDesc().ShaderType;
				resources.push_back(Desc);
			}
			return resources;
		}
		SHADER_RESOURCE_VARIABLE_TYPE ShaderManager::ParseNameToGetType(const std::string & name)
		{
			if (name.find("NEStatic") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
			else if (name.find("NEMutable") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;
			else if (name.find("NEDynamic") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;

			return SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
		}
	}
}