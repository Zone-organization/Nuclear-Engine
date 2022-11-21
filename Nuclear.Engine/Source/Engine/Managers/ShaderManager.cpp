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

		void ShaderManager::Initialize()
		{
			Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/", &pShaderSourceFactory);
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


		void ParsePSOShader(toml::table* tbl,std::string_view& name, SHADER_TYPE type , Graphics::ShaderObjectCreationDesc& result)
		{
			result.mType = type;
			result.mName = name.data();
			auto entrypointnode = tbl->get("EntryPoint");
			if (entrypointnode)
			{
				result.mEntrypoint = entrypointnode->value_or("main"sv);

			}

			auto defines = tbl->get("Defines");
			if (defines)
			{
				toml::array* arr = defines->as_array();

				for (Uint32 i = 0; i < arr->size(); i++)
				{
					if (arr[i].as_string())
					{
						result.mDefines.insert(arr[i].value_or(""));
					}
				}
			}
			auto sourcenode = tbl->get("Source");
			if (sourcenode)
			{
				std::optional<std::string> source = sourcenode->value<std::string>();
				if (source.has_value())
				{
					result.mSource = source.value();
				}
			}
			else 
			{
				auto pathnode = tbl->get("Path");
				if (pathnode)
				{
					std::optional<std::string> path = pathnode->value<std::string>();
					if (path.has_value())
					{
						result.mPath = Core::Path(path.value());
					}
				}
				else {
					NUCLEAR_ERROR("[ShaderManager] Shader: ", name , " has no Source or Path!");
				}
			}
		}



		void ParsePSO(toml::table* tbl, toml::table& parent, Graphics::ShaderPSODesc& desc)
		{
			if (toml::array* arr = tbl->get("RTVFormats")->as_array())
			{
				desc.GraphicsPipeline.NumRenderTargets = arr->size();

				for (Uint32 i = 0; i< arr->size(); i++)
				{
					desc.GraphicsPipeline.RTVFormats[i] = ParseTexFormat(arr->at(i).value_or("TEX_FORMAT_RGBA8_UNORM_SRGB"sv));
				}
			}

			desc.GraphicsPipeline.DSVFormat = ParseTexFormat(tbl->get("DSVFormat")->value_or("TEX_FORMAT_D32_FLOAT"sv));

			auto vsnode = tbl->get("VertexShader");

			if (vsnode)
			{
				auto str1 = vsnode->value<std::string_view>();
				 ParsePSOShader(parent.get(str1.value())->as_table(), str1.value(), SHADER_TYPE_VERTEX, desc.mVertexShader);
			}
			auto psnode = tbl->get("PixelShader");
			if (psnode)
			{
				auto str2 = psnode->value<std::string_view>();
				 ParsePSOShader(parent.get(str2.value())->as_table(), str2.value(), SHADER_TYPE_PIXEL, desc.mPixelShader);
			}
		}

		bool ShaderManager::ParseShaderAsset(const std::string& source, Assets::ShaderBuildDesc& desc)
		{
			toml::table tbl;

			try
			{
				tbl = toml::parse(source);

				std::string_view ShaderName = tbl["Shader"]["Name"].value_or("UnNamed"sv);
				desc.mPipelineDesc.mName = ShaderName;
				desc.mSupportSkinnedMeshes = tbl["Shader"]["SupportSkinnedMeshes"].value_or(false);
				desc.mSupportShadows = tbl["Shader"]["SupportShadows"].value_or(false);

				if (toml::array* arr = tbl["Shader"]["Variants"].as_array())
				{
					for (Uint32 i = 0; i < arr->size(); i++)
					{
						desc.mPipelineDesc.Switches.push_back(Graphics::ShaderPipelineSwitch(arr->at(i).as_string()->value_or("")));
					}
				}
				else {

				}


				desc.mSupportForwardRendering = tbl["Shader"]["SupportForwardRendering"].value_or(true);
				desc.mSupportDefferedRendering = tbl["Shader"]["SupportDefferedRendering"].value_or(false);

				if (desc.mSupportForwardRendering)
				{
					std::optional<std::string_view> str1 = tbl["Shader"]["ForwardPipeline"].value<std::string_view>();
					if (str1.has_value())
					{
						ParsePSO(tbl.get(str1.value())->as_table(), tbl, desc.mPipelineDesc.mForwardPSOCreateInfo);
					}
				}

				if (desc.mSupportDefferedRendering)
				{
					std::optional<std::string> str1 = tbl["Shader"]["DefferedPipeline"].value<std::string>();

					if (str1.has_value())
					{ 
						auto tbl1 = tbl.get(str1.value());
						if (tbl1)
							ParsePSO(tbl1->as_table(), tbl, desc.mPipelineDesc.mDefferedPSOCreateInfo);
						else
							NUCLEAR_ERROR("[ShaderManager] Parsing Shader error -> DefferedPipeline '{0}' not found", str1.value());
					}

					std::optional<std::string> str2 = tbl["Shader"]["GBufferPipeline"].value<std::string>();
					if (str2.has_value())
					{
						auto tbl2 = tbl.get(str2.value());
						if (tbl2)
							ParsePSO(tbl2->as_table(), tbl, desc.mPipelineDesc.mGBufferPSOCreateInfo);
						else
							NUCLEAR_ERROR("[ShaderManager] Parsing Shader error -> GBufferPipeline '{0}' not found", str2.value());
					}
				}
			
			}
			catch (const toml::parse_error& err)
			{
				NUCLEAR_ERROR("[ShaderManager] Parsing Shader failed: '{0}'", err.description());
				return false;
			}

			return true;
		}

		/*ShadersReflection ShaderManager::ReflectShaderAsset(Assets::ShaderBuildDesc& desc)
		{
			ShadersReflection result;
			std::vector<ShaderResourceVariableDesc> resources;
			if (desc.mPipelineDesc.mVertexShader)
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
			return ShadersReflection();
		}*/


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ShaderManager::CreateShader(IShader** result, const Graphics::ShaderObjectCreationDesc& desc)
		{
			ShaderCreateInfo CreationAttribs;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = desc.mType;
			CreationAttribs.EntryPoint = desc.mEntrypoint.c_str();
			CreationAttribs.Desc.Name = desc.mName.c_str();
			CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;

			std::string Source = desc.mSource;
			if (Source.empty())
			{
				Source = Core::FileSystem::LoadShader(desc.mPath.GetRealPath(), desc.mDefines, std::set<std::string>(), true);
			}
			else
			{
				//add defines
				if (desc.mDefines.size() > 0)
				{
					std::vector<std::string> MergedCode;

					std::string firstLine = Source.substr(0, Source.find("\n"));
					
					for (auto& i : desc.mDefines)
					{
						std::string define = "#define " + i + "\n";
						MergedCode.push_back(define);
					}

					MergedCode.push_back(Source);
					std::string str;
					for (unsigned int i = 0; i < MergedCode.size(); ++i)
						str = str + MergedCode[i].c_str();

					Source = str;
				}
				
			}

			CreationAttribs.Source = Source.c_str();

			Graphics::Context::GetDevice()->CreateShader(CreationAttribs, result);
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