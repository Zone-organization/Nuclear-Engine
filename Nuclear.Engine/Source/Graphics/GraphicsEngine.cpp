#include "Graphics\GraphicsEngine.h"
#include <Graphics\Context.h>
#include <Assets\AssetManager.h>
#include "..\Graphics\ImGUI\imgui_impl_sdl.h"
#include "..\Graphics\ImGUI\imgui_impl.h"
#include <Utilities/Logger.h>
#include <Platform\FileSystem.h>
#include <Assets/DefaultMeshes.h>
#include <Diligent\Graphics\GraphicsAccessories\interface\GraphicsAccessories.hpp>
#include <Diligent\Graphics\GraphicsTools\interface\GraphicsUtilities.h>
#include <Assets\Texture.h>

namespace Nuclear
{
	namespace Graphics
	{
		inline GraphicsEngine& GraphicsEngine::GetInstance()
		{
			static GraphicsEngine engine;

			return engine;
		}
		bool GraphicsEngine::Initialize(const GraphicsEngineDesc& desc)
		{
			//Initialize Context
			if (!Graphics::Context::GetInstance().Initialize(desc))
			{
				NUCLEAR_INFO("[GraphicsEngine] Failed to initialize Context!");
				return false;
			}

			Graphics::Context::GetInstance().GetEngineFactory()->CreateDefaultShaderSourceStreamFactory(Core::Path("@NuclearAssets@/Shaders/").GetRealPath().c_str(), &pShaderSourceFactory);
			Assets::DefaultMeshes::Initialize();

			if (desc.InitImGui)
			{
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.Fonts->AddFontFromFileTTF(Core::Path("@NuclearAssets@/Fonts/Roboto-Medium.ttf").GetRealPath().c_str(), 15);
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				io.ConfigDockingWithShift = true;
				ImGui_ImplSDL2_InitForDiligent(desc.pWindowHandle);
				ImGui_Impl_Init();
				ImGui_Impl_CreateDeviceObjects();
				//io.Fonts->GetTexDataAsAlpha8()

				NUCLEAR_INFO("[GraphicsEngine] ImGUI Initalized.");
			}

			NUCLEAR_INFO("[GraphicsEngine] GraphicsEngine has has been initalized successfully!.");
			return true;
		}
		void GraphicsEngine::Shutdown()
		{
			NUCLEAR_INFO("[GraphicsEngine] Shutting down...");
			Graphics::Context::GetInstance().Shutdown();
		}

		bool GraphicsEngine::CreateImage(Assets::Texture* result, Assets::ImageData* data)
		{
			result->mTextureView = nullptr;

			result->mWidth = data->mTexDesc.Width;
			result->mHeight = data->mTexDesc.Height;

			//CREATE IMAGE
			Diligent::TextureData TexData;
			TexData.pSubResources = data->mSubresources.data();
			TexData.NumSubresources = data->mSubresources.size();
			RefCntAutoPtr<ITexture> mTexture;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(data->mTexDesc, &TexData, &mTexture);

			if (mTexture.RawPtr() != nullptr)
			{
				result->mTextureView = mTexture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
				result->SetState(Assets::IAsset::State::Created);
				return true;
			}

			return false;
		}

		template <typename ChannelType>
		void ModifyComponentCount(const void* pSrcData,
			Uint32      SrcStride,
			Uint32      SrcCompCount,
			void* pDstData,
			Uint32      DstStride,
			Uint32      Width,
			Uint32      Height,
			Uint32      DstCompCount)
		{
			auto CompToCopy = std::min(SrcCompCount, DstCompCount);
			for (size_t row = 0; row < size_t{ Height }; ++row)
			{
				for (size_t col = 0; col < size_t{ Width }; ++col)
				{
					// clang-format off
					auto* pDst = reinterpret_cast<ChannelType*>((reinterpret_cast<Uint8*>(pDstData) + size_t{ DstStride } *row)) + col * DstCompCount;
					const auto* pSrc = reinterpret_cast<const ChannelType*>((reinterpret_cast<const Uint8*>(pSrcData) + size_t{ SrcStride } *row)) + col * SrcCompCount;
					// clang-format on

					for (size_t c = 0; c < CompToCopy; ++c)
						pDst[c] = pSrc[c];

					for (size_t c = CompToCopy; c < DstCompCount; ++c)
					{
						pDst[c] = c < 3 ?
							(SrcCompCount == 1 ? pSrc[0] : 0) :      // For single-channel source textures, propagate r to other channels
							std::numeric_limits<ChannelType>::max(); // Use 1.0 as default value for alpha
					}
				}
			}
		}

		void GraphicsEngine::CreateImageData(Assets::ImageData* result, const Assets::ImageDesc& desc)
		{
			result->mTexDesc.Type = desc.mType;
			result->mTexDesc.Usage = desc.mUsage;
			result->mTexDesc.BindFlags = desc.mBindFlags;
			result->mTexDesc.Format = desc.mFormat;
			result->mTexDesc.CPUAccessFlags = desc.mCPUAccessFlags;
			result->mTexDesc.Width = desc.mWidth;
			result->mTexDesc.Height = desc.mHeight;

			if (result->mSubresources.size() == 0)
			{
				result->mTexDesc.MipLevels = ComputeMipLevelsCount(result->mTexDesc.Width, result->mTexDesc.Height);
				if (desc.mMipLevels > 0)
					result->mTexDesc.MipLevels = std::min(result->mTexDesc.MipLevels, desc.mMipLevels);

				Uint32 NumComponents = 0;
				const auto ChannelDepth = GetValueSize(desc.mComponentType) * 8;

				bool IsSRGB = (desc.mNumComponents >= 3 && ChannelDepth == 8) ? desc.mIsSRGB : false;
				if (result->mTexDesc.Format == TEX_FORMAT_UNKNOWN)
				{
					NumComponents = desc.mNumComponents == 3 ? 4 : desc.mNumComponents;

					if (ChannelDepth == 8)
					{
						switch (NumComponents)
						{
						case 1: result->mTexDesc.Format = TEX_FORMAT_R8_UNORM; break;
						case 2: result->mTexDesc.Format = TEX_FORMAT_RG8_UNORM; break;
						case 4: result->mTexDesc.Format = IsSRGB ? TEX_FORMAT_RGBA8_UNORM_SRGB : TEX_FORMAT_RGBA8_UNORM; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", desc.mNumComponents, ")");
						}
					}
					else if (ChannelDepth == 16)
					{
						switch (NumComponents)
						{
						case 1: result->mTexDesc.Format = TEX_FORMAT_R16_UNORM; break;
						case 2: result->mTexDesc.Format = TEX_FORMAT_RG16_UNORM; break;
						case 4: result->mTexDesc.Format = TEX_FORMAT_RGBA16_UNORM; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", desc.mNumComponents, ")");
						}
					}
					else if (ChannelDepth == 32)
					{
						switch (NumComponents)
						{
						case 1: result->mTexDesc.Format = TEX_FORMAT_R16_FLOAT; break;
						case 2: result->mTexDesc.Format = TEX_FORMAT_RG16_FLOAT; break;
						case 4: result->mTexDesc.Format = TEX_FORMAT_RGBA32_FLOAT; break;
						default: LOG_ERROR_AND_THROW("Unexpected number of color channels (", desc.mNumComponents, ")");
						}
					}
					else
						LOG_ERROR_AND_THROW("Unsupported color channel depth (", ChannelDepth, ")");
				}
				else
				{
					const auto& TexFmtDesc = GetTextureFormatAttribs(result->mTexDesc.Format);

					NumComponents = TexFmtDesc.NumComponents;
					if (TexFmtDesc.ComponentSize != ChannelDepth / 8)
						LOG_ERROR_AND_THROW("Image channel size ", ChannelDepth, " is not compatible with texture format ", TexFmtDesc.Name);
				}
				result->mSubresources = std::vector<TextureSubResData>(result->mTexDesc.MipLevels);
				result->mMips = std::vector< std::vector<Uint8> >(result->mTexDesc.MipLevels);

				auto& pSubResources = result->mSubresources;
				auto& Mips = result->mMips;

				if (desc.mNumComponents != NumComponents)
				{
					auto DstStride = desc.mWidth * NumComponents * ChannelDepth / 8;
					DstStride = AlignUp(DstStride, Uint32{ 4 });
					Mips[0].resize(size_t{ DstStride } *size_t{ desc.mHeight });
					pSubResources[0].pData = Mips[0].data();
					pSubResources[0].Stride = DstStride;
					if (ChannelDepth == 8)
					{
						ModifyComponentCount<Uint8>(desc.mData, desc.mRowStride, desc.mNumComponents,
							Mips[0].data(), DstStride,
							desc.mWidth, desc.mHeight, NumComponents);
					}
					else if (ChannelDepth == 16)
					{
						ModifyComponentCount<Uint16>(desc.mData, desc.mRowStride, desc.mNumComponents,
							Mips[0].data(), DstStride,
							desc.mWidth, desc.mHeight, NumComponents);
					}
					else if (ChannelDepth == 32)
					{
						ModifyComponentCount<float>(desc.mData, desc.mRowStride, desc.mNumComponents,
							Mips[0].data(), DstStride,
							desc.mWidth, desc.mHeight, NumComponents);
					}
				}
				else
				{
					auto MipLevelProps = GetMipLevelProperties(result->mTexDesc, 0);

					pSubResources[0].pData = desc.mData;
					pSubResources[0].Stride = MipLevelProps.RowSize;
				}

				for (Uint32 m = 1; m < result->mTexDesc.MipLevels; ++m)
				{
					auto MipLevelProps = GetMipLevelProperties(result->mTexDesc, m);
					Mips[m].resize(StaticCast<size_t>(MipLevelProps.MipSize));
					pSubResources[m].pData = Mips[m].data();
					pSubResources[m].Stride = MipLevelProps.RowSize;

					if (desc.mGenerateMipMaps)
					{
						auto FinerMipProps = GetMipLevelProperties(result->mTexDesc, m - 1);

						ComputeMipLevelAttribs Attribs;
						Attribs.Format = result->mTexDesc.Format;
						Attribs.FineMipWidth = FinerMipProps.LogicalWidth;
						Attribs.FineMipHeight = FinerMipProps.LogicalHeight;
						Attribs.pFineMipData = pSubResources[m - 1].pData;
						Attribs.FineMipStride = StaticCast<size_t>(pSubResources[m - 1].Stride);
						Attribs.pCoarseMipData = Mips[m].data();
						Attribs.CoarseMipStride = StaticCast<size_t>(pSubResources[m].Stride);
						Attribs.AlphaCutoff = desc.mMipMapsAlphaCutoff;
						Attribs.FilterType = desc.mMipMapsFilter;
						ComputeMipLevel(Attribs);
					}
				}
			}
		}

		bool GraphicsEngine::isGammaCorrect()
		{
			if (Graphics::Context::GetInstance().GetSwapChain()->GetDesc().ColorBufferFormat == TEX_FORMAT_RGBA8_UNORM_SRGB)
				return true;
			else
				return false;
		}

		std::vector<LayoutElement> GraphicsEngine::GetBasicVSLayout(bool isDeffered)
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

		void GraphicsEngine::CreateShader(IShader** result, const Graphics::ShaderObjectCreationDesc& desc)
		{
			ShaderCreateInfo CreationAttribs;

			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.Desc.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = desc.mType;
			CreationAttribs.EntryPoint = desc.mEntrypoint.c_str();
			CreationAttribs.Desc.Name = desc.mName.c_str();
			CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;

			std::string Source = desc.mSource;
			if (Source.empty())
			{
				Source = Platform::FileSystem::GetInstance().LoadShader(desc.mPath.GetRealPath(), desc.mDefines, std::set<std::string>(), true);
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

			Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, result);
		}

		IShaderSourceInputStreamFactory* GraphicsEngine::GetDefaultShaderSourceFactory()
		{
			return pShaderSourceFactory;
		}


		void GraphicsEngine::CreateShader(const std::string& source, IShader** result, SHADER_TYPE type)
		{
			ShaderCreateInfo CreationAttribs;
			CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.Desc.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = type;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Source = source.c_str();

			Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, result);
		}

		bool GraphicsEngine::ProcessAndCreatePipeline(
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

			Graphics::Context::GetInstance().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, PipelineState);

			return true;
		}

		bool CheckSampler(const std::string& name)
		{
			if (name.find("_sampler") != std::string::npos)
				return true;

			return false;
		}

		std::vector<ShaderResourceVariableDesc> GraphicsEngine::ReflectShaderVariables(IShader* VShader, IShader* PShader)
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
				NUCLEAR_WARN("[GraphicsEngine] ReflectShaderVariables() skipped null Vertex Shader...");
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
				NUCLEAR_WARN("[GraphicsEngine] ReflectShaderVariables() skipped null Pixel Shader...");
			}
			return resources;
		}
		SHADER_RESOURCE_VARIABLE_TYPE GraphicsEngine::ParseNameToGetType(const std::string& name)
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

		GraphicsEngine::GraphicsEngine()
		{
		}

	}
}