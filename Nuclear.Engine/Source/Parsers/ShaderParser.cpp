#include <Parsers/ShaderParser.h>
#include <Parsers/toml.h>
#include <Utilities/Logger.h>
#include <ThirdParty/magic_enum.hpp>
using namespace std::string_view_literals;

namespace Nuclear
{
	namespace Parsers
	{
		using namespace Diligent;
	
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
					desc.GraphicsPipeline.RTVFormats[i] = magic_enum::enum_cast<TEXTURE_FORMAT>(arr->at(i).value_or("TEX_FORMAT_RGBA8_UNORM_SRGB")).value_or(TEX_FORMAT_RGBA8_UNORM_SRGB);
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
			
			desc.GraphicsPipeline.DSVFormat = magic_enum::enum_cast<TEXTURE_FORMAT>(tbl->get("DSVFormat")->value_or("TEX_FORMAT_D32_FLOAT")).value_or(TEX_FORMAT_D32_FLOAT);
			desc.GraphicsPipeline.PrimitiveTopology = magic_enum::enum_cast<PRIMITIVE_TOPOLOGY>(tbl->get("PrimitiveTopology")->value_or("PRIMITIVE_TOPOLOGY_TRIANGLE_LIST")).value_or(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
			desc.GraphicsPipeline.RasterizerDesc.CullMode = magic_enum::enum_cast<CULL_MODE>(tbl->get("RasterizerDesc.CullMode")->value_or("CULL_MODE_BACK")).value_or(CULL_MODE_BACK);
			desc.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = tbl->get("RasterizerDesc.FrontCounterClockwise")->value_or(!COORDSYSTEM_LH_ENABLED);
			desc.GraphicsPipeline.DepthStencilDesc.DepthEnable = tbl->get("DepthStencilDesc.DepthEnable")->value_or(true);
			desc.GraphicsPipeline.DepthStencilDesc.StencilEnable = tbl->get("DepthStencilDesc.StencilEnable")->value_or(false);

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


			auto layout = tbl->get("InputLayout");
			if (layout)
			{
				toml::array* arr = layout->as_array();
				if (arr->is_array())
				{
					for (Uint32 i = 0; i < arr->size(); i++)
					{
						if (arr->at(i).is_array())
						{
							auto layoutobj = arr->at(i).as_array();
							assert(layoutobj->size() == 5);
							Diligent::LayoutElement element;

							element.InputIndex = layoutobj->at(0).as_integer()->value_or(0);
							element.BufferSlot = layoutobj->at(1).as_integer()->value_or(0);
							element.NumComponents = layoutobj->at(2).as_integer()->value_or(0);
							element.ValueType = magic_enum::enum_cast<VALUE_TYPE>(layoutobj->at(3).as_string()->value_or("VT_FLOAT32")).value_or(VT_FLOAT32);
							element.IsNormalized = layoutobj->at(4).as_boolean()->value_or(true);
							//element.RelativeOffset = layoutobj->at(5).as_integer()->value_or(LAYOUT_ELEMENT_AUTO_OFFSET);
							//element.Stride = layoutobj->at(6).as_integer()->value_or(LAYOUT_ELEMENT_AUTO_STRIDE);
							//element.Frequency = magic_enum::enum_cast<INPUT_ELEMENT_FREQUENCY>(layoutobj->at(7).as_string()->value_or("INPUT_ELEMENT_FREQUENCY_PER_VERTEX")).value_or(INPUT_ELEMENT_FREQUENCY_PER_VERTEX);
							//element.InstanceDataStepRate = layoutobj->at(8).as_integer()->value_or(1);

							desc.mLayout.push_back(element);
						}
						else
						{
							NUCLEAR_ERROR("[ShaderManager] Parsing Shader error -> InputLayout element {0} isn't an array", i);
						}
					}
				}
				else
				{
					NUCLEAR_ERROR("[ShaderManager] Parsing Shader error -> InputLayout isn't an array");
				}

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