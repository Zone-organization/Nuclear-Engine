#include <Engine\Systems\RenderSystem.h>
#include <Engine\Graphics\Context.h>
#include <Core/Logger.h>
#include <Engine\Components/EntityInfoComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Scene.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Assets\Material.h>
#include <Engine\Systems\CameraSystem.h>
#include <Core\FileSystem.h>

namespace Nuclear
{
	namespace Systems
	{
		RenderSystem::RenderSystem()
		{
		}
		RenderSystem::~RenderSystem()
		{
		}

		bool RenderSystem::NeedsBaking()
		{
			if (mStatus.BakeLighting || mStatus.BakePipelines)
			{
				return true;
			}

			return false;
		}

		void RenderSystem::Bake(Uint32 RTWidth, Uint32 RTHeight, bool AllPipelines)
		{		
			mCameraSystemPtr = mScene->GetSystemManager().GetSystem<CameraSystem>();

			//mLightingSystem.UpdateBuffer(Math::Vector4(mCameraSystem.GetMainCamera()->GetPosition(), 1.0f));

			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_Animation";
			CBDesc.Size = sizeof(Math::Matrix4) * 100;
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &mAnimationCB);

			BakeScenePipeline(RTWidth, RTHeight);
			BakeLights();

			Graphics::ShaderRenderingBakingDesc bakedesc;
			bakedesc.DirLights = static_cast<Uint32>(GetDirLightsNum());
			bakedesc.SpotLights = static_cast<Uint32>(GetSpotLightsNum());
			bakedesc.PointLights = static_cast<Uint32>(GetPointLightsNum());
			bakedesc.CameraBufferPtr = mCameraSystemPtr->GetCameraCB();
			bakedesc.LightsBufferPtr = GetLightCB();
			bakedesc.pShadowPass = GetRenderPass<Rendering::ShadowPass>();
			bakedesc.AnimationBufferPtr = mAnimationCB;
			bakedesc.pIBLContext = pIBLContext;
			bakedesc.mRTWidth = RTWidth;
			bakedesc.mRTHeight = RTHeight;

			for (auto i : mRegisteredShaders)
			{
				if (i)
				{
					i->mPipeline.Bake(&bakedesc);
				}
			}

		}
		void RenderSystem::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{
			mRenderData.mFinalRT.Resize(RTWidth, RTHeight);
			mRenderData.mFinalDepthRT.Resize(RTWidth, RTHeight);

			for (auto pass : mRenderPasses)
			{
				pass->ResizeRTs(RTWidth, RTHeight);
			}

		}

		void RenderSystem::AddRenderPass(Rendering::RenderPass* pass)
		{
			//pass->Initialize();
			mRenderPasses.push_back(pass);
		}
		
		IBuffer* RenderSystem::GetAnimationCB()
		{
			return mAnimationCB;
		}

		CameraSystem* RenderSystem::GetCameraSystem()
		{
			return mCameraSystemPtr.get();
		}

		void RenderSystem::SetIBLContext(Rendering::ImageBasedLighting* IBLContext)
		{
			pIBLContext = IBLContext;
		}

		void RenderSystem::RegisterShader(Assets::Shader* shader)
		{
			mRegisteredShaders.push_back(shader);
		}

