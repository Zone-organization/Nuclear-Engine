#include <Engine\Systems\LightingSystem.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\Assets\Scene.h>
#include <Engine\Components\EntityInfoComponent.h>
#include <Engine\Systems\CameraSystem.h>
#include <Engine\Components/MeshComponent.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Core\FileSystem.h>
#include <Engine\Systems\DebugSystem.h>

namespace Nuclear
{
	namespace Systems
	{

		LightingSystem::LightingSystem(const LightingSystemDesc& desc)
		{
			pShadowPass = desc.ShadowPass;
		}
		bool LightingSystem::RequiresBaking()
		{
			if (Baked_DirLights_Size == DirLights.size() && Baked_PointLights_Size == PointLights.size() && Baked_SpotLights_Size == SpotLights.size())
				return true;

			return false;
		}
		Rendering::ShadowPass* LightingSystem::GetShadowPass()
		{
			return pShadowPass;
		}
		IBuffer* LightingSystem::GetLightCB()
		{
			return mPSLightCB.RawPtr();
		}
		size_t LightingSystem::GetDirLightsNum()
		{
			return DirLights.size();
		}
		size_t LightingSystem::GetPointLightsNum()
		{
			return PointLights.size();
		}
		size_t LightingSystem::GetSpotLightsNum()
		{
			return SpotLights.size();
		}

		void LightingSystem::BakeBuffer()
		{
			if (HasbeenBakedBefore)
			{
				if (!RequiresBaking())
				{
					NUCLEAR_WARN("[LightingSystem] No need for baking the sub system!");
				}
			}

			HasbeenBakedBefore = true;

			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
			Baked_DirLights_Size = DirLights.size();
			Baked_PointLights_Size = PointLights.size();
			Baked_SpotLights_Size = SpotLights.size();

			if (DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (DirLights.size() * 2);
			}
			if (PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (PointLights.size() * 3);
			}
			if (SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (SpotLights.size() * 5);
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

		void LightingSystem::Bake()
		{
			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);
				if (pShadowPass)
				{
					if (!DirLight.GetShadowMap()->isInitialized())
					{
						Graphics::ShadowMapDesc desc;
						desc.mResolution = pShadowPass->GetDesc().mDirLightShadowMapInfo.mResolution;
						DirLight.GetShadowMap()->Initialize(desc);
					}
				}
				DirLights.push_back(&DirLight);
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				if (pShadowPass)
				{
					if (!SpotLight.GetShadowMap()->isInitialized())
					{
						Graphics::ShadowMapDesc desc;
						desc.mResolution = pShadowPass->GetDesc().mSpotLightShadowMapInfo.mResolution;
						SpotLight.GetShadowMap()->Initialize(desc);
					}
				}
				SpotLights.push_back(&SpotLight);
			}

			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());
				PointLights.push_back(&PointLight);
			}

			BakeBuffer();

			if (pShadowPass)
			{
				pShadowPass->Initialize();
			}
		}
		void LightingSystem::Update(ECS::TimeDelta dt)
		{
			//TODO: Multiple Cameras

			std::vector<Math::Matrix4> lightspacematrices;

			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				DirLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

				if (DirLight.mCastShadows && pShadowPass)
				{
					float near_plane = 1.0f, far_plane = 7.5f;
					auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
					auto lightpos = DirLight.GetInternalPosition();

					auto lightdir = DirLight.GetDirection();
					auto lightView = glm::lookAt(lightpos, lightdir, glm::vec3(0.0, 1.0, 0.0));

					DirLight.LightSpace = lightProjection * lightView;
					lightspacematrices.push_back(DirLight.LightSpace);
					pShadowPass->DirLightShadowDepthPass(DirLight, mScene);

					//Add to debug system
					if (mScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
					{
						mScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->mRegisteredRTs.push_back(DirLight.GetShadowMap());
					}
				}
			}

			//auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			//for (auto entity : SpotLightView)
			//{
			//	auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
			//	auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
			//	SpotLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

			//	if (SpotLight.mCastShadows && pShadowPass)
			//	{
			//		float near_plane = 1.0f, far_plane = 100.f;
			//		auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			//		auto lightpos = SpotLight.GetInternalPosition();
			//		auto up = glm::vec3(0.0, 1.0, 0.0);
			//		//	auto target = lightpos + glm::vec3(0.0f);
			//		auto lightView = glm::lookAt(lightpos, lightpos + SpotLight.GetDirection(), up);

			//		SpotLight.LightSpace = lightProjection * lightView;
			//		lightspacematrices.push_back(SpotLight.LightSpace);

			//		pShadowPass->SpotLightShadowDepthPass(SpotLight, mScene);

			//		//Add to debug system
			//		if (mScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
			//		{
			//			mScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->mRegisteredRTs.push_back(SpotLight.GetShadowMap());
			//		}
			//	}
			//}

			//auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			//for (auto entity : PointLightView)
			//{
			//	auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
			//	auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
			//	PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

			//	//Shadows WIP
			//}


			//Update Shadow Manager CB
			if (pShadowPass)
			{
				PVoid data;
				Graphics::Context::GetContext()->MapBuffer(pShadowPass->GetShadowCastersCB(), MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
				data = memcpy(data, lightspacematrices.data(), sizeof(Math::Matrix4) * lightspacematrices.size());
				Graphics::Context::GetContext()->UnmapBuffer(pShadowPass->GetShadowCastersCB(), MAP_WRITE);
			}
		}
		void LightingSystem::UpdateBuffer(const Math::Vector4& CameraPos)
		{
			std::vector<Math::Vector4> LightsBuffer;
			LightsBuffer.reserve(NUM_OF_LIGHT_VECS + 1);

			LightsBuffer.push_back(CameraPos);

			for (size_t i = 0; i < DirLights.size(); i++)
			{
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Color_Intensity);
			}
			for (size_t i = 0; i < PointLights.size(); i++)
			{
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Color);

			}
			for (size_t i = 0; i < SpotLights.size(); i++)
			{
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().InnerCutOf_OuterCutoff);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Color);
			}

			PVoid data;
			Graphics::Context::GetContext()->MapBuffer(mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
			data = memcpy(data, LightsBuffer.data(), NE_Light_CB_Size);
			Graphics::Context::GetContext()->UnmapBuffer(mPSLightCB, MAP_WRITE);
		}
	}
}