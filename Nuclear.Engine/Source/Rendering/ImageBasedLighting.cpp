#include "Rendering/ImageBasedLighting.h"
#include <Graphics/GraphicsModule.h>
#include <Platform\FileSystem.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Graphics/ImGui.h>
#include <Assets/DefaultMeshes.h>
#include <array>
#include "Diligent/Common/interface/BasicMath.hpp"
#include <Graphics/GraphicsModule.h>

namespace Nuclear
{
	namespace Rendering
	{
		using namespace Diligent;
		void ImageBasedLighting::Initialize(const ImageBasedLightingDesc& desc)
		{
			mDesc = desc;
			//Common
			RefCntAutoPtr<IShader> CubeVSShader;
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.Desc.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "CubemapSample_VS";

				auto source = Platform::FileSystem::Get().LoadShader("@NuclearAssets@/Shaders/PBR/CubemapSample.vs.hlsl", std::set<std::string>());
				CreationAttribs.Source = source.c_str();
				CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsModule::Get().GetDefaultShaderSourceFactory();


				Graphics::GraphicsModule::Get().GetDevice()->CreateShader(CreationAttribs, &CubeVSShader);
			}

			//Create Constant buffer
			{
				BufferDesc CBDesc;
				CBDesc.Name = "CaptureInfo_CB";
				CBDesc.Size = sizeof(Math::Matrix4);
				CBDesc.Usage = USAGE_DYNAMIC;
				CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
				CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
				Graphics::GraphicsModule::Get().GetDevice()->CreateBuffer(CBDesc, nullptr, &pCaptureCB);

				CBDesc.Name = "Roughness_CB";
				CBDesc.Size = sizeof(Math::Vector4);

				Graphics::GraphicsModule::Get().GetDevice()->CreateBuffer(CBDesc, nullptr, &pPrefilterRoughnessCB);
			}

			//Create ERectToCubemap PSO
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "ERectToCubemap_PSO";

				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = mDesc.mIrradianceCubeFmt;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_UNKNOWN;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				//Create Shaders
				RefCntAutoPtr<IShader> PSShader;

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.Desc.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "ERectToCubemap_PS";

					auto source = Platform::FileSystem::Get().LoadShader("@NuclearAssets@/Shaders/PBR/EquirectangularToCubemap.ps.hlsl", std::set<std::string>());
					CreationAttribs.Source = source.c_str();
					CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsModule::Get().GetDefaultShaderSourceFactory();

					Graphics::GraphicsModule::Get().GetDevice()->CreateShader(CreationAttribs, &PSShader);
				}

				PSOCreateInfo.pVS = CubeVSShader;
				PSOCreateInfo.pPS = PSShader;