		void RenderSystem::Update(ECS::TimeDelta dt)
		{
			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 0: Update Entites
			//////////////////////////////////////////////////////////////////////////////////////////////
			UpdateLights();
			UpdateLightsBuffer(Math::Vector4(mCameraSystemPtr->GetMainCamera()->GetPosition(), 1.0f));

			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 1: Build FrameRenderData
			//////////////////////////////////////////////////////////////////////////////////////////////
			mRenderData.pScene = mScene;
			mRenderData.pAnimationCB = mAnimationCB;
			mRenderData.pCameraSystemPtr = mCameraSystemPtr;
			mRenderData.pCamera = mCameraSystemPtr->GetMainCamera();
			mRenderData.mUsedDefferedPipelines.clear();

			auto shadowpass = GetRenderPass<Rendering::ShadowPass>();
			if (shadowpass)
			{
				mRenderData.pDirPosShadowMapSRV = shadowpass->GetDirPosShadowMapSRV();
				mRenderData.pSpotPosShadowMapSRV = shadowpass->GetSpotShadowMapSRV();
				mRenderData.pOmniDirShadowMapSRV = shadowpass->GetOmniDirShadowMapSRV();

			}

			auto meshview = mScene->GetRegistry().view<Components::MeshComponent>();
			for (auto entity : meshview)
			{
				auto& mesh = meshview.get<Components::MeshComponent>(entity);
				mesh.Update();
			}

			//sort accroding to shadingmodels (aka pipelines) & update their renderqueues
			mScene->GetRegistry().sort<Nuclear::Components::MeshComponent>([](const auto& lhs, const auto& rhs)
				{
					return lhs.GetRenderQueue() < rhs.GetRenderQueue();
				});

			mRenderData.mMeshView = mScene->GetRegistry().view<Components::MeshComponent>();

			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 2: Clear main RTVs
			//////////////////////////////////////////////////////////////////////////////////////////////
			Graphics::Context::GetContext()->SetRenderTargets(1, mRenderData.mFinalRT.GetRTVDblPtr(), mRenderData.mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(mRenderData.mFinalRT.GetRTV(), (float*)&mCameraSystemPtr->GetMainCamera()->RTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(mRenderData.mFinalDepthRT.GetRTV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 3: Update RenderPasses
			//////////////////////////////////////////////////////////////////////////////////////////////
			for (auto pass : mRenderPasses)
			{
				pass->Update(&mRenderData);
			}


			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 4: RenderScene to screen
			//////////////////////////////////////////////////////////////////////////////////////////////
			auto* RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
			auto* DSV = Graphics::Context::GetSwapChain()->GetDepthBufferDSV();
			Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(RTV, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Graphics::Context::GetContext()->SetPipelineState(pSceneToScreenPSO);
			pSceneToScreenSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(mRenderData.mFinalRT.GetSRV());
			Graphics::Context::GetContext()->CommitShaderResources(pSceneToScreenSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Assets::DefaultMeshes::RenderScreenQuad();
		}

		void RenderSystem::BakeLightsBuffer()
		{
			if (HasbeenBakedBefore)
			{
				if (!LightRequiresBaking())
				{
					NUCLEAR_WARN("[LightingSystem] No need for baking the sub system!");
				}
			}

			HasbeenBakedBefore = true;

			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
			Baked_DirLights_Size = mRenderData.DirLights.size();
			Baked_PointLights_Size = mRenderData.PointLights.size();
			Baked_SpotLights_Size = mRenderData.SpotLights.size();

			if (mRenderData.DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (mRenderData.DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (mRenderData.DirLights.size() * 2);
			}
			if (mRenderData.PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (mRenderData.PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (mRenderData.PointLights.size() * 3);
			}
			if (mRenderData.SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (mRenderData.SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (mRenderData.SpotLights.size() * 5);
			}

			if (mPSLightCB.RawPtr() != nullptr)
			{
				mPSLightCB.Release();
			}
			BufferDesc CBDesc;
			CBDesc.Name = "LightCB";
			CBDesc.Size = static_cast<Uint32>(NE_Light_CB_Size);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			BufferData DATA;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, &DATA, mPSLightCB.RawDblPtr());
		}

		void RenderSystem::BakeScenePipeline(Uint32 RTWidth, Uint32 RTHeight)
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			mRenderData.mFinalRT.Create(RTDesc);

			RTDesc.DepthTexFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			mRenderData.mFinalDepthRT.Create(RTDesc);

			GraphicsPipelineStateCreateInfo PSOCreateInfo;

			PSOCreateInfo.PSODesc.Name = "SceneToScreen PSO";
			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

			//Create Shaders
			RefCntAutoPtr<IShader> VSShader;
			RefCntAutoPtr<IShader> PSShader;

			std::vector<LayoutElement> LayoutElems = Graphics::GraphicsEngine::GetShaderManager()->GetBasicVSLayout(true);

			//Create Vertex Shader
			{

				ShaderCreateInfo CreationAttribs;

				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "SceneToScreenVS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/BasicVertex.vs.hlsl", std::set<std::string>(), std::set<std::string>(), true);
				CreationAttribs.Source = source.c_str();
				RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
				Graphics::Context::GetEngineFactory()->CreateDefaultShaderSourceStreamFactory("Assets/NuclearEngine/Shaders/", &pShaderSourceFactory);
				CreationAttribs.pShaderSourceStreamFactory = pShaderSourceFactory;
				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, VSShader.RawDblPtr());
			}

			//Create Pixel Shader
			{
				ShaderCreateInfo CreationAttribs;

				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.EntryPoint = "main";
				CreationAttribs.Desc.Name = "SceneToScreenPS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/SceneToScreen.ps.hlsl", std::set<std::string>(), std::set<std::string>(), true);
				CreationAttribs.Source = source.c_str();
				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, PSShader.RawDblPtr());
			}

			PSOCreateInfo.pVS = VSShader;
			PSOCreateInfo.pPS = PSShader;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
			auto Vars = Graphics::GraphicsEngine::GetShaderManager()->ReflectShaderVariables(VSShader, PSShader);
			Graphics::GraphicsEngine::GetShaderManager()->ProcessAndCreatePipeline(&pSceneToScreenPSO, PSOCreateInfo, Vars, true);
			pSceneToScreenPSO->CreateShaderResourceBinding(pSceneToScreenSRB.RawDblPtr());
		}

		void RenderSystem::BakeLights()
		{
			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();

			std::vector<Components::DirLightComponent*> DirLights_noShadows;
			std::vector<Components::PointLightComponent*> PointLights_noShadows;
			std::vector<Components::SpotLightComponent*> SpotLights_noShadows;

			//Shadows Enabled First
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);
				if (DirLight.mCastShadows)
					mRenderData.DirLights.push_back(&DirLight);
				else
					DirLights_noShadows.push_back(&DirLight);
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				if (SpotLight.mCastShadows)
					mRenderData.SpotLights.push_back(&SpotLight);
				else
					SpotLights_noShadows.push_back(&SpotLight);
			}

			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				if (PointLight.mCastShadows)
					mRenderData.PointLights.push_back(&PointLight);
				else
					PointLights_noShadows.push_back(&PointLight);
			}

