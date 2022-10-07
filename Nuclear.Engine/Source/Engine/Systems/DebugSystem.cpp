#include <Engine\Systems\DebugSystem.h>
#include <Engine/Graphics/ImGui.h>
#include <Engine/Components/DirLightComponent.h>
#include <Engine/Components/SpotLightComponent.h>
#include <Engine/Components/PointLightComponent.h>
#include <Engine\Components/EntityInfoComponent.h>
#include <Engine\Systems/CameraSystem.h>
#include <Engine\Systems/RenderSystem.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine/Assets/Scene.h>
#include <Core\Logger.h>
#include <Engine\Assets\DefaultMeshes.h>

namespace Nuclear
{
	namespace Systems
	{
		DebugSystem::DebugSystem()
		{
		}
		DebugSystem::~DebugSystem()
		{
		}
		void DebugSystem::Initialize(Graphics::Camera* camera, IBuffer* _AnimationBufferPtr)
		{
			Rendering::ShadingModelBakingDesc info;
			info.CameraBufferPtr = mScene->GetSystemManager().GetSystem<CameraSystem>()->GetCameraCB();
			info.AnimationBufferPtr = _AnimationBufferPtr;

			DebugRP.Bake(info);
			mPipelineSRB = DebugRP.GetActiveSRB();
		}
		void DebugSystem::Update(ECS::TimeDelta dt)
		{
			if (ShowRegisteredRenderTargets)
			{
				ShowRendertargets();
			}

			mRegisteredRTs.clear();

			//Render Light Sources
			if (RenderLightSources)
			{
				Graphics::Context::GetContext()->SetPipelineState(DebugRP.GetActivePipeline());
				auto RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
				Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, mScene->GetSystemManager().GetSystem<RenderSystem>()->mRenderData.mFinalDepthRT.GetRTV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				
				{
					auto view = mScene->GetRegistry().view<Components::DirLightComponent>();
					for (auto entity : view)
					{
						auto& dirLightComp = view.get<Components::DirLightComponent>(entity);
						auto& EntityInfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);

						EntityInfo.mTransform.Update();
						mScene->GetSystemManager().GetSystem<CameraSystem>()->GetMainCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
						mScene->GetSystemManager().GetSystem<CameraSystem>()->UpdateBuffer();
						auto AnimationBufferPtr = mScene->GetSystemManager().GetSystem<RenderSystem>()->GetAnimationCB();

						Math::Matrix4 empty(0.0f);
						PVoid data;
						Graphics::Context::GetContext()->MapBuffer(AnimationBufferPtr, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
						data = memcpy(data, &empty, sizeof(Math::Matrix4));
						Graphics::Context::GetContext()->UnmapBuffer(AnimationBufferPtr, MAP_WRITE);


						InstantRender(Assets::DefaultMeshes::GetSphereAsset(), Managers::AssetManager::DefaultGreyTex.GetImage());


						//TODO: Render Cube at direction
						/*EntityInfo.mTransform.Update();
						mScene->GetSystemManager().GetSystem<CameraSystem>()->GetMainCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
						mScene->GetSystemManager().GetSystem<CameraSystem>()->UpdateBuffer();

						InstantRender(Assets::DefaultMeshes::GetCubeAsset(), Managers::AssetManager::DefaultGreyTex.GetImage());*/
					}
				}
				{
					auto view = mScene->GetRegistry().view<Components::PointLightComponent>();
					for (auto entity : view)
					{
						auto& EntityInfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);

						EntityInfo.mTransform.Update();
						mScene->GetSystemManager().GetSystem<CameraSystem>()->GetMainCamera()->SetModelMatrix(EntityInfo.mTransform.GetWorldMatrix());
						mScene->GetSystemManager().GetSystem<CameraSystem>()->UpdateBuffer();
						auto AnimationBufferPtr = mScene->GetSystemManager().GetSystem<RenderSystem>()->GetAnimationCB();

						Math::Matrix4 empty(0.0f);
						PVoid data;
						Graphics::Context::GetContext()->MapBuffer(AnimationBufferPtr, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
						data = memcpy(data, &empty, sizeof(Math::Matrix4));
						Graphics::Context::GetContext()->UnmapBuffer(AnimationBufferPtr, MAP_WRITE);


						InstantRender(Assets::DefaultMeshes::GetSphereAsset(), Managers::AssetManager::DefaultGreyTex.GetImage());
					}
				}
			}

		}
		void DebugSystem::ShowRendertargets()
		{
			ImGui::Begin("DebugSystem - Registered RenderTargets");

			std::vector<const char*> rt_gr;

			std::set<std::string> RT_Groups;
			for (auto& i : mRegisteredRTs)
			{
				RT_Groups.insert(i->GetDesc().mType);
			}
			for (auto& i : RT_Groups)
			{
				rt_gr.push_back(i.c_str());
			}

			static int item_current = 0;
			ImGui::Combo("RenderTarget Type", &item_current, rt_gr.data(), rt_gr.size());

			for (Uint32 i = 0; i < mRegisteredRTs.size(); i++)
			{
				auto& ref = mRegisteredRTs.at(i);
				if (ref->GetDesc().mType == rt_gr.at(item_current))
				{
					ImGui::Text(ref->GetDesc().mName.c_str());
					ImGui::SameLine();
					ImGui::Image(ref->GetSRV(), { 256.f,256.f });
					if (i % 2 == 0)
					{
						ImGui::SameLine();
					}
				}
			}

			ImGui::End();

		}
		void DebugSystem::AddRenderTarget(Graphics::RenderTarget* rt)
		{
			mRegisteredRTs.push_back(rt);
		}
		void DebugSystem::InstantRender(Assets::Mesh* mesh, Assets::Image* diffusetex)
		{

			if (mesh == nullptr)
			{
				NUCLEAR_ERROR("[DebugSystem] Skipped Rendering invalid Mesh...");
				return;
			}
			if (diffusetex == nullptr)
			{
				NUCLEAR_ERROR("[DebugSystem] Skipped Rendering Mesh with invalid texture...");
				return;
			}


			Uint64 offset = 0;

			auto i = mPipelineSRB->GetVariableCount(SHADER_TYPE_PIXEL);

			mPipelineSRB->GetVariableByName(SHADER_TYPE_PIXEL, "NEMat_Diffuse1")->Set(diffusetex->mTextureView.RawPtr());
			Graphics::Context::GetContext()->CommitShaderResources(mPipelineSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{
				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);

			}
		}
	}
}