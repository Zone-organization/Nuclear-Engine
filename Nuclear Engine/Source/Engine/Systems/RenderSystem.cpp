#include <Engine\Systems\RenderSystem.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/EntityInfoComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\AnimatorComponent.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Managers\CameraManager.h>
#include <Engine\Managers\AssetManager.h>
#include <Engine\ECS\Scene.h>
#include <Core\Engine.h>
#include <cstring>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include "Engine/Animation/Animator.h"
#include <Core\Logger.h>

namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(Managers::CameraManager* CameraManager)
		{
			mCameraManager = CameraManager;
			mActiveRenderingPipeline = nullptr;
		}
		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::AddRenderingPipeline(Graphics::RenderingPipeline* Pipeline)
		{
			if (!Pipeline)
			{
				Log.Error("[RenderSystem] Pipeline is invalid!\n");
				return;
			}
			mRenderingPipelines[Pipeline->GetID()] = Pipeline;

			if (mActiveRenderingPipeline == nullptr)
				mActiveRenderingPipeline =	mRenderingPipelines[Pipeline->GetID()];
		}

		void RenderSystem::SetActiveRenderingPipeline(Uint32 PipelineID)
		{
			if (mActiveRenderingPipeline)
			{
				if (mActiveRenderingPipeline->GetID() == PipelineID)
				{
					return;
				}
			}

			auto it = mRenderingPipelines.find(PipelineID);
			if (it != mRenderingPipelines.end())
			{
				mActiveRenderingPipeline = it->second;
				return;
			}
			Log.Error("[RenderSystem] Pipeline ID(" + Utilities::int_to_hex<Uint32>(PipelineID)+ ") is not found, while setting it active.\n");
		}

		void RenderSystem::CreateMaterialForAllPipelines(Assets::Material* material)
		{
			if (!material)
				return;

			for (auto it : mRenderingPipelines)
			{
				material->CreateInstance(it.second);
			}
		}

		void RenderSystem::CreateMaterial(Assets::Material* material, Uint32 PipelineID)
		{
			if (!material)
				return;

			auto it = mRenderingPipelines.find(PipelineID);
			if (it != mRenderingPipelines.end())
			{
				material->CreateInstance(it->second);
				return;
			}

			Log.Error("[RenderSystem] Pipeline ID(" + Utilities::int_to_hex<Uint32>(PipelineID) + ") is not found, while creating material instance from it.\n");
		}

		bool RenderSystem::NeedsBaking()
		{
			if (mStatus.BakeLighting || mStatus.BakePipelines)
			{
				return true;
			}

			return false;
		}

		void RenderSystem::Bake(bool AllPipelines)
		{
			auto DirLightView = mScene->GetRegistry().view<Components::DirLightComponent>();
			for (auto entity : DirLightView)
			{
				auto& DirLight = DirLightView.get<Components::DirLightComponent>(entity);

				mLightingSystem.DirLights.push_back(&DirLight);
			}

			auto SpotLightView = mScene->GetRegistry().view<Components::SpotLightComponent>();
			for (auto entity : SpotLightView)
			{
				auto& SpotLight = SpotLightView.get<Components::SpotLightComponent>(entity);
				mLightingSystem.SpotLights.push_back(&SpotLight);
			}

			auto PointLightView = mScene->GetRegistry().view<Components::PointLightComponent>();
			for (auto entity : PointLightView)
			{
				auto& PointLight = PointLightView.get<Components::PointLightComponent>(entity);
				mLightingSystem.PointLights.push_back(&PointLight);
			}

			mLightingSystem.BakeBuffer();
			//mLightingSystem.UpdateBuffer(Math::Vector4(mCameraManager->GetMainCamera()->GetPosition(), 1.0f));
			BufferDesc CBDesc;
			CBDesc.Name = "NEStatic_Animation";
			CBDesc.Size = sizeof(Math::Matrix4) * 100;
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &animCB);

			Graphics::RenderingPipelineDesc RPDesc;
			
			RPDesc.DirLights = static_cast<Uint32>(mLightingSystem.DirLights.size());
			RPDesc.SpotLights = static_cast<Uint32>(mLightingSystem.SpotLights.size());
			RPDesc.PointLights = static_cast<Uint32>(mLightingSystem.PointLights.size());
			RPDesc.CameraBufferPtr = mCameraManager->GetCameraCB();
			RPDesc.LightsBufferPtr = mLightingSystem.mPSLightCB;
			RPDesc.AnimationBufferPtr = animCB;
			if(AllPipelines)
			{ 
				for (auto it : mRenderingPipelines)
				{
					if(it.second->GetStatus() != Graphics::BakeStatus::Baked)
						it.second->Bake(RPDesc);
				}
			}
			else
			{
				if (mActiveRenderingPipeline->GetStatus() != Graphics::BakeStatus::Baked)
					mActiveRenderingPipeline->Bake(RPDesc);
			}

			//TODO: Move!
			Assets::TextureSet CubeSet;
			CubeSet.mData.push_back({ 0, Managers::AssetManager::DefaultDiffuseTex });
			CubeSet.mData.push_back({ 1, Managers::AssetManager::DefaultSpecularTex });
			LightSphereMaterial.mPixelShaderTextures.push_back(CubeSet);
			CreateMaterialForAllPipelines(&LightSphereMaterial);

			Assets::Mesh::CreateScreenQuad(&CameraScreenQuad);

		}
		IPipelineState * RenderSystem::GetPipeline()
		{
			return mActiveRenderingPipeline->GetPipeline();
		}
		void RenderSystem::RenderMeshes()
		{
			auto view = mScene->GetRegistry().view<Components::MeshComponent>();

			for (auto entity : view)
			{
				auto& MeshObject = view.get<Components::MeshComponent>(entity);
				if (MeshObject.mRender)
				{
					if (!MeshObject.mMultiRender)
					{
						auto& EntityInfo = mScene->GetRegistry().get<Components::EntityInfoComponent>(entity);
						mCameraManager->GetMainCamera()->SetModelMatrix(EntityInfo.mTransform.GetTransform());
						mCameraManager->UpdateBuffer();

						auto AnimatorComponent = mScene->GetRegistry().try_get<Components::AnimatorComponent>(entity);

						if (AnimatorComponent != nullptr)
						{
							std::vector<Math::Matrix4> ok;
							ok.reserve(100);

							auto transforms = AnimatorComponent->mAnimator->GetFinalBoneMatrices();
							for (int i = 0; i < transforms.size(); ++i)
							{
								ok.push_back(transforms[i]);
							}

							PVoid data;
							Graphics::Context::GetContext()->MapBuffer(animCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
							data = memcpy(data, ok.data(), ok.size() * sizeof(Math::Matrix4));
							Graphics::Context::GetContext()->UnmapBuffer(animCB, MAP_WRITE);

						}

						InstantRender(MeshObject);

					}
					else
					{
						for (auto i : MeshObject.mMultiRenderTransforms)
						{
							mCameraManager->GetMainCamera()->SetModelMatrix(i);
							mCameraManager->UpdateBuffer();
							InstantRender(MeshObject);
						}
					}
				}
			}
		}
		void RenderSystem::Update(ECS::TimeDelta dt)
		{	
			//Render Scene from each avtive camera perspective
			for (auto Camera : mCameraManager->ActiveCameras)
			{
				Camera->GetCameraRT()->SetActive((float*)&Camera->RTClearColor);
				mLightingSystem.UpdateBuffer(Math::Vector4(Camera->GetPosition(), 1.0f));

				Graphics::Context::GetContext()->SetPipelineState(GetPipeline());

				RenderMeshes();

				if (VisualizePointLightsPositions)
				{
					for (unsigned int i = 0; i < mLightingSystem.PointLights.size(); i++)
					{
						Math::Matrix4 model(1.0f);
						model = Math::translate(model, Math::Vector3(mLightingSystem.PointLights[i]->GetInternalData().Position));
						model = Math::scale(model, Math::Vector3(0.25f));
						Camera->SetModelMatrix(model);
						mCameraManager->UpdateBuffer();

						InstantRender(Assets::DefaultMeshes::GetSphereAsset(), &LightSphereMaterial);

					}
				}

				if (Camera->RenderSkybox == true && Camera->mSkybox != nullptr)
				{
					Camera->mSkybox->Render();
				}
			}
			//Render Main camera view to screen
			auto* RTV = Graphics::Context::GetSwapChain()->GetCurrentBackBufferRTV();
			auto* DSV = Graphics::Context::GetSwapChain()->GetDepthBufferDSV();
			Graphics::Context::GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearRenderTarget(RTV, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			auto MainCameraPtr = mCameraManager->GetMainCamera();
			Graphics::Context::GetContext()->SetPipelineState(MainCameraPtr->GetActivePipeline());
			Graphics::Context::GetContext()->CommitShaderResources(MainCameraPtr->GetActiveSRB(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

			Uint64 offset = 0;
			Graphics::Context::GetContext()->SetIndexBuffer(CameraScreenQuad.mSubMeshes.at(0).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &CameraScreenQuad.mSubMeshes.at(0).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
			
			DrawIndexedAttribs DrawAttrs;
			DrawAttrs.IndexType = VT_UINT32;
			DrawAttrs.NumIndices = CameraScreenQuad.mSubMeshes.at(0).mIndicesCount;
			Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);
		}

		void RenderSystem::InstantRender(const Components::MeshComponent &object)
		{
			if (Core::Engine::isDebug())
			{
				if (object.mMesh == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering invalid Mesh...\n");
					return;
				}
				if (object.mMaterial == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering Mesh with invalid Material...\n");
					return;
				}
			}

			InstantRender(object.mMesh, object.mMaterial);
		}
		void RenderSystem::InstantRender(Assets::Mesh * mesh, Assets::Material* material)
		{
			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{
				material->GetMaterialInstance(mActiveRenderingPipeline->GetID())->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

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