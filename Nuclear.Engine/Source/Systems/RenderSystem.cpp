#include <Systems\RenderSystem.h>
#include <Graphics\Context.h>
#include <Utilities/Logger.h>
#include <Components/EntityInfoComponent.h>
#include <Components\CameraComponent.h>
#include <Components\MeshComponent.h>
#include <Graphics\GraphicsEngine.h>
#include <Assets\AssetManager.h>
#include <Core\Scene.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Assets\Material.h>
#include <Platform\FileSystem.h>
#include <Rendering\RenderPasses\DefferedPass.h>
#include <Rendering\RenderingEngine.h>

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

		void RenderSystem::Bake(const RenderSystemBakingDesc& desc)
		{								
			BakeLights();

			Graphics::ShaderRenderingBakingDesc bakedesc;
			bakedesc.DirLights = static_cast<Uint32>(GetDirLightsNum());
			bakedesc.SpotLights = static_cast<Uint32>(GetSpotLightsNum());
			bakedesc.PointLights = static_cast<Uint32>(GetPointLightsNum());
			bakedesc.LightsBufferPtr = GetLightCB();
			bakedesc.pShadowPass = GetRenderPass<Rendering::ShadowPass>();
			bakedesc.pIBLContext = pIBLContext;
			bakedesc.mRTWidth = desc.RTWidth;
			bakedesc.mRTHeight = desc.RTHeight;

			if (GetRenderPass<Rendering::DefferedPass>())
			{
				bakedesc.mRenderSystemHasDefferedPass = true;
			}

			if (GetRenderPass<Rendering::ShadowPass>())
			{
				bakedesc.mRenderSystemHasShadowPass = true;
			}


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
			Math::Vector2ui newsize(RTWidth, RTHeight);

			for (auto pass : mRenderPasses)
			{
				pass->ResizeRTs(RTWidth, RTHeight);
			}

			for (auto i : mRegisteredShaders)
			{
				if (i)
				{
					if (i->mPipeline.GetGBuffer()->GetDimensions() != newsize)
					{
						i->mPipeline.GetGBuffer()->Resize(newsize);
					}
				}
			}
		}

		void RenderSystem::AddRenderPass(Rendering::RenderPass* pass)
		{
			mRenderPasses.push_back(pass);
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
			//Step 1: Update Entites
			//////////////////////////////////////////////////////////////////////////////////////////////
			UpdateLights();
			bool hasdefpasss = false, hasshadowpass = false;
			if (GetRenderPass<Rendering::DefferedPass>())
			{
				hasdefpasss = true;
			}

			auto shadowpass = GetRenderPass<Rendering::ShadowPass>();
			if (shadowpass)
			{
				mRenderData.pDirPosShadowMapSRV = shadowpass->GetDirPosShadowMapSRV();
				mRenderData.pSpotPosShadowMapSRV = shadowpass->GetSpotShadowMapSRV();
				mRenderData.pOmniDirShadowMapSRV = shadowpass->GetOmniDirShadowMapSRV();
				hasshadowpass = true;
			}

			auto meshview = Core::Scene::GetInstance().GetRegistry().view<Components::MeshComponent>();
			for (auto entity : meshview)
			{
				auto& mesh = meshview.get<Components::MeshComponent>(entity);
				mesh.SetRenderSystemFlags(hasdefpasss, hasshadowpass);
				mesh.Update();
			}

			//sort accroding to shadingmodels (aka pipelines) & update their renderqueues
			Core::Scene::GetInstance().GetRegistry().sort<Nuclear::Components::MeshComponent>([](const auto& lhs, const auto& rhs)
				{
					return lhs.GetRenderQueue() < rhs.GetRenderQueue();
				});

			mRenderData.mMeshView = Core::Scene::GetInstance().GetRegistry().view<Components::MeshComponent>();

			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 2: Render scene from each camera pov
			//////////////////////////////////////////////////////////////////////////////////////////////
			auto CameraView = Core::Scene::GetInstance().GetRegistry().view<Components::CameraComponent>();
			for (auto entity : CameraView)
			{
				auto& camera = CameraView.get<Components::CameraComponent>(entity);
				if (camera.GetIsActive())
				{
					UpdateLightsBuffer(Math::Vector4(camera.GetPosition(), 1.0f));

					////////////////////////////////////
					//Step 2.1: Build FrameRenderData
					////////////////////////////////////
					mRenderData.pCamera = &camera;
					mRenderData.mUsedDefferedPipelines.clear();

					////////////////////////////////////
					//Step 2.2: Clear main RTVs
					////////////////////////////////////
					Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, camera.GetRenderTarget().GetRTVDblPtr(), camera.GetDepthRenderTarget().GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(camera.GetRenderTarget().GetRTV(), (float*)&camera.mRTClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
					Graphics::Context::GetInstance().GetContext()->ClearDepthStencil(camera.GetDepthRenderTarget().GetRTV(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

					////////////////////////////////////
					//Step 2.3: Update RenderPasses
					////////////////////////////////////
					for (auto pass : mRenderPasses)
					{
						pass->Update(&mRenderData);
					}
				}
			}		

			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 3: Copy Main camera RT to RenderingEngine RTs.
			//////////////////////////////////////////////////////////////////////////////////////////////
			CopyTextureAttribs attrib;
			attrib.pSrcTexture = Core::Scene::GetInstance().GetMainCamera()->GetRenderTarget().GetSRV()->GetTexture();
			attrib.pDstTexture = Rendering::RenderingEngine::GetInstance().GetFinalRT().GetSRV()->GetTexture();
			Graphics::Context::GetInstance().GetContext()->CopyTexture(attrib);
			
			//---------TODO: Maybe copy main camera depth rt?--------------

			//////////////////////////////////////////////////////////////////////////////////////////////
			//Step 4: Render to screen
			//////////////////////////////////////////////////////////////////////////////////////////////
			Rendering::RenderingEngine::GetInstance().RenderFinalRT();
		}

		//Shader Structs
		namespace ShaderReference
		{
			struct NEAPI Shader_SpotLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Direction;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 InnerCutOf_OuterCutoff;
				Math::Vector4 Color;
			};

			struct NEAPI Shader_PointLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 Color_FarPlane;
			};

			struct NEAPI Shader_DirLight_Struct
			{
				Math::Vector4 Direction;
				Math::Vector4 Color_Intensity;

			};
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
				NE_Light_CB_Size = NE_Light_CB_Size + (mRenderData.DirLights.size() * sizeof(ShaderReference::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (mRenderData.DirLights.size() * 2);
			}
			if (mRenderData.PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (mRenderData.PointLights.size() * sizeof(ShaderReference::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (mRenderData.PointLights.size() * 3);
			}
			if (mRenderData.SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (mRenderData.SpotLights.size() * sizeof(ShaderReference::Shader_SpotLight_Struct));
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
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, &DATA, mPSLightCB.RawDblPtr());
		}

		void RenderSystem::BakeLights()
		{
			auto LightSourcesView = Core::Scene::GetInstance().GetRegistry().view<Components::LightComponent>();

			std::vector<Components::LightComponent*> DirLights_noShadows;
			std::vector<Components::LightComponent*> PointLights_noShadows;
			std::vector<Components::LightComponent*> SpotLights_noShadows;

			//Shadows Enabled First
			for (auto entity : LightSourcesView)
			{
				auto& Light = LightSourcesView.get<Components::LightComponent>(entity);
				auto type = Light.GetType();

				if (type == Components::LightComponent::Type::Directional)
				{
					if (Light.mCastShadows)
						mRenderData.DirLights.push_back(&Light);
					else
						DirLights_noShadows.push_back(&Light);
				}
				else if (type == Components::LightComponent::Type::Point)
				{
					if (Light.mCastShadows)
						mRenderData.PointLights.push_back(&Light);
					else
						PointLights_noShadows.push_back(&Light);
				}			
				else if (type == Components::LightComponent::Type::Spot)
				{
					if (Light.mCastShadows)
						mRenderData.SpotLights.push_back(&Light);
					else
						SpotLights_noShadows.push_back(&Light);
				}
				else {
					assert(0);
				}
			}
			
			//append the non shadowed vectors
			mRenderData.DirLights.insert(mRenderData.DirLights.end(), DirLights_noShadows.begin(), DirLights_noShadows.end());
			mRenderData.SpotLights.insert(mRenderData.SpotLights.end(), SpotLights_noShadows.begin(), SpotLights_noShadows.end());
			mRenderData.PointLights.insert(mRenderData.PointLights.end(), PointLights_noShadows.begin(), PointLights_noShadows.end());

			BakeLightsBuffer();
		}

		void RenderSystem::UpdateLights()
		{
			auto LightView = Core::Scene::GetInstance().GetRegistry().view<Components::LightComponent>();
			for (auto entity : LightView)
			{
				auto& Light = LightView.get<Components::LightComponent>(entity);
				auto& Einfo = Core::Scene::GetInstance().GetRegistry().get<Components::EntityInfoComponent>(entity);
				Light.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
			}		
		}
		void RenderSystem::UpdateLightsBuffer(const Math::Vector4& CameraPos)
		{
			std::vector<Math::Vector4> LightsBuffer;
			LightsBuffer.reserve(NUM_OF_LIGHT_VECS + 1);

			LightsBuffer.push_back(CameraPos);

			for (size_t i = 0; i < mRenderData.DirLights.size(); i++)
			{
				auto& data = mRenderData.DirLights[i]->GetInternalData();
				LightsBuffer.push_back(data.Direction);
				LightsBuffer.push_back(data.Color_Intensity);
			}
			for (size_t i = 0; i < mRenderData.PointLights.size(); i++)
			{
				auto& data = mRenderData.PointLights[i]->GetInternalData();

				LightsBuffer.push_back(data.Position);
				Math::Vector4 Intensity_Attenuation(data.Color_Intensity.w, data.Attenuation_FarPlane.x, data.Attenuation_FarPlane.y, data.Attenuation_FarPlane.z);
				LightsBuffer.push_back(Intensity_Attenuation);
				Math::Vector4 Color_FarPlane(Math::Vector3(data.Color_Intensity) , data.Attenuation_FarPlane.w);
				LightsBuffer.push_back(Color_FarPlane);

			}
			for (size_t i = 0; i < mRenderData.SpotLights.size(); i++)
			{
				auto& data = mRenderData.SpotLights[i]->GetInternalData();

				LightsBuffer.push_back(data.Position);
				LightsBuffer.push_back(data.Direction);
				Math::Vector4 Intensity_Attenuation(data.Color_Intensity.w, data.Attenuation_FarPlane.x, data.Attenuation_FarPlane.y, data.Attenuation_FarPlane.z);
				LightsBuffer.push_back(Intensity_Attenuation);
				LightsBuffer.push_back(data.InnerCutOf_OuterCutoff);
				Math::Vector4 Color(Math::Vector3(data.Color_Intensity), 0.0f);
				LightsBuffer.push_back(Color);
			}

			PVoid data;
			Graphics::Context::GetInstance().GetContext()->MapBuffer(mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
			data = memcpy(data, LightsBuffer.data(), NE_Light_CB_Size);
			Graphics::Context::GetInstance().GetContext()->UnmapBuffer(mPSLightCB, MAP_WRITE);
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