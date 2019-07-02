#include <Engine\Systems\RenderSystem.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/TransformComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Assets\Material.h>
#include <Engine\Assets\DefaultTextures.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Core\Engine.h>
#include <cstring>

namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(Components::CameraComponent* camera)
		{
			ActiveCamera = camera;
			mStatus = RenderSystemStatus::RequireBaking;
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

		void RenderSystem::SetCamera(Components::CameraComponent * camera)
		{
			this->ActiveCamera = camera;
		}
		Components::CameraComponent* RenderSystem::GetCamera()
		{
			return this->ActiveCamera;
		}

		void RenderSystem::AddLight(Components::DirectionalLight * light)
		{
			mLightingSystem.DirLights.push_back(light);
		}
		void RenderSystem::AddLight(Components::PointLight * light)
		{
			mLightingSystem.PointLights.push_back(light);
		}
		void RenderSystem::AddLight(Components::SpotLight * light)
		{
			mLightingSystem.SpotLights.push_back(light);
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

		void RenderSystem::Bake(bool AllPipelines)
		{
			mLightingSystem.BakeBuffer();
			mLightingSystem.UpdateBuffer(Math::Vector4(ActiveCamera->GetPosition(), 1.0f));

			Graphics::RenderingPipelineDesc RPDesc;
			
			RPDesc.DirLights = static_cast<Uint32>(mLightingSystem.DirLights.size());
			RPDesc.SpotLights = static_cast<Uint32>(mLightingSystem.SpotLights.size());
			RPDesc.PointLights = static_cast<Uint32>(mLightingSystem.PointLights.size());
			RPDesc.CameraBufferPtr = ActiveCamera->GetCBuffer();
			RPDesc.LightsBufferPtr = mLightingSystem.mPSLightCB;

			if(AllPipelines)
			{ 
				for (auto it : mRenderingPipelines)
				{
					it.second->Bake(RPDesc);
				}
			}
			else
			{
				mActiveRenderingPipeline->Bake(RPDesc);
			}

			//TODO: Move!
			Assets::TextureSet CubeSet;
			CubeSet.push_back({ 0, Assets::DefaultTextures::DefaultDiffuseTex });
			CubeSet.push_back({ 1, Assets::DefaultTextures::DefaultSpecularTex });
			LightSphereMaterial.mPixelShaderTextures.push_back(CubeSet);
			CreateMaterialForAllPipelines(&LightSphereMaterial);
		}
		IPipelineState * RenderSystem::GetPipeline()
		{
			return mActiveRenderingPipeline->GetPipeline();
		}
		void RenderSystem::UpdateMeshes(ECS::EntityManager & es)
		{
			ECS::ComponentHandle<Components::MeshComponent> MeshObject;
			for (ECS::Entity entity : es.entities_with_components(MeshObject))
			{
				if (!MeshObject.Get()->mMultiRender)
				{
					//entity.GetTransform()->Update();
					ActiveCamera->SetModelMatrix(entity.GetComponent<Components::TransformComponent>().Get()->GetTransformMatrix());
					InstantRender(MeshObject.Get());
				}
				else 
				{
					for (auto i : MeshObject.Get()->mMultiRenderTransforms)
					{
						ActiveCamera->SetModelMatrix(i);
						InstantRender(MeshObject.Get());
					}
				}
			}
		}
		void RenderSystem::Update(ECS::EntityManager & es, ECS::EventManager & events, ECS::TimeDelta dt)
		{
			Graphics::Context::GetContext()->SetPipelineState(GetPipeline());

			UpdateMeshes(es);
			mLightingSystem.UpdateBuffer(Math::Vector4(ActiveCamera->GetPosition(), 1.0f));

			if (VisualizePointLightsPositions)
			{
				for (unsigned int i = 0; i < mLightingSystem.PointLights.size(); i++)
				{
					Math::Matrix4 model(1.0f);
					model = Math::translate(model, Math::Vector3(mLightingSystem.PointLights[i]->GetInternalData().Position));
					model = Math::scale(model, Math::Vector3(0.25f));
					ActiveCamera->SetModelMatrix(model);

					InstantRender(Assets::DefaultMeshes::GetSphereAsset(), &LightSphereMaterial);

				}

			}
		}
		void RenderSystem::InstantRender(Components::MeshComponent * object)
		{
			if (Core::Engine::isDebug())
			{
				if (object == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering invalid MeshComponent...\n");
					return;
				}
				if (object->mMesh == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering invalid Mesh...\n");
					return;
				}
				if (object->mMaterial == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering Mesh with invalid Material...\n");
					return;
				}
			}

			InstantRender(object->mMesh, object->mMaterial);
		}
		void RenderSystem::InstantRender(Assets::Mesh * mesh, Assets::Material* material)
		{
			Uint32 offset = 0;

			for (size_t i = 0; i< mesh->mSubMeshes.size(); i++)
			{
				material->GetMaterialInstance(mActiveRenderingPipeline->GetID())->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
				
				DrawAttribs DrawAttrs;
				DrawAttrs.IsIndexed = true;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->Draw(DrawAttrs);
			}
		}	
	}
}