			//append the non shadowed vectors
			mRenderData.DirLights.insert(mRenderData.DirLights.end(), DirLights_noShadows.begin(), DirLights_noShadows.end());
			mRenderData.SpotLights.insert(mRenderData.SpotLights.end(), SpotLights_noShadows.begin(), SpotLights_noShadows.end());
			mRenderData.PointLights.insert(mRenderData.PointLights.end(), PointLights_noShadows.begin(), PointLights_noShadows.end());

			BakeLightsBuffer();
		}

		void RenderSystem::UpdateLights()
		{
			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				DirLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				SpotLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
			}


			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
			}
		
		}
		void RenderSystem::UpdateLightsBuffer(const Math::Vector4& CameraPos)
		{
			std::vector<Math::Vector4> LightsBuffer;
			LightsBuffer.reserve(NUM_OF_LIGHT_VECS + 1);

			LightsBuffer.push_back(CameraPos);

			for (size_t i = 0; i < mRenderData.DirLights.size(); i++)
			{
				LightsBuffer.push_back(mRenderData.DirLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(mRenderData.DirLights[i]->GetInternalData().Color_Intensity);
			}
			for (size_t i = 0; i < mRenderData.PointLights.size(); i++)
			{
				LightsBuffer.push_back(mRenderData.PointLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(mRenderData.PointLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(mRenderData.PointLights[i]->GetInternalData().Color_FarPlane);

			}
			for (size_t i = 0; i < mRenderData.SpotLights.size(); i++)
			{
				LightsBuffer.push_back(mRenderData.SpotLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(mRenderData.SpotLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(mRenderData.SpotLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(mRenderData.SpotLights[i]->GetInternalData().InnerCutOf_OuterCutoff);
				LightsBuffer.push_back(mRenderData.SpotLights[i]->GetInternalData().Color);
			}

			PVoid data;
			Graphics::Context::GetContext()->MapBuffer(mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
			data = memcpy(data, LightsBuffer.data(), NE_Light_CB_Size);
			Graphics::Context::GetContext()->UnmapBuffer(mPSLightCB, MAP_WRITE);
		}
		bool RenderSystem::LightRequiresBaking()
		{
			if (Baked_DirLights_Size == mRenderData.DirLights.size()
				&& Baked_PointLights_Size == mRenderData.PointLights.size()
				&& Baked_SpotLights_Size == mRenderData.SpotLights.size())
				return true;

			return false;
		}
		IBuffer* RenderSystem::GetLightCB()
		{
			return mPSLightCB.RawPtr();
		}
		size_t RenderSystem::GetDirLightsNum()
		{
			return mRenderData.DirLights.size();
		}
		size_t RenderSystem::GetPointLightsNum()
		{
			return mRenderData.PointLights.size();
		}
		size_t RenderSystem::GetSpotLightsNum()
		{
			return mRenderData.SpotLights.size();
		}
	}
}