				Graphics::PSOResourcesInitInfo ResourcesInitinfo;
				Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

				Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &pERectToCubemap_PSO);


				pERectToCubemap_PSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_CaptureInfo")->Set(pCaptureCB);

				pERectToCubemap_PSO->CreateShaderResourceBinding(&pERectToCubemap_SRB, true);

			}

			//Create PrecomputeIrradiance PSO
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "PrecomputeIrradiance_PSO";

				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = mDesc.mIrradianceCubeFmt;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_UNKNOWN;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				//Create Shaders
				RefCntAutoPtr<IShader> PSShader;

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.Desc.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "PrecomputeIrradiance_PS";

					auto source = Platform::FileSystem::Get().LoadShader("@NuclearAssets@/Shaders/PBR/IrradianceConvolution.ps.hlsl", std::set<std::string>());
					CreationAttribs.Source = source.c_str();
					CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsModule::Get().GetDefaultShaderSourceFactory();

					Graphics::GraphicsModule::Get().GetDevice()->CreateShader(CreationAttribs, &PSShader);
				}

				PSOCreateInfo.pVS = CubeVSShader;
				PSOCreateInfo.pPS = PSShader;

				Graphics::PSOResourcesInitInfo ResourcesInitinfo;
				Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

				Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &pPrecomputeIrradiancePSO);

				pPrecomputeIrradiancePSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_CaptureInfo")->Set(pCaptureCB);
				pPrecomputeIrradiancePSO->CreateShaderResourceBinding(&pPrecomputeIrradiance_SRB, true);
			}

			//Create PrecomputePrefilter PSO
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "PrecomputePrefilter_PSO";

				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = mDesc.mPrefilteredEnvMapFmt;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = TEX_FORMAT_UNKNOWN;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				//Create Shaders
				RefCntAutoPtr<IShader> PSShader;

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.Desc.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "PrecomputePrefilter_PS";

					auto source = Platform::FileSystem::Get().LoadShader("@NuclearAssets@/Shaders/PBR/PrefilterConvolution.ps.hlsl", std::set<std::string>());
					CreationAttribs.Source = source.c_str();
					CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsModule::Get().GetDefaultShaderSourceFactory();

					Graphics::GraphicsModule::Get().GetDevice()->CreateShader(CreationAttribs, &PSShader);
				}

				PSOCreateInfo.pVS = CubeVSShader;
				PSOCreateInfo.pPS = PSShader;

				Graphics::PSOResourcesInitInfo ResourcesInitinfo;
				Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

				Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &pPrecomputePrefilterPSO);


				pPrecomputePrefilterPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_CaptureInfo")->Set(pCaptureCB);
				pPrecomputePrefilterPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "NEStatic_Roughness")->Set(pPrefilterRoughnessCB);
				pPrecomputePrefilterPSO->CreateShaderResourceBinding(&pPrecomputePrefilter_SRB, true);
			}

			//Create PrecomputeBRDF PSO
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "PrecomputeBRDF_PSO";

				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = TEX_FORMAT_RG16_FLOAT;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

				//Create Shaders
				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;

				//Create Vertex Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.Desc.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "PrecomputeBRDF_VS";

					auto source = Platform::FileSystem::Get().LoadShader("@NuclearAssets@/Shaders/FullScreenTriangle.vs.hlsl", std::set<std::string>());
					CreationAttribs.Source = source.c_str();
					CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsModule::Get().GetDefaultShaderSourceFactory();

					Graphics::GraphicsModule::Get().GetDevice()->CreateShader(CreationAttribs, &VSShader);
				}

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.Desc.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "PrecomputeBRDF_PS";

					auto source = Platform::FileSystem::Get().LoadShader("@NuclearAssets@/Shaders/PBR/BRDF.ps.hlsl", std::set<std::string>());
					CreationAttribs.Source = source.c_str();
					CreationAttribs.pShaderSourceStreamFactory = Graphics::GraphicsModule::Get().GetDefaultShaderSourceFactory();

					Graphics::GraphicsModule::Get().GetDevice()->CreateShader(CreationAttribs, &PSShader);
				}

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;

				Graphics::PSOResourcesInitInfo ResourcesInitinfo;
				Graphics::GraphicsModule::Get().InitPSOResources(PSOCreateInfo, ResourcesInitinfo);

				Graphics::GraphicsModule::Get().GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &pPrecomputeBRDF_PSO);

				pPrecomputeBRDF_PSO->CreateShaderResourceBinding(&pPrecomputeBRDF_SRB, true);
			}
			//IBL: generate a 2D LUT from the BRDF equations used.
			RefCntAutoPtr<ITexture> pBRDF_LUT;
			RefCntAutoPtr<ITextureView> pBRDF_LUT_RTV;

			{
				TextureDesc TexDesc;
				TexDesc.Name = "BRDF_LUT";
				TexDesc.Type = RESOURCE_DIM_TEX_2D;
				TexDesc.Usage = USAGE_DEFAULT;
				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
				TexDesc.Width = mDesc.mBRDF_LUTDim;
				TexDesc.Height = mDesc.mBRDF_LUTDim;
				TexDesc.Format = TEX_FORMAT_RG16_FLOAT;
				TexDesc.MipLevels = 1;

				Graphics::GraphicsModule::Get().GetDevice()->CreateTexture(TexDesc, nullptr, &pBRDF_LUT);
			}
			mBRDF_LUT_Image.SetTextureView(pBRDF_LUT->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
			pBRDF_LUT_RTV = pBRDF_LUT->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET);

			{
				Graphics::GraphicsModule::Get().GetContext()->SetPipelineState(pPrecomputeBRDF_PSO.RawPtr());

				Graphics::GraphicsModule::Get().GetContext()->SetRenderTargets(1, pBRDF_LUT_RTV.RawDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::GraphicsModule::Get().GetContext()->ClearRenderTarget(pBRDF_LUT_RTV.RawPtr(), NULL, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				Graphics::GraphicsModule::Get().GetContext()->CommitShaderResources(pPrecomputeBRDF_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				DrawAttribs attrs(3, DRAW_FLAG_VERIFY_ALL);
				Graphics::GraphicsModule::Get().GetContext()->Draw(attrs);
			}
		}
		const std::array<float4x4, 6> Matrices =
		{
			/* +X */ float4x4::RotationY(+PI_F / 2.f),
			/* -X */ float4x4::RotationY(-PI_F / 2.f),
			/* +Y */ float4x4::RotationX(-PI_F / 2.f),
			/* -Y */ float4x4::RotationX(+PI_F / 2.f),
			/* +Z */ float4x4::Identity(),
			/* -Z */ float4x4::RotationY(PI_F)
		};

		Assets::Texture ImageBasedLighting::EquirectangularToCubemap(Assets::MaterialTexture* HDR_Env)
		{
			RefCntAutoPtr<ITexture> TexCubeRTs;

			// IBL: setup cubemap to render to
			{
				//Create TexCube
				TextureDesc TexDesc;
				TexDesc.Name = "ERectToCubemap_Tex";
				TexDesc.Type = RESOURCE_DIM_TEX_CUBE;
				TexDesc.Usage = USAGE_DEFAULT;
				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
				TexDesc.Width = 512;
				TexDesc.Height = 512;
				TexDesc.Format = TEX_FORMAT_RGBA32_FLOAT;
				TexDesc.ArraySize = 6;
				TexDesc.MipLevels = 1;

				Graphics::GraphicsModule::Get().GetDevice()->CreateTexture(TexDesc, nullptr, &TexCubeRTs);
			}
		
			// IBL: convert HDR equirectangular environment map to cubemap equivalent
			{
				Graphics::GraphicsModule::Get().GetContext()->SetPipelineState(pERectToCubemap_PSO.RawPtr());
				pERectToCubemap_SRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(HDR_Env->pTexture->GetTextureView());

				float col[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

				for (unsigned int i = 0; i < 6; ++i)
				{
					{
						Diligent::MapHelper<float4x4> CBConstants(Graphics::GraphicsModule::Get().GetContext(), pCaptureCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = Matrices[i];
					}
					TextureViewDesc RTVDesc{ "ERect_Cube_RTV", TEXTURE_VIEW_RENDER_TARGET, RESOURCE_DIM_TEX_2D_ARRAY };
					RTVDesc.MostDetailedMip = 0;
					RTVDesc.FirstArraySlice = i;
					RTVDesc.NumArraySlices = 1;
					RefCntAutoPtr<ITextureView> pRTV;
					TexCubeRTs->CreateView(RTVDesc, &pRTV);


					Graphics::GraphicsModule::Get().GetContext()->SetRenderTargets(1, pRTV.RawDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					Graphics::GraphicsModule::Get().GetContext()->ClearRenderTarget(pRTV.RawPtr(), col, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

					Graphics::GraphicsModule::Get().GetContext()->CommitShaderResources(pERectToCubemap_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

					DrawAttribs drawAttrs(4, DRAW_FLAG_VERIFY_ALL);
					Graphics::GraphicsModule::Get().GetContext()->Draw(drawAttrs);
				}
			}

			Assets::Texture result;
			result.SetTextureView(TexCubeRTs->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
			return result;
		}

		Rendering::PBRCapture ImageBasedLighting::PrecomputePBRCapture(Assets::Texture* cubemap)
		{
			// IBL: create an irradiance cubemap.
			RefCntAutoPtr<ITexture> pIrradianceTex;
			{
				TextureDesc TexDesc;
				TexDesc.Name = "PBRCapture_Irradiance";
				TexDesc.Type = RESOURCE_DIM_TEX_CUBE;
				TexDesc.Usage = USAGE_DEFAULT;
				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
				TexDesc.Width = mDesc.mIrradianceCubeDim;
				TexDesc.Height = mDesc.mIrradianceCubeDim;
				TexDesc.Format = mDesc.mIrradianceCubeFmt;
				TexDesc.ArraySize = 6;
				TexDesc.MipLevels = 1;

				Graphics::GraphicsModule::Get().GetDevice()->CreateTexture(TexDesc, nullptr, &pIrradianceTex);
			}

			//IBL: Precompute Irradiance
			{
				Graphics::GraphicsModule::Get().GetContext()->SetPipelineState(pPrecomputeIrradiancePSO.RawPtr());

				float col[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
				pPrecomputeIrradiance_SRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(cubemap->GetTextureView());

				for (unsigned int i = 0; i < 6; ++i)
				{
					{
						Diligent::MapHelper<float4x4> CBConstants(Graphics::GraphicsModule::Get().GetContext(), pCaptureCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = Matrices[i];
					}
					TextureViewDesc RTVDesc{ "IrradianceConvolution_RTV", TEXTURE_VIEW_RENDER_TARGET, RESOURCE_DIM_TEX_2D_ARRAY };
					RTVDesc.MostDetailedMip = 0;
					RTVDesc.FirstArraySlice = i;
					RTVDesc.NumArraySlices = 1;
					RefCntAutoPtr<ITextureView> pRTV;
					pIrradianceTex->CreateView(RTVDesc, &pRTV);


					Graphics::GraphicsModule::Get().GetContext()->SetRenderTargets(1, pRTV.RawDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					Graphics::GraphicsModule::Get().GetContext()->ClearRenderTarget(pRTV.RawPtr(), col, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

					Graphics::GraphicsModule::Get().GetContext()->CommitShaderResources(pPrecomputeIrradiance_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					DrawAttribs drawAttrs(4, DRAW_FLAG_VERIFY_ALL);
					Graphics::GraphicsModule::Get().GetContext()->Draw(drawAttrs);
				}
			}

			// IBL: create an prefilter cubemap.
			RefCntAutoPtr<ITexture> pPrefilterTex;
			{
				TextureDesc TexDesc;
				TexDesc.Name = "PBRCapture_Prefiltered";
				TexDesc.Type = RESOURCE_DIM_TEX_CUBE;
				TexDesc.Usage = USAGE_DEFAULT;
				TexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
				TexDesc.Width = mDesc.mPrefilteredEnvMapDim;
				TexDesc.Height = mDesc.mPrefilteredEnvMapDim;
				TexDesc.Format = mDesc.mPrefilteredEnvMapFmt;
				TexDesc.ArraySize = 6;
				TexDesc.MipLevels = 0;

				Graphics::GraphicsModule::Get().GetDevice()->CreateTexture(TexDesc, nullptr, &pPrefilterTex);
			}


			//IBL: Precompute Prefilter
			{
				Graphics::GraphicsModule::Get().GetContext()->SetPipelineState(pPrecomputePrefilterPSO.RawPtr());

				float col[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
				pPrecomputePrefilter_SRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(cubemap->GetTextureView());

				const auto& PrefilteredEnvMapDesc = pPrefilterTex->GetDesc();
				for (Uint32 mip = 0; mip < PrefilteredEnvMapDesc.MipLevels; ++mip)
				{

					float roughness = (float)mip / (float)(PrefilteredEnvMapDesc.MipLevels);

					{
						Diligent::MapHelper<Math::Vector4> CBConstants(Graphics::GraphicsModule::Get().GetContext(), pPrefilterRoughnessCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = Math::Vector4(roughness);
					}

					for (unsigned int face = 0; face < 6; ++face)
					{
						{
							Diligent::MapHelper<float4x4> CBConstants(Graphics::GraphicsModule::Get().GetContext(), pCaptureCB, MAP_WRITE, MAP_FLAG_DISCARD);
							*CBConstants = Matrices[face];
						}
						TextureViewDesc RTVDesc{ "PrefilterConvolution_RTV", TEXTURE_VIEW_RENDER_TARGET, RESOURCE_DIM_TEX_2D_ARRAY };
						RTVDesc.MostDetailedMip = mip;
						RTVDesc.FirstArraySlice = face;
						RTVDesc.NumArraySlices = 1;
						RefCntAutoPtr<ITextureView> pRTV;
						pPrefilterTex->CreateView(RTVDesc, &pRTV);


						Graphics::GraphicsModule::Get().GetContext()->SetRenderTargets(1, pRTV.RawDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
						Graphics::GraphicsModule::Get().GetContext()->ClearRenderTarget(pRTV.RawPtr(), col, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

						Graphics::GraphicsModule::Get().GetContext()->CommitShaderResources(pPrecomputePrefilter_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
						DrawAttribs drawAttrs(4, DRAW_FLAG_VERIFY_ALL);
						Graphics::GraphicsModule::Get().GetContext()->Draw(drawAttrs);
					}
				}
			}
			Rendering::PBRCapture result;

			result.mIrradiance.SetTextureView(pIrradianceTex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
			result.mPrefiltered.SetTextureView(pPrefilterTex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE));
			return result;
		}

		void ImageBasedLighting::SetEnvironmentCapture(Rendering::PBRCapture* cap)
		{
			pEnvCapture = cap;
		}

		Rendering::PBRCapture* ImageBasedLighting::GetEnvironmentCapture()
		{
			return pEnvCapture;
		}
		Assets::Texture* ImageBasedLighting::GetBRDF_LUT()
		{
			return &mBRDF_LUT_Image;
		}

	}
}