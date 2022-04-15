#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Managers
	{
		ShaderManager::ShaderManager()
		{
		}
		ShaderManager::~ShaderManager()
		{
		}
		void ShaderManager::CreateShader(const std::string& source, IShader** result, SHADER_TYPE type)
		{
			ShaderCreateInfo CreationAttribs;
			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = type;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Source = source.c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, result);
		}
		void ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc& desc, IShader** shader, std::vector<LayoutElement>* Layout)
		{
			ShaderCreateInfo CreationAttribs;
			Layout->clear();


			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Desc.Name = desc.Name;


			std::vector<std::string> defines;

			Layout->push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
			Layout->push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
			Layout->push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
			Layout->push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
			Layout->push_back(LayoutElement(4, 0, 4, VT_INT32, false));    //BONE ID
			Layout->push_back(LayoutElement(5, 0, 4, VT_FLOAT32, false));  //WEIGHT

			if (desc.Use_Camera)
				defines.push_back("NE_USE_DEF_CAMERA");

			if (desc.OutFragPos)
				defines.push_back("NE_OUT_FRAG_POS");

			auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true);
			CreationAttribs.Source = source.c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, shader);
		}
		void ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc, IShader** shader)
		{
			ShaderCreateInfo CreationAttribs;

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
			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, shader);
		}

		bool ShaderManager::ProcessAndCreatePipeline(
			IPipelineState** PipelineState,
			GraphicsPipelineStateCreateInfo& PSOCreateInfo,
			const std::vector<ShaderResourceVariableDesc>& Resources,
			bool AutoCreateSamplersDesc,
			const std::vector<ImmutableSamplerDesc>& StaticSamplers)
		{
			PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = static_cast<Uint32>(Resources.size());
			PSOCreateInfo.PSODesc.ResourceLayout.Variables = Resources.data();
			std::vector<ImmutableSamplerDesc> GeneratedSamplerDesc;

			if (!AutoCreateSamplersDesc)
			{
				PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = static_cast<Uint32>(StaticSamplers.size());
				PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = StaticSamplers.data();
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
							TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP);
						ImmutableSamplerDesc desc(SHADER_TYPE_PIXEL, i.Name, SamLinearClampDesc);
						GeneratedSamplerDesc.push_back(desc);
					}
				}

				PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = static_cast<Uint32>(GeneratedSamplerDesc.size());
				PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = GeneratedSamplerDesc.data();
			}

			Graphics::Context::GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, PipelineState);

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
				ShaderResourceDesc RsrcDesc;
				VShader->GetResourceDesc(i, RsrcDesc);
				if (!CheckSampler(RsrcDesc.Name))
				{
					ShaderResourceVariableDesc Desc;
					Desc.Name = RsrcDesc.Name;
					Desc.Type = ParseNameToGetType(RsrcDesc.Name);
					Desc.ShaderStages = VShader->GetDesc().ShaderType;
					resources.push_back(Desc);
				}
			}
			for (Uint32 i = 0; i < PShader->GetResourceCount(); i++)
			{
				ShaderResourceDesc RsrcDesc;
				PShader->GetResourceDesc(i, RsrcDesc);
				if (!CheckSampler(RsrcDesc.Name))
				{
					std::string name(RsrcDesc.Name);
					ShaderResourceVariableDesc Desc;
					Desc.Name = RsrcDesc.Name;
					Desc.Type = ParseNameToGetType(RsrcDesc.Name);
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