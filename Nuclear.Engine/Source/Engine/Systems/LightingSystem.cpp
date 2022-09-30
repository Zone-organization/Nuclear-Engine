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
#include <Engine/Graphics/ImGui.h>

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

			std::vector<Components::DirLightComponent*> DirLights_noShadows;
			std::vector<Components::PointLightComponent*> PointLights_noShadows;
			std::vector<Components::SpotLightComponent*> SpotLights_noShadows;

			//Shadows Enabled First
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);
				if(DirLight.mCastShadows)
					DirLights.push_back(&DirLight);
				else
					DirLights_noShadows.push_back(&DirLight);
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				if (SpotLight.mCastShadows)
					SpotLights.push_back(&SpotLight);
				else
					SpotLights_noShadows.push_back(&SpotLight);
			}

			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				if (PointLight.mCastShadows)
					PointLights.push_back(&PointLight);
				else
					PointLights_noShadows.push_back(&PointLight);
			}
					
			//append the non shadowed vectors
			DirLights.insert(DirLights.end(), DirLights_noShadows.begin(), DirLights_noShadows.end());
			SpotLights.insert(SpotLights.end(), SpotLights_noShadows.begin(), SpotLights_noShadows.end());
			PointLights.insert(PointLights.end(), PointLights_noShadows.begin(), PointLights_noShadows.end());

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
			Uint32 DirPosCasterRTIndex = 0 , SpotCasterRTIndex = 0;

			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				DirLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

				if (DirLight.mCastShadows && pShadowPass)
				{
					static float near_plane = 1.0f, far_plane = 7.5f, ab = 10.f;
					static bool useLightPos = true;

					ImGui::Begin("LIGHT");

					ImGui::DragFloat("Near plane", &near_plane, 0.5f, 0.0f, 0.0f, "%.6f");
					ImGui::DragFloat("Far plane", &far_plane, 0.5f, 0.0f, 0.0f, "%.6f");

					ImGui::DragFloat("AABB", &ab);

					ImGui::Checkbox("useLightPos", &useLightPos);

					ImGui::End();
					
					glm::vec4 aabb = glm::vec4(-ab, ab, -ab, ab);

					auto lightpos = DirLight.GetInternalPosition();

					glm::vec3 lightdir;

					if (useLightPos)
					{
						lightdir = lightpos;
					}
					else {
						auto lightdir = DirLight.GetDirection();
					}

					auto lightProjection = glm::ortho(aabb.x, aabb.y, aabb.z, aabb.w, near_plane, far_plane);

					auto lightView = glm::lookAt(lightdir, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
					

					DirLight.LightSpace = lightProjection * lightView;
					lightspacematrices.push_back(DirLight.LightSpace);


					pShadowPass->DirLightShadowDepthPass(DirLight, DirPosCasterRTIndex, mScene);
					DirPosCasterRTIndex++;
					//Add to debug system
					/*if (mScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
					{
						mScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->AddRenderTarget(DirLight.GetShadowMap());
					}*/
				}
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				SpotLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

				if (SpotLight.mCastShadows && pShadowPass)
				{
					auto lightProjection = glm::perspective(Math::radians(SpotLight.GetFOV()), 1024.f/1024.f, SpotLight.GetNearPlane(), SpotLight.GetFarPlane());
					auto lightpos = SpotLight.GetInternalPosition();
					auto up = glm::vec3(0.0, 1.0, 0.0);

					auto lightView = glm::lookAt(lightpos, lightpos + SpotLight.GetDirection(), up);

					SpotLight.LightSpace = lightProjection * lightView;
					lightspacematrices.push_back(SpotLight.LightSpace);

					pShadowPass->SpotLightShadowDepthPass(SpotLight, SpotCasterRTIndex, mScene);
					SpotCasterRTIndex++;

					//Add to debug system
					/*if (mScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
					{
						mScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->AddRenderTarget(SpotLight.GetShadowMap());
					}*/
				}
			}


			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				auto& Einfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
				PointLight.SetInternalPosition(Einfo.mTransform.GetLocalPosition());

				if (PointLight.mCastShadows && pShadowPass)
				{
					pShadowPass->PointLightShadowDepthPass(PointLight, mScene);

					//Add to debug system
					//if (mScene->GetSystemManager().GetSystem<Systems::DebugSystem>())
				//	{
					//	mScene->GetSystemManager().GetSystem<Systems::DebugSystem>()->AddRenderTarget(PointLight.GetShadowMap());
				//	}
				}
			}

		

			//Update Shadow Manager CB
			if (pShadowPass && pShadowPass->GetLightSpacesCB())
			{
				PVoid data;
				Graphics::Context::GetContext()->MapBuffer(pShadowPass->GetLightSpacesCB(), MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
				data = memcpy(data, lightspacematrices.data(), sizeof(Math::Matrix4) * lightspacematrices.size());
				Graphics::Context::GetContext()->UnmapBuffer(pShadowPass->GetLightSpacesCB(), MAP_WRITE);
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
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Color_FarPlane);

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