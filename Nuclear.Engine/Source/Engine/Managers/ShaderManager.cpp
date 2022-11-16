#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Core/Parsers/toml.h>
#include <iostream>
using namespace std::string_view_literals;

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

#define TEXFMTMAP(name)	else if (!str.compare(#name)) { return name; }




		//TODO : OPTIMIZE
		TEXTURE_FORMAT ParseTexFormat(const std::string_view& str, TEXTURE_FORMAT defaultvalue = TEX_FORMAT_UNKNOWN)
		{
			if (!str.compare("TEX_FORMAT_UNKNOWN")) { return TEX_FORMAT_UNKNOWN; }
				TEXFMTMAP(TEX_FORMAT_RGBA32_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RGBA32_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RGBA32_FLOAT)
				TEXFMTMAP(TEX_FORMAT_RGBA32_UINT)
				TEXFMTMAP(TEX_FORMAT_RGBA32_SINT)
				TEXFMTMAP(TEX_FORMAT_RGB32_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RGB32_FLOAT)
				TEXFMTMAP(TEX_FORMAT_RGB32_UINT)
				TEXFMTMAP(TEX_FORMAT_RGB32_SINT)
				TEXFMTMAP(TEX_FORMAT_RGBA16_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RGBA16_FLOAT)
				TEXFMTMAP(TEX_FORMAT_RGBA16_UNORM)
				TEXFMTMAP(TEX_FORMAT_RGBA16_UINT)
				TEXFMTMAP(TEX_FORMAT_RGBA16_SNORM)
				TEXFMTMAP(TEX_FORMAT_RGBA16_SINT)
				TEXFMTMAP(TEX_FORMAT_RG32_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RG32_FLOAT)
				TEXFMTMAP(TEX_FORMAT_RG32_UINT)
				TEXFMTMAP(TEX_FORMAT_RG32_SINT)
				TEXFMTMAP(TEX_FORMAT_R32G8X24_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_D32_FLOAT_S8X24_UINT)
				TEXFMTMAP(TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_X32_TYPELESS_G8X24_UINT)
				TEXFMTMAP(TEX_FORMAT_RGB10A2_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RGB10A2_UNORM)
				TEXFMTMAP(TEX_FORMAT_RGB10A2_UINT)
				TEXFMTMAP(TEX_FORMAT_R11G11B10_FLOAT)
				TEXFMTMAP(TEX_FORMAT_RGBA8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RGBA8_UNORM)
				TEXFMTMAP(TEX_FORMAT_RGBA8_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_RGBA8_UINT)
				TEXFMTMAP(TEX_FORMAT_RGBA8_SNORM)
				TEXFMTMAP(TEX_FORMAT_RGBA8_SINT)
				TEXFMTMAP(TEX_FORMAT_RG16_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RG16_FLOAT)
				TEXFMTMAP(TEX_FORMAT_RG16_UNORM)
				TEXFMTMAP(TEX_FORMAT_RG16_UINT)
				TEXFMTMAP(TEX_FORMAT_RG16_SNORM)
				TEXFMTMAP(TEX_FORMAT_RG16_SINT)
				TEXFMTMAP(TEX_FORMAT_R32_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_D32_FLOAT)
				TEXFMTMAP(TEX_FORMAT_R32_FLOAT)
				TEXFMTMAP(TEX_FORMAT_R32_UINT)
				TEXFMTMAP(TEX_FORMAT_R32_SINT)
				TEXFMTMAP(TEX_FORMAT_R24G8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_D24_UNORM_S8_UINT)
				TEXFMTMAP(TEX_FORMAT_R24_UNORM_X8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_X24_TYPELESS_G8_UINT)
				TEXFMTMAP(TEX_FORMAT_RG8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_RG8_UNORM)
				TEXFMTMAP(TEX_FORMAT_RG8_UINT)
				TEXFMTMAP(TEX_FORMAT_RG8_SNORM)
				TEXFMTMAP(TEX_FORMAT_RG8_SINT)
				TEXFMTMAP(TEX_FORMAT_R16_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_R16_FLOAT)
				TEXFMTMAP(TEX_FORMAT_D16_UNORM)
				TEXFMTMAP(TEX_FORMAT_R16_UNORM)
				TEXFMTMAP(TEX_FORMAT_R16_UINT)
				TEXFMTMAP(TEX_FORMAT_R16_SNORM)
				TEXFMTMAP(TEX_FORMAT_R16_SINT)
				TEXFMTMAP(TEX_FORMAT_R8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_R8_UNORM)
				TEXFMTMAP(TEX_FORMAT_R8_UINT)
				TEXFMTMAP(TEX_FORMAT_R8_SNORM)
				TEXFMTMAP(TEX_FORMAT_R8_SINT)
				TEXFMTMAP(TEX_FORMAT_A8_UNORM)
				TEXFMTMAP(TEX_FORMAT_R1_UNORM)
				TEXFMTMAP(TEX_FORMAT_RGB9E5_SHAREDEXP)
				TEXFMTMAP(TEX_FORMAT_RG8_B8G8_UNORM)
				TEXFMTMAP(TEX_FORMAT_G8R8_G8B8_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC1_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC1_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC1_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_BC2_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC2_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC2_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_BC3_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC3_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC3_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_BC4_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC4_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC4_SNORM)
				TEXFMTMAP(TEX_FORMAT_BC5_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC5_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC5_SNORM)
				TEXFMTMAP(TEX_FORMAT_B5G6R5_UNORM)
				TEXFMTMAP(TEX_FORMAT_B5G5R5A1_UNORM)
				TEXFMTMAP(TEX_FORMAT_BGRA8_UNORM)
				TEXFMTMAP(TEX_FORMAT_BGRX8_UNORM)
				TEXFMTMAP(TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM)
				TEXFMTMAP(TEX_FORMAT_BGRA8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BGRA8_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_BGRX8_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BGRX8_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_BC6H_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC6H_UF16)
				TEXFMTMAP(TEX_FORMAT_BC6H_SF16)
				TEXFMTMAP(TEX_FORMAT_BC7_TYPELESS)
				TEXFMTMAP(TEX_FORMAT_BC7_UNORM)
				TEXFMTMAP(TEX_FORMAT_BC7_UNORM_SRGB)
				TEXFMTMAP(TEX_FORMAT_NUM_FORMATS)

				return defaultvalue;
		}

		Rendering::ShaderPipelineDesc ParsePipeline(toml::table* tbl)
		{
			Rendering::ShaderPipelineDesc desc;
			desc.mMainPSOCreateInfo.GraphicsPipeline.NumRenderTargets = tbl->get("NumRenderTargets")->as_integer()->value_or(1);

			if (toml::array* arr = tbl->get("RTVFormats")->as_array())
			{
				desc.mMainPSOCreateInfo.GraphicsPipeline.NumRenderTargets = arr->size();

				for (Uint32 i = 0; i< arr->size(); i++)
				{
					desc.mMainPSOCreateInfo.GraphicsPipeline.RTVFormats[i] = ParseTexFormat(arr[i].as_string()->value_or("TEX_FORMAT_RGBA8_UNORM_SRGB"sv));
				}
			}

			desc.mMainPSOCreateInfo.GraphicsPipeline.DSVFormat = ParseTexFormat(tbl->get("DSVFormat")->value_or("TEX_FORMAT_D32_FLOAT"sv));


			return desc;	
		}

		IShader* ParseShader(toml::table* tbl, SHADER_TYPE type)
		{
			ShaderCreateInfo CreationAttribs;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Desc.Name = "BlinnPhongVS";


			std::vector<std::string> defines;
			defines.push_back("NE_ENABLE_ANIMATION");

			if (mInitInfo.mDefferedPipeline)
			{
				defines.push_back("NE_DEFFERED");
			}
			if (mInitInfo.ShadowingEnabled == true && desc.pShadowPass)
			{
				defines.push_back("NE_SHADOWS");
				auto shadowpassdesc = desc.pShadowPass->GetBakingDesc();
				AddToDefinesIfNotZero(defines, "NE_MAX_DIR_CASTERS ", shadowpassdesc.MAX_DIR_CASTERS);
				AddToDefinesIfNotZero(defines, "NE_MAX_SPOT_CASTERS ", shadowpassdesc.MAX_SPOT_CASTERS);
				AddToDefinesIfNotZero(defines, "NE_MAX_OMNIDIR_CASTERS ", shadowpassdesc.MAX_OMNIDIR_CASTERS);
			}
			auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Basic.vs.hlsl", defines, std::vector<std::string>(), true);
			CreationAttribs.Source = source.c_str();
			RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
			Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/", &pShaderSourceFactory);
			CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;
			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, VSShader.RawDblPtr());
		}

		Assets::ShaderBuildDesc ShaderManager::ParseShaderAsset(const std::string& source)
		{
			toml::table tbl;
			Assets::ShaderBuildDesc desc;

			try
			{
				tbl = toml::parse(source);

				std::string_view ShaderName = tbl["Shader"]["Name"].value_or("UnNamed"sv);

				desc.mSupportDefferedRendering = tbl["Shader"]["SupportDefferedRendering"].as_boolean()->value_or(false);
				desc.mSupportSkinnedMeshes = tbl["Shader"]["SupportSkinnedMeshes"].as_boolean()->value_or(false);
				desc.mSupportShadows = tbl["Shader"]["SupportShadows"].as_boolean()->value_or(false);

				std::optional<std::string_view> str1 = tbl["Shader"]["ForwardPipeline"].value<std::string_view>();
				if (str1.has_value())
				{
					ParsePipeline(tbl.get(str1.value())->as_table());
				}
			}
			catch (const toml::parse_error& err)
			{
				std::cerr << "Parsing failed:\n" << err << "\n";
				return desc;
			}

			return desc;
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
		std::vector<LayoutElement> ShaderManager::GetBasicVSLayout(bool isDeffered)
		{
			std::vector<LayoutElement> LayoutElems;

			LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
			LayoutElems.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV

			if (!isDeffered)
			{
				LayoutElems.push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
				LayoutElems.push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
				LayoutElems.push_back(LayoutElement(4, 0, 3, VT_FLOAT32, false));  //Bitangents
				LayoutElems.push_back(LayoutElement(5, 0, 4, VT_INT32, false));    //BONE ID
				LayoutElems.push_back(LayoutElement(6, 0, 4, VT_FLOAT32, false));  //WEIGHT
			}

			return LayoutElems;
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

				for (auto& i : Resources)
				{
					std::string name(i.Name);
					//Check its type
					if (name.find("NEMat_") != std::string::npos)
					{
						SamplerDesc SamLinearWrapDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
							TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP);
						ImmutableSamplerDesc desc(SHADER_TYPE_PIXEL, i.Name, SamLinearWrapDesc);
						GeneratedSamplerDesc.push_back(desc);
					}
					else if (name.find("NE_ShadowMap") != std::string::npos)
					{
						SamplerDesc SamLinearWrapDesc(FILTER_TYPE_POINT, FILTER_TYPE_POINT, FILTER_TYPE_POINT,
							TEXTURE_ADDRESS_BORDER, TEXTURE_ADDRESS_BORDER, TEXTURE_ADDRESS_BORDER);
						//SamLinearWrapDesc.ComparisonFunc = COMPARISON_FUNC_LESS;
						SamLinearWrapDesc.BorderColor[0] = 1.0f;
						SamLinearWrapDesc.BorderColor[1] = 1.0f;
						SamLinearWrapDesc.BorderColor[2] = 1.0f;
						SamLinearWrapDesc.BorderColor[3] = 1.0f;

					//	SamLinearWrapDesc.MipLODBias = 0.f;
						//SamLinearWrapDesc.MaxAnisotropy = 0;
						ImmutableSamplerDesc desc(SHADER_TYPE_PIXEL, i.Name, SamLinearWrapDesc);
						GeneratedSamplerDesc.push_back(desc);
					}
					else if (name.find("NE_RT_") != std::string::npos || name.find("NEIBL_") != std::string::npos)
					{
						SamplerDesc SamLinearClampDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
							TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP);
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
			if (VShader)
			{
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
			}
			else {
				NUCLEAR_WARN("[ShaderManager] ReflectShaderVariables() skipped null Vertex Shader...");
			}
			if (PShader)
			{
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
			}
			else {
				NUCLEAR_WARN("[ShaderManager] ReflectShaderVariables() skipped null Pixel Shader...");
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
			else if (name.find("NE_RT") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;
			else if (name.find("NEIBL") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;
			else if (name.find("NE_ShadowMap") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;

			return SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
		}
	}
}