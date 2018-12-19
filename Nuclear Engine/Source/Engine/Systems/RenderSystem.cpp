#include <Engine\Systems\RenderSystem.h>
#include <Core\FileSystem.h>
#include <LLGL\Buffer.h>
#include <Engine/Graphics/ShaderCompiler.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Components/TransformComponent.h>
#include <Engine\Managers\ShaderManager.h>
#include <Engine\Assets\Material.h>

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
		void RenderSystem::BakePipelines()
		{  
			// Create shader program which is used as composite
			LLGL::ShaderProgramDescriptor shaderProgramDesc;
			{
				shaderProgramDesc.vertexFormats = { VShader.mVSFormat };
				shaderProgramDesc.vertexShader = VShader.mShader;
				shaderProgramDesc.fragmentShader = PShader.mShader;
			}
			LLGL::ShaderProgram* shaderProgram = Graphics::Context::GetRenderer()->CreateShaderProgram(shaderProgramDesc);

			// Link shader program and check for errors
			if (shaderProgram->HasErrors())
				Log.Error(shaderProgram->QueryInfoLog());
			
		
			// Create graphics pipeline
			LLGL::GraphicsPipelineDescriptor pipelineDesc;
			{
				pipelineDesc.shaderProgram = ShaderProgram;
				pipelineDesc.pipelineLayout = PipelineLayout;
				pipelineDesc.primitiveTopology = LLGL::PrimitiveTopology::TriangleStrip;
			}
			Pipeline = Graphics::Context::GetRenderer()->CreateGraphicsPipeline(pipelineDesc);
		}
		void RenderSystem::InitializePostProcessing(unsigned int WindowWidth, unsigned int WindowHeight)
		{
			LLGL::TextureDescriptor ScreenTexDesc;
			if (Desc.HDR == true)
			{
				ScreenTexDesc.format = LLGL::Format::RGBA16Float;
			}
			else
			{
				ScreenTexDesc.format = LLGL::Format::RGBA8UNorm;
			}
			ScreenTexDesc.type = LLGL::TextureType::Texture2D;

			LLGL::SrcImageDescriptor Data;
			Data.data = NULL;
			ScreenTexDesc.extent.width = WindowWidth;
			ScreenTexDesc.extent.height = WindowHeight;

			PostProcessTexture = Graphics::Context::GetRenderer()->CreateTexture(ScreenTexDesc, &Data);

			LLGL::RenderTargetDescriptor RTDesc;
			RTDesc.resolution.width = WindowWidth;
			RTDesc.resolution.height = WindowHeight;
			RTDesc.attachments.push_back({ LLGL::AttachmentType::Color, PostProcessTexture });
			RTDesc.attachments.push_back({ LLGL::AttachmentType::DepthStencil });

			PostProcessRT = Graphics::Context::GetRenderer()->CreateRenderTarget(RTDesc);

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


			Graphics::ShaderCompiler::CompileAndCreateShader(&PostProcess_PShader,
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
				LLGL::ShaderType::Fragment);
	
			LLGL::SamplerDescriptor Samplerdesc;
			Samplerdesc.magFilter = Samplerdesc.minFilter = Samplerdesc.mipMapFilter = LLGL::SamplerFilter::Nearest;
			ScreenSampler = Graphics::Context::GetRenderer()->CreateSampler(Samplerdesc);
		}
		void RenderSystem::SetCamera(Components::CameraComponent * camera)
		{
			this->ActiveCamera = camera;
		}
		Components::CameraComponent* RenderSystem::GetCamera()
		{
			return this->ActiveCamera;
		}
		Graphics::Shader RenderSystem::GetVertexShader()
		{
			return this->VShader;
		}
		Graphics::Shader RenderSystem::GetPixelShader()
		{
			return this->PShader;
		}
		void RenderSystem::BindShaders()
		{
			//BINDING_LLGL
			//VShader.Bind();
			//PShader.Bind();
		}
		void RenderSystem::BindConstantBuffers()
		{
			///ActiveCamera->GetCBuffer().VSBind(VShader.GetCBSlot(&ActiveCamera->GetCBuffer()));
			///NE_Light_CB.PSBind(VShader.GetCBSlot(&NE_Light_CB));
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

		void RenderSystem::Bake()
		{
			BakeVertexShader();
			BakePixelShader();
			BakePipelines();
		}
		void RenderSystem::BakePixelShader()
		{
			std::vector<std::string> defines;
			LLGL::PipelineLayoutDescriptor layoutDesc;
			layoutDesc.bindings.push_back({ LLGL::ResourceType::Texture, LLGL::StageFlags::FragmentStage, 0 });
			layoutDesc.bindings.push_back({ LLGL::ResourceType::Texture, LLGL::StageFlags::FragmentStage, 1 });


			if (this->DirLights.size() > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(DirLights.size())); PSDirty = true; }
			if (this->PointLights.size() > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(PointLights.size()));  PSDirty = true; }
			if (this->SpotLights.size() > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(SpotLights.size()));  PSDirty = true; }
			if (Desc.NormalMaps == true) 
			{ 
				defines.push_back("NE_USE_NORMAL_MAPS");
				layoutDesc.bindings.push_back({ LLGL::ResourceType::Texture, LLGL::StageFlags::FragmentStage, 2 });
			}

			layoutDesc.bindings.push_back({ LLGL::ResourceType::Sampler, LLGL::StageFlags::FragmentStage, 0 });
			layoutDesc.bindings.push_back({ LLGL::ResourceType::Sampler, LLGL::StageFlags::FragmentStage, 1 });

			if (Desc.NormalMaps == true)
			{
				layoutDesc.bindings.push_back({ LLGL::ResourceType::Sampler, LLGL::StageFlags::FragmentStage, 2 });
			}

			if (PSDirty = true)
			{
				Graphics::Context::GetRenderer()->Release(*PShader.mShader);
				Graphics::Context::GetRenderer()->Release(*NE_Light_CB);

				Graphics::ShaderCompiler::CompileAndCreateShader(&PShader,
					Core::FileSystem::LoadShader(Desc.PShaderPath, defines, std::vector<std::string>(), true), LLGL::ShaderType::Fragment);


				Calculate_Light_CB_Size();

				NE_Light_CB = Graphics::Context::GetRenderer()->CreateBuffer(LLGL::ConstantBufferDesc(this->NE_Light_CB_Size));

				//this->PShader.SetConstantBuffer(&this->NE_Light_CB);
				PSDirty = false;
			}

			PipelineLayout = Graphics::Context::GetRenderer()->CreatePipelineLayout(layoutDesc);
		}
		void RenderSystem::BakeVertexShader()
		{
			if (VSDirty = true)
			{
				Graphics::Context::GetRenderer()->Release(*VShader.mShader);

				if (Desc.VShaderPath == "NE_Default")
				{
					Managers::AutoVertexShaderDesc VertShaderDesc;
					if (Desc.NormalMaps == true) { VertShaderDesc.InTangents = true; }

					VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
				}
				else {
					Graphics::ShaderCompiler::CompileAndCreateShader(&VShader,Core::FileSystem::LoadFileToString(Desc.VShaderPath), LLGL::ShaderType::Vertex);

				}

				/*if (this->ActiveCamera != nullptr)
					this->VShader.SetConstantBuffer(&this->ActiveCamera->GetCBuffer());
				else
					Log.Warning("[RenderSystem] Baking the renderer without an active camera!\n");
					*/
				VSDirty = false;
			}
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
			Graphics::Context::GetRenderer()->WriteBuffer(*NE_Light_CB, 0, LightsBuffer.data(), NE_Light_CB_Size);
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
			BindShaders();
			BindConstantBuffers();
			Update_Meshes(es);
			Update_Light();

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
		/*	material->Bind();
			material->UpdateMaterialCBuffer();
			for (size_t i = 0; i< mesh->mSubMeshes.size(); i++)
			{
				material->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetCommands()->SetIndexBuffer(*mesh->mSubMeshes.at(i).mIB);
				Graphics::Context::GetCommands()->SetVertexBuffer(*mesh->SubMeshes.at(i).mVB);

				Graphics::Context::GetCommands()->DrawIndexed(mesh->SubMeshes.at(i).IndicesCount, 0);
			}*/
		}
		void RenderSystem::RenderToPostProcessingRT()
		{
			///PostProcessRT.Bind();
			///LLGL::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		}
		void RenderSystem::RenderPostProcessingContents()
		{
			///PostProcessRT.Bind_Default();
			///LLGL::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
			///PostProcess_VShader.Bind();
			///PostProcess_PShader.Bind();
			///ScreenSampler.PSBind(0);
			///PostProcessTexture.PSBind(0);
			///InstantRender(&PostProcessScreenQuad, nullptr);
		}
	
		void RenderSystem::Calculate_Light_CB_Size()
		{
			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
			if (this->DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (this->DirLights.size() * 2);
			}
			if (this->PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (this->PointLights.size() * 3);
			}
			if (this->SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (this->SpotLights.size() * 5);
			}
		}

	}
}