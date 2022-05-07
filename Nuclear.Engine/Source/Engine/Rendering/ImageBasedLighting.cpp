#include "Engine/Rendering/ImageBasedLighting.h"
#include <Engine\Graphics\Context.h>
#include <Core\FileSystem.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine/Graphics/ImGui.h>
namespace Nuclear
{
	namespace Rendering
	{
		void ImageBasedLighting::Initialize(const ImageBasedLightingDesc& desc)
		{
			mDesc = desc;

			//Create Capture CB
			{
				BufferDesc CBDesc;
				CBDesc.Name = "CaptureInfo_CB";
				CBDesc.Size = sizeof(Math::Matrix4) * 2;
				CBDesc.Usage = USAGE_DYNAMIC;
				CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
				CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
				Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &pCaptureCB);
			}

			//Create ERectToCubemap PSO
			{
				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "ERectToCubemap_PSO";

				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

				//Create Shaders
				RefCntAutoPtr<IShader> VSShader;
				RefCntAutoPtr<IShader> PSShader;
				std::vector<LayoutElement> LayoutElems;
				LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS

				//Create Vertex Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "ERectToCubemap_VS";

					auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PBR/Cubemap.vs.hlsl", std::vector<std::string>());
					CreationAttribs.Source = source.c_str();
					RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
					Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/PBR/", &pShaderSourceFactory);
					CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;

					Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VSShader);
				}

				//Create Pixel Shader
				{
					ShaderCreateInfo CreationAttribs;
					CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
					CreationAttribs.UseCombinedTextureSamplers = true;
					CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
					CreationAttribs.EntryPoint = "main";
					CreationAttribs.Desc.Name = "ERectToCubemap_PS";

					auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PBR/EquirectangularToCubemap.ps.hlsl", std::vector<std::string>());
					CreationAttribs.Source = source.c_str();
					RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
					Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/PBR/", &pShaderSourceFactory);
					CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;

					Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PSShader);
				}

				PSOCreateInfo.pVS = VSShader;
				PSOCreateInfo.pPS = PSShader;
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
				auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
				Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&pERectToCubemap_PSO, PSOCreateInfo, Vars, true);

				pERectToCubemap_PSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "NEStatic_CaptureInfo")->Set(pCaptureCB);

				pERectToCubemap_PSO->CreateShaderResourceBinding(&pERectToCubemap_SRB, true);

			}

			//Create CaptureRT
			{
				Graphics::RenderTargetDesc RTDesc;
				RTDesc.Width = 512;
				RTDesc.Height = 512;
				//mCaptureRT.Create(RTDesc);


				RTDesc.DepthTexFormat = TEX_FORMAT_D24_UNORM_S8_UINT;
				mCaptureDepthRT.Create(RTDesc);
			}


			//Setup matrices
			{
				mCaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
				mCaptureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
				mCaptureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
				mCaptureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				mCaptureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				mCaptureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
				mCaptureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			}
			mSkybox.InitializeCube();
		}

		struct PS_CaptureInfo
		{
			Math::Matrix4 view;
			Math::Matrix4 projection;
		};
		Rendering::PBRCapture ImageBasedLighting::EquirectangularToCubemap(Graphics::Texture* HDR_Env)
		{
			Rendering::PBRCapture result;
			RefCntAutoPtr<ITexture> TexCubeRTs;

			// IBL: setup cubemap to render to and attach to rendertarget
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

				Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &TexCubeRTs);
			}
		
			// IBL: convert HDR equirectangular environment map to cubemap equivalent
			{
				Graphics::Context::GetContext()->SetPipelineState(pERectToCubemap_PSO.RawPtr());
				//	Graphics::Context::GetContext()->SetRenderTargets(1, mCaptureRT.GetMainRTVDblPtr(), mCaptureDepthRT.GetMainRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				//	Graphics::Context::GetContext()->ClearRenderTarget(mCaptureRT.GetMainRTV(), NULL, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

				pERectToCubemap_SRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(HDR_Env->GetImage()->mTextureView.RawPtr());

				PS_CaptureInfo cbdata;
				cbdata.projection = mCaptureProjection;

				Graphics::Context::GetContext()->CommitShaderResources(pERectToCubemap_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				
				for (unsigned int i = 0; i < 6; ++i)
				{
					cbdata.view = mCaptureViews[i];
					{
						Diligent::MapHelper<PS_CaptureInfo> CBConstants(Graphics::Context::GetContext(), pCaptureCB, MAP_WRITE, MAP_FLAG_DISCARD);
						*CBConstants = cbdata;
					}
					TextureViewDesc RTVDesc{ "RTV", TEXTURE_VIEW_RENDER_TARGET, RESOURCE_DIM_TEX_2D_ARRAY };
					RTVDesc.MostDetailedMip = 0;
					RTVDesc.FirstArraySlice = i;
					RTVDesc.NumArraySlices = 1;
					RefCntAutoPtr<ITextureView> pRTV;
					TexCubeRTs->CreateView(RTVDesc, &pRTV);


					Graphics::Context::GetContext()->SetRenderTargets(1, pRTV.RawDblPtr(), nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

					mSkybox.RenderCube();
				}

			//	mSkybox.mTextureSRV = TexCubeRTs->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
			//	mSkybox.InitializePipeline(mDesc.cameraCB);
			}

			// IBL: create an irradiance cubemap, and re-scale captureRT to irradiance scale.
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
				TexDesc.MipLevels = 0;

				Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &result.mIrradiance);

				/*TexDesc.Name = "PBRCapture_Prefiltered";
				TexDesc.Width = mDesc.mPrefilteredEnvMapDim;
				TexDesc.Height = mDesc.mPrefilteredEnvMapDim;
				TexDesc.Format = mDesc.mPrefilteredEnvMapFmt;

				Graphics::Context::GetDevice()->CreateTexture(TexDesc, nullptr, &result.mPrefiltered);*/


				//mCaptureDepthRT.Resize(mDesc.mIrradianceCubeDim, mDesc.mIrradianceCubeDim);
			}

			return result;
		}
		void ImageBasedLighting::TestRender(Rendering::PBRCapture* cap)
		{
			mSkybox.Render();
		}
	}
}