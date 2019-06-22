#include <Engine\Systems\RenderSystem.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/TransformComponent.h>
#include <Engine\Graphics\GraphicsEngine.h>
#include <Engine\Assets\Material.h>
#include "Diligent\Graphics\GraphicsEngine\interface\MapHelper.h"
#include <Engine\Assets\DefaultTextures.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <cstring>

namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(const RenderSystemDesc & desc, Components::CameraComponent* camera)
		{
			Desc = desc;
			ActiveCamera = camera;
			this->status = RenderSystemStatus::RequireBaking;
		}
		RenderSystem::~RenderSystem()
		{
		}

		void RenderSystem::SetRenderingPipeline(Graphics::RenderingPipeline* Pipeline)
		{
			mRenderingPipeline = Pipeline;
		}

		/*void RenderSystem::InitializePostProcessing(unsigned int WindowWidth, unsigned int WindowHeight)
		{
		    ITextureDescriptor ScreenTexDesc;
			if (Desc.HDR == true)
			{
				ScreenTexDesc.format = IFormat::RGBA16Float;
			}
			else
			{
				ScreenTexDesc.format = IFormat::RGBA8UNorm;
			}
			ScreenTexDesc.type = ITextureType::Texture2D;

			ISrcImageDescriptor Data;
			Data.data = NULL;
			ScreenTexDesc.extent.width = WindowWidth;
			ScreenTexDesc.extent.height = WindowHeight;

			PostProcessTexture = IContext::GetRenderer()->CreateTexture(ScreenTexDesc, &Data);

			IRenderTargetDescriptor RTDesc;
			RTDesc.resolution.width = WindowWidth;
			RTDesc.resolution.height = WindowHeight;
			RTDesc.attachments.push_back({ IAttachmentType::Color, PostProcessTexture });
			RTDesc.attachments.push_back({ IAttachmentType::DepthStencil });

			PostProcessRT = IContext::GetRenderer()->CreateRenderTarget(RTDesc);

			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Use_Camera = false;
			VertShaderDesc.InNormals = false;
			VertShaderDesc.OutFragPos = false;

			PostProcess_VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
			Assets::Mesh::CreateScreenQuad(&PostProcessScreenQuad);
			PostProcessScreenQuad.Initialize(PostProcess_VShader);
			std::vector<std::string> defines;
			if (Desc.GammaCorrection == true) { defines.push_back("NE_GAMMA_CORRECTION_ENABLED"); }
			if (Desc.HDR == true) { defines.push_back("NE_HDR_ENABLED"); }


			IShaderCompiler::CompileAndCreateShader(&PostProcess_PShader,
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
				IShaderType::Fragment);
	
			ISamplerDescriptor Samplerdesc;
			Samplerdesc.magFilter = Samplerdesc.minFilter = Samplerdesc.mipMapFilter = ISamplerFilter::Nearest;
			ScreenSampler = IContext::GetRenderer()->CreateSampler(Samplerdesc);
		}*/
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
			DirLights.push_back(light);
		}
		void RenderSystem::AddLight(Components::PointLight * light)
		{
			PointLights.push_back(light);
		}
		void RenderSystem::AddLight(Components::SpotLight * light)
		{
			SpotLights.push_back(light);
		}

		void RenderSystem::CreateMaterial(Assets::Material * material)
		{
			if (!material)
				return;

			material->CreateInstance(mRenderingPipeline);
		}

		void RenderSystem::Bake()
		{
			BakeLightConstantBuffer();
			Update_Light();

			Graphics::RenderingPipelineDesc RPDesc;
			
			RPDesc.DirLights = DirLights.size();
			RPDesc.SpotLights = SpotLights.size();
			RPDesc.PointLights = PointLights.size();
			RPDesc.UseNormalMaps = false;
			RPDesc.CameraBufferPtr = ActiveCamera->GetCBuffer();
			RPDesc.LightsBufferPtr = mPSLightCB;
			mRenderingPipeline->Bake(RPDesc);
	
			//TODO: Move!
			Assets::TextureSet CubeSet;
			CubeSet.push_back({ 0, Assets::DefaultTextures::DefaultDiffuseTex });
			CubeSet.push_back({ 1, Assets::DefaultTextures::DefaultSpecularTex });
			LightSphereMaterial.mPixelShaderTextures.push_back(CubeSet);
			CreateMaterial(&LightSphereMaterial);
		}
		IPipelineState * RenderSystem::GetPipeline()
		{
			return mRenderingPipeline->GetPipeline();
		}

		void RenderSystem::BakeLightConstantBuffer()
		{	
			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
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

			BufferDesc CBDesc;
			CBDesc.Name = "RenderSystem LightCB";
			CBDesc.uiSizeInBytes = NE_Light_CB_Size;
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, &BufferData(), mPSLightCB.GetRawDblPtr());
		}

		void RenderSystem::Update_Light()
		{
			std::vector<Math::Vector4> LightsBuffer;
			//LightsBuffer.reserve(NUM_OF_LIGHT_VECS);

			LightsBuffer.push_back(Math::Vector4(ActiveCamera->GetPosition(), 1.0f));

			for (size_t i = 0; i < DirLights.size(); i++)
			{
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Color);
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

		void RenderSystem::Update_Meshes(ECS::EntityManager & es)
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

			Update_Meshes(es);
			Update_Light();

			if (VisualizePointLightsPositions)
			{
				for (unsigned int i = 0; i < PointLights.size(); i++)
				{
					Math::Matrix4 model(1.0f);
					model = Math::translate(model, Math::Vector3(PointLights[i]->GetInternalData().Position));
					model = Math::scale(model, Math::Vector3(0.25f));
					ActiveCamera->SetModelMatrix(model);

					InstantRender(Assets::DefaultMeshes::GetSphereAsset(), &LightSphereMaterial);

				}

			}
		}
		void RenderSystem::InstantRender(Components::MeshComponent * object)
		{
			if (object == nullptr)
			{
				Log.Error("[RenderSystem] Skipped Rendering invalid MeshComponent...\n");
				return;
			}
			if (object->mMesh == nullptr)
			{
				Log.Error("[RenderSystem] Skipped Rendering invalid Mesh...\n");
				return;
			}
			if (object->mMaterial == nullptr)
			{
				Log.Error("[RenderSystem] Skipped Rendering Mesh with invalid Material...\n");
				return;
			}

			InstantRender(object->mMesh, object->mMaterial);
		}
		void RenderSystem::InstantRender(Assets::Mesh * mesh, Assets::Material* material)
		{
			Uint32 offset = 0;

			for (size_t i = 0; i< mesh->mSubMeshes.size(); i++)
			{
				material->GetMaterialInstance(mRenderingPipeline->GetID())->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
				
				DrawAttribs DrawAttrs;
				DrawAttrs.IsIndexed = true;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->Draw(DrawAttrs);
			}
		}
		//void RenderSystem::RenderToPostProcessingRT()
		//{
			///PostProcessRT.Bind();
			///IContext::Clear(IColor(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		//}
		//void RenderSystem::RenderPostProcessingContents()
		//{
			///PostProcessRT.Bind_Default();
			///IContext::Clear(IColor(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
			///PostProcess_VShader.Bind();
			///PostProcess_PShader.Bind();
			///ScreenSampler.PSBind(0);
			///PostProcessTexture.PSBind(0);
			///InstantRender(&PostProcessScreenQuad, nullptr);
		//}
	
	}
}