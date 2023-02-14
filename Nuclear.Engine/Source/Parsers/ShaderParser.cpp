#include <Parsers/ShaderParser.h>
#include <Parsers/toml.h>
#include <Utilities/Logger.h>
#include <ThirdParty/magic_enum.hpp>
using namespace std::string_view_literals;

namespace Nuclear
{
	namespace Parsers
	{

#define TEXFMTMAP(name)	else if (!str.compare(#name)) { return name; }

		using namespace Diligent;
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


		void ParsePSOShader(toml::table* tbl, std::string_view& name, SHADER_TYPE type, Graphics::ShaderObjectCreationDesc& result)
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
					NUCLEAR_ERROR("[ShaderManager] Shader: ", name, " has no Source or Path!");
				}
			}
		}


		void ParsePSO(toml::table* tbl, toml::table& parent, Graphics::ShaderPSODesc& desc)
		{
			if (toml::array* arr = tbl->get("RTVFormats")->as_array())
			{
				desc.GraphicsPipeline.NumRenderTargets = arr->size();

				for (Uint32 i = 0; i < arr->size(); i++)
				{
					desc.GraphicsPipeline.RTVFormats[i] = ParseTexFormat(arr->at(i).value_or("TEX_FORMAT_RGBA8_UNORM_SRGB"sv));
				}
			}

			auto rtnames = tbl->get("RTVNames");
			if (rtnames)
			{
				toml::array* arr = rtnames->as_array();
				for (Uint32 i = 0; i < desc.GraphicsPipeline.NumRenderTargets; i++)
				{
					if (i < arr->size())
					{
						desc.mRTsNames.push_back(arr->at(i).value_or("UnNamed RT"));
					}
					else
					{
						desc.mRTsNames.push_back("UnNamed RT");
					}
				}
			}
			else {
				for (Uint32 i = 0; i < desc.GraphicsPipeline.NumRenderTargets; i++)
				{
					desc.mRTsNames.push_back("UnNamed RT" + std::to_string(i));
				}
			}

			desc.GraphicsPipeline.DSVFormat = ParseTexFormat(tbl->get("DSVFormat")->value_or("TEX_FORMAT_D32_FLOAT"sv));

			auto defines = tbl->get("Defines");
			if (defines)
			{
				toml::array* arr = defines->as_array();

				for (Uint32 i = 0; i < arr->size(); i++)
				{
					std::optional<std::string> val = arr->at(i).value<std::string>();

					if (val.has_value())
					{
						desc.mVertexShader.mDefines.insert(val.value());
						desc.mPixelShader.mDefines.insert(val.value());
					}
				}
			}

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

			desc.mValid = true;
		}

		bool ShaderParser::ParseSource(const std::string& source, Graphics::ShaderBuildDesc& desc)
		{
			toml::table tbl;

			try
			{
				tbl = toml::parse(source);

				std::string_view ShaderName = tbl["Shader"]["Name"].value_or("UnNamed"sv);
				desc.mPipelineDesc.mName = ShaderName;
				desc.mSupportSkinnedMeshes = tbl["Shader"]["SupportSkinnedMeshes"].value_or(false);
				desc.mSupportShadows = tbl["Shader"]["SupportShadows"].value_or(false);
				desc.mType = magic_enum::enum_cast<Graphics::ShaderType>(tbl["Shader"]["Type"].value_or("Unknown")).value_or(Graphics::ShaderType::Unknown);
				toml::array* arr = tbl["Shader"]["Variants"].as_array();
				if (arr)
				{
					auto arrsize = arr->size();
					std::vector<int> excludedvariantsIndex;
					for (Uint32 i = 0; i < arrsize; i++)
					{
						bool match = false;
						auto name = arr->at(i).as_string()->value_or("ERROR");
						for (auto& j : desc.mExcludedVariants)
						{
							if (name == j)
							{
								match = true;
								excludedvariantsIndex.push_back(i);
							}
						}

						if(!match)
							desc.mPipelineDesc.Switches.push_back(Graphics::ShaderPipelineSwitch(name));
					}

					toml::array* valarr = tbl["Shader"]["VariantsValues"].as_array();
					std::vector<bool> VariantsValues;
					if (valarr)
					{
						for (Uint32 i = 0; i < valarr->size(); i++)
						{
							bool match = false;
							for (Uint32 j = 0; j < excludedvariantsIndex.size(); j++)
							{
								if (i == excludedvariantsIndex[j])
								{
									match = true;
								}
							}

							if(!match)
								VariantsValues.push_back(valarr->at(i).as_boolean()->value_or(true));
						}

						for (Uint32 i = 0; i < VariantsValues.size(); i++)
						{
							desc.mPipelineDesc.Switches.at(i).SetValue(VariantsValues[i]);
						}
					}
				}

				bool hasforward = false, hasdeffered = false;
				{
					{
						std::optional<std::string_view> str1 = tbl["Shader"]["ForwardPipeline"].value<std::string_view>();
						if (str1.has_value())
						{
							ParsePSO(tbl.get(str1.value())->as_table(), tbl, desc.mPipelineDesc.mForwardPSOCreateInfo);
							hasforward = true;
						}
					}
					{
						std::optional<std::string> str1 = tbl["Shader"]["DefferedPipeline"].value<std::string>();

						if (str1.has_value())
						{
							auto tbl1 = tbl.get(str1.value());
							if (tbl1)
							{
								ParsePSO(tbl1->as_table(), tbl, desc.mPipelineDesc.mDefferedPSOCreateInfo);
								hasdeffered = true;
							}
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



				if (hasforward && hasdeffered)
					desc.mSupportedTechniques = Graphics::SupportedRenderingTechnique::ForwardDeffered;				
				else if (hasforward)				
					desc.mSupportedTechniques = Graphics::SupportedRenderingTechnique::ForwardOnly;				
				else if (hasdeffered)				
					desc.mSupportedTechniques = Graphics::SupportedRenderingTechnique::DefferedOnly;				
				else				
					desc.mSupportedTechniques = Graphics::SupportedRenderingTechnique::Unknown;
				
			}
			catch (const toml::parse_error& err)
			{
				NUCLEAR_ERROR("[ShaderManager] Parsing Shader failed: '{0}'", err.description());
				return false;
			}

			return true;
		}

}
}