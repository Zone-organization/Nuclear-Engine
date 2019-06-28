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
			IShader* result = nullptr;
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
			CreationAttribs.Desc.Name = desc.Name;
			

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

		bool ShaderManager::ProcessAndCreatePipeline(
			IPipelineState** PipelineState,
			PipelineStateDesc& PSODesc,
			const std::vector<ShaderResourceVariableDesc>& Resources,
			bool AutoCreateSamplersDesc,
			const std::vector<StaticSamplerDesc>& StaticSamplers)
		{
			PSODesc.ResourceLayout.NumVariables = Resources.size();
			PSODesc.ResourceLayout.Variables = Resources.data();
			std::vector<StaticSamplerDesc> GeneratedSamplerDesc;

			if (!AutoCreateSamplersDesc)
			{
				PSODesc.ResourceLayout.NumStaticSamplers = StaticSamplers.size();
				PSODesc.ResourceLayout.StaticSamplers = StaticSamplers.data();
			}
			else 
			{

				for (auto i : Resources)
				{
					std::string name(i.Name);
					//Check its type
					if (name.find("NEMat_") != std::string::npos)
					{
						SamplerDesc SamLinearClampDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
							TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP);
						StaticSamplerDesc desc(SHADER_TYPE_PIXEL, i.Name, SamLinearClampDesc);
						GeneratedSamplerDesc.push_back(desc);
					}
				}

				PSODesc.ResourceLayout.NumStaticSamplers = GeneratedSamplerDesc.size();
				PSODesc.ResourceLayout.StaticSamplers = GeneratedSamplerDesc.data();
			}


			Graphics::Context::GetDevice()->CreatePipelineState(PSODesc, PipelineState);

			return true;
		}
			
		bool CheckSampler(const std::string& name)
		{
			if (name.find("_sampler") != std::string::npos)
				return true;

			return false;
		}

		std::vector<ShaderResourceVariableDesc> ShaderManager::ReflectShaderVariables(IShader* VShader, IShader* PShader)
		{
			std::vector<ShaderResourceVariableDesc> resources;
			for (Uint32 i = 0; i < VShader->GetResourceCount(); i++)
			{
				if (!CheckSampler(VShader->GetResource(i).Name))
				{
					ShaderResourceVariableDesc Desc;
					Desc.Name = VShader->GetResource(i).Name;
					Desc.Type = ParseNameToGetType(VShader->GetResource(i).Name);
					Desc.ShaderStages = VShader->GetDesc().ShaderType;
					resources.push_back(Desc);
				}
			}
			for (Uint32 i = 0; i < PShader->GetResourceCount(); i++)
			{
				if (!CheckSampler(PShader->GetResource(i).Name))
				{
					std::string name(PShader->GetResource(i).Name);
					ShaderResourceVariableDesc Desc;
					Desc.Name = PShader->GetResource(i).Name;
					Desc.Type = ParseNameToGetType(PShader->GetResource(i).Name);
					Desc.ShaderStages = PShader->GetDesc().ShaderType;
					resources.push_back(Desc);
				}
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
			else if (name.find("NEMat") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;

			return SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
		}
	}
}