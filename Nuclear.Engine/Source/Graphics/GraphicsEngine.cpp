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
#include <Fallbacks/FallbacksEngine.h>

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

			mDefaultStaticSamplers["NEMat_"] = SamLinearWrapDesc;
			mDefaultStaticSamplers["NE_ShadowMap"] = ShadowMapSamplerDesc;
			mDefaultStaticSamplers["NE_RT_"] = SamLinearClampDesc;
			mDefaultStaticSamplers["NEIBL_"] = SamLinearClampDesc;

			NUCLEAR_INFO("[GraphicsEngine] GraphicsEngine has has been initalized successfully!");
			return true;
		}
		void GraphicsEngine::Shutdown()
		{
			NUCLEAR_INFO("[GraphicsEngine] Shutting down...");
			Graphics::Context::GetInstance().Shutdown();
		}

		bool GraphicsEngine::CreateMesh(Assets::Mesh* mesh)
		{
			if (mesh)
			{
				bool result = true;

				for (unsigned int i = 0; i < mesh->mSubMeshes.size(); i++)
				{
					if (!mesh->mSubMeshes.at(i).Create())
					{
						result = false;
					}
				}

				mesh->SetState(Assets::IAsset::State::Created);
				return result;
			}
			return false;
		}

		bool GraphicsEngine::CreateImage(Assets::Texture* result, Assets::TextureData* data)
		{
			//CREATE IMAGE
			Diligent::TextureData TexData;
			TexData.pSubResources = data->mSubresources.data();
			TexData.NumSubresources = data->mSubresources.size();
			Diligent::RefCntAutoPtr<Diligent::ITexture> mTexture;
			Graphics::Context::GetInstance().GetDevice()->CreateTexture(data->mTexDesc, &TexData, &mTexture);

			if (mTexture.RawPtr() != nullptr)
			{
				result->mTextureView = nullptr;
				result->mWidth = data->mTexDesc.Width;
				result->mHeight = data->mTexDesc.Height;

				result->mTextureView = mTexture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
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

		void GraphicsEngine::CreateImageData(Assets::TextureData* result, const Assets::TextureDesc& desc)
		{
			using namespace Diligent;

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
			if (Graphics::Context::GetInstance().GetSwapChain()->GetDesc().ColorBufferFormat == Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB)
				return true;
			else
				return false;
		}

		Assets::TextureUsageType GraphicsEngine::ParseTexUsageFromName(std::string& name)
		{
			if (name.find("Diffuse") == 0)
				return Assets::TextureUsageType::Diffuse;
			else if (name.find("Albedo") == 0)
				return Assets::TextureUsageType::Albedo;
			else if (name.find("Specular") == 0)
				return Assets::TextureUsageType::Specular;
			else if (name.find("Metallic") == 0)
				return Assets::TextureUsageType::Metallic;
			else if (name.find("Normal") == 0)
				return Assets::TextureUsageType::Normal;
			else if (name.find("Roughness") == 0)
				return Assets::TextureUsageType::Roughness;
			else if (name.find("AO") == 0)
				return Assets::TextureUsageType::AO;

			else if (name.find("SSAO") == 0 || name.find("AmbientOcclusion") == 0)
				return Assets::TextureUsageType::AmbientOcclusion;

			//IBL
			else if (name.find("IrradianceMap") == 0)
				return Assets::TextureUsageType::IrradianceMap;
			else if (name.find("PrefilterMap") == 0)
				return Assets::TextureUsageType::PreFilterMap;
			else if (name.find("BRDF_LUT") == 0)
				return Assets::TextureUsageType::BRDF_LUT;

			//Shadows
			else if (name.find("ShadowMap") == 0)
				return Assets::TextureUsageType::ShadowTex;

			return Assets::TextureUsageType::Unknown;
		}

		bool GraphicsEngine::ReflectShader(const ShaderBuildDesc& desc, ShaderReflection& out)
		{
			using namespace Diligent;

			//create pixel shader
			RefCntAutoPtr<IShader> PShader;
			ShaderObjectCreationDesc pixelshader;

			if ((desc.mSupportedTechniques == SupportedRenderingTechnique::ForwardDeffered
				|| desc.mSupportedTechniques == SupportedRenderingTechnique::ForwardOnly)
				&& desc.mPipelineDesc.mForwardPSOCreateInfo.mValid)
				pixelshader = desc.mPipelineDesc.mForwardPSOCreateInfo.mPixelShader;
			else if (desc.mSupportedTechniques == SupportedRenderingTechnique::DefferedOnly && desc.mPipelineDesc.mGBufferPSOCreateInfo.mValid)
				pixelshader = desc.mPipelineDesc.mGBufferPSOCreateInfo.mPixelShader;
			else if (desc.mSupportedTechniques == SupportedRenderingTechnique::DefferedOnly && desc.mPipelineDesc.mDefferedPSOCreateInfo.mValid)
				pixelshader = desc.mPipelineDesc.mDefferedPSOCreateInfo.mPixelShader;
			else
			{
				NUCLEAR_INFO("[GraphicsEngine] ReflectShader Error: All Pipelines are invalid!");
				return false;
			}

			//just for reflection sake
			pixelshader.mDefines.insert("NE_DIR_LIGHTS_NUM 1");
			pixelshader.mDefines.insert("NE_SPOT_LIGHTS_NUM 1");
			pixelshader.mDefines.insert("NE_POINT_LIGHTS_NUM 1");
			pixelshader.mDefines.insert("NE_MAX_DIR_CASTERS 1");
			pixelshader.mDefines.insert("NE_MAX_SPOT_CASTERS 1");
			pixelshader.mDefines.insert("NE_MAX_OMNIDIR_CASTERS 1");

			pixelshader.mName = pixelshader.mName + "_ShaderReflection_TempObject";
			CreateShader(PShader.RawDblPtr(), pixelshader);
			if (!PShader)
			{
				return false;
			}
			int slot = 0;
			for (Uint32 i = 0; i < PShader->GetResourceCount(); i++)
			{
				Diligent::ShaderResourceDesc RsrcDesc;
				PShader->GetResourceDesc(i, RsrcDesc);

				if (RsrcDesc.Type == SHADER_RESOURCE_TYPE_TEXTURE_SRV)
				{
					std::string name = RsrcDesc.Name;
					if (name.find("NEMat_") != std::string::npos)
					{
						Assets::ShaderTexture ReflectedTex;
						ReflectedTex.mTex.pTexture = Fallbacks::FallbacksEngine::GetInstance().GetDefaultBlackImage();
						ReflectedTex.mTex.mUsageType = ParseTexUsageFromName(std::string(name).erase(0, 6));
						ReflectedTex.mSlot = slot;
						ReflectedTex.mName = name;
						ReflectedTex.mType = Assets::ShaderTextureType::MaterialTex;
						out.mMaterialTexturesInfo.push_back(ReflectedTex);
						slot++;
					}
				}
			}
			return true;
		}

		void GraphicsEngine::CreateShader(Diligent::IShader** result, const Graphics::ShaderObjectCreationDesc& desc)
		{
			Diligent::ShaderCreateInfo CreationAttribs;

			CreationAttribs.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
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

		Diligent::IShaderSourceInputStreamFactory* GraphicsEngine::GetDefaultShaderSourceFactory()
		{
			return pShaderSourceFactory;
		}

		void GraphicsEngine::InitPSOResources(Diligent::GraphicsPipelineStateCreateInfo& PSOCreateInfo, PSOResourcesInitInfo& desc)
		{
			if (desc.mResources.size() == 0)
			{
				ReflectShaderResources(PSOCreateInfo.pVS, desc);
				ReflectShaderResources(PSOCreateInfo.pPS, desc);
				ReflectShaderResources(PSOCreateInfo.pDS, desc);
				ReflectShaderResources(PSOCreateInfo.pHS, desc);
				ReflectShaderResources(PSOCreateInfo.pGS, desc);
				ReflectShaderResources(PSOCreateInfo.pAS, desc);
				ReflectShaderResources(PSOCreateInfo.pMS, desc);
			}

			if (desc.mStaticSamplers.size() == 0)
			{
				for (auto& i : desc.mTexturesSamplers)
				{
					bool match = false;
					for (auto& j : mDefaultStaticSamplers)
					{
						if (i.first.find(j.first) != std::string::npos)
						{
							Diligent::ImmutableSamplerDesc samdesc(i.second.ShaderStages, i.first.c_str() , j.second);
							desc.mStaticSamplers.push_back(samdesc);
							match = true;
						}
					}

					if (!match)
					{
						Diligent::ImmutableSamplerDesc samdesc(i.second.ShaderStages, i.first.c_str(), SamLinearWrapDesc);
						desc.mStaticSamplers.push_back(samdesc);
					}
				}
			}

			PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = static_cast<Uint32>(desc.mResources.size());
			PSOCreateInfo.PSODesc.ResourceLayout.Variables = desc.mResources.data();

			PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = static_cast<Uint32>(desc.mStaticSamplers.size());
			PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = desc.mStaticSamplers.data();
		}


		void GraphicsEngine::CreateShader(const std::string& source, Diligent::IShader** result, Diligent::SHADER_TYPE type)
		{
			Diligent::ShaderCreateInfo CreationAttribs;
			CreationAttribs.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
			CreationAttribs.Desc.UseCombinedTextureSamplers = true;
			CreationAttribs.Desc.ShaderType = type;
			CreationAttribs.EntryPoint = "main";
			CreationAttribs.Source = source.c_str();

			Graphics::Context::GetInstance().GetDevice()->CreateShader(CreationAttribs, result);
		}

		void GraphicsEngine::ReflectShaderResources(Diligent::IShader* Shader, PSOResourcesInitInfo& result)
		{
			if (Shader)
			{
				for (Uint32 i = 0; i < Shader->GetResourceCount(); i++)
				{
					Diligent::ShaderResourceDesc RsrcDesc;
					Shader->GetResourceDesc(i, RsrcDesc);

					Diligent::ShaderResourceVariableDesc Desc;
					Desc.Name = RsrcDesc.Name;
					Desc.Type = ParseNameToGetType(RsrcDesc.Name);
					Desc.ShaderStages = Shader->GetDesc().ShaderType;

					std::string name(RsrcDesc.Name);
					if (name.find("_sampler") != std::string::npos)
						result.mTexturesSamplers[name.erase(name.length() - 8)] = Desc;
					else					
						result.mResources.push_back(Desc);
					
				}
			}
		}

		Diligent::SHADER_RESOURCE_VARIABLE_TYPE GraphicsEngine::ParseNameToGetType(const std::string& name)
		{
			using namespace Diligent;
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
			else if (name.find("NE_FX_") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;
			else if (name.find("NEIBL") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;
			else if (name.find("NE_ShadowMap") == 0)
				return SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC;

			return SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
		}

		ShaderPipelineVariantFactory& GraphicsEngine::GetDefaultShaderPipelineVariantFactory()
		{
			return mDefaultVariantFactory;
		}

		GraphicsEngine::GraphicsEngine()
		{
			using namespace Diligent;

			{
				mRenderToTextureInputLayout.clear();
				mRenderToTextureInputLayout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));  //POS
				mRenderToTextureInputLayout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
			}

			{
				mRendering3D_InputLayout.clear();
				mRendering3D_InputLayout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));  //POS
				mRendering3D_InputLayout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));  //UV
				mRendering3D_InputLayout.push_back(LayoutElement(2, 0, 3, VT_FLOAT32, false));  //NORMAL
				mRendering3D_InputLayout.push_back(LayoutElement(3, 0, 3, VT_FLOAT32, false));  //Tangents
				mRendering3D_InputLayout.push_back(LayoutElement(4, 0, 3, VT_FLOAT32, false));  //Bitangents
				mRendering3D_InputLayout.push_back(LayoutElement(5, 0, 4, VT_INT32, false));    //BONE ID
				mRendering3D_InputLayout.push_back(LayoutElement(6, 0, 4, VT_FLOAT32, false));  //WEIGHT
			}

			SamLinearWrapDesc = SamplerDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
				TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP);

			SamPointBorderDesc = SamplerDesc(FILTER_TYPE_POINT, FILTER_TYPE_POINT, FILTER_TYPE_POINT,
				TEXTURE_ADDRESS_BORDER, TEXTURE_ADDRESS_BORDER, TEXTURE_ADDRESS_BORDER);

			SamLinearClampDesc = SamplerDesc(FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
				TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP);

			ShadowMapSamplerDesc = SamPointBorderDesc;
			ShadowMapSamplerDesc.BorderColor[0] = 1.0f;
			ShadowMapSamplerDesc.BorderColor[1] = 1.0f;
			ShadowMapSamplerDesc.BorderColor[2] = 1.0f;
			ShadowMapSamplerDesc.BorderColor[3] = 1.0f;
		}
	}
}