#include <Engine\Systems\RenderSystem.h>
#include <Core\FileSystem.h>
#include <Engine/Graphics/API/ConstantBuffer.h>
#include <Engine/Graphics/API/ShaderCompiler.h>
#include <Engine\Graphics\API\Context.h>
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
		void RenderSystem::InitializePostProcessing(unsigned int WindowWidth, unsigned int WindowHeight)
		{
			Graphics::API::Texture_Desc ScreenTexDesc;
			if (Desc.HDR == true)
			{
				ScreenTexDesc.Format = Graphics::API::Format::R16G16B16A16_FLOAT;
			}
			else
			{
				ScreenTexDesc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
			}
			ScreenTexDesc.Type = Graphics::API::TextureType::Texture2D;
			ScreenTexDesc.GenerateMipMaps = false;

			Graphics::API::Texture_Data Data;
			Data.Img_Data_Buf = NULL;
			Data.Width = WindowWidth;
			Data.Height = WindowHeight;
			Graphics::API::Texture::Create(&PostProcessTexture, Data, ScreenTexDesc);

			Graphics::API::RenderTarget::Create(&PostProcessRT);
			PostProcessRT.AttachTexture(&PostProcessTexture);
			PostProcessRT.AttachDepthStencilBuffer(Math::Vector2ui(WindowWidth, WindowHeight));

			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Use_Camera = false;
			VertShaderDesc.InNormals = false;
			VertShaderDesc.OutFragPos = false;

			PostProcess_VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
			Assets::Mesh::CreateScreenQuad(&PostProcessScreenQuad);
			PostProcessScreenQuad.Initialize(&PostProcess_VShader);
			std::vector<std::string> defines;
			if (Desc.GammaCorrection == true) { defines.push_back("NE_GAMMA_CORRECTION_ENABLED"); }
			if (Desc.HDR == true) { defines.push_back("NE_HDR_ENABLED"); }

			Graphics::API::PixelShader::Create(
				&PostProcess_PShader,
				Graphics::API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl", defines, std::vector<std::string>(), true),
					Graphics::API::ShaderType::Pixel));


			Graphics::API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = Graphics::API::TextureFilter::Point2D;
			Graphics::API::Sampler::Create(&ScreenSampler, Samplerdesc);
		}
		void RenderSystem::SetCamera(Components::CameraComponent * camera)
		{
			this->ActiveCamera = camera;
		}
		Components::CameraComponent* RenderSystem::GetCamera()
		{
			return this->ActiveCamera;
		}
		Graphics::API::VertexShader RenderSystem::GetVertexShader()
		{
			return this->VShader;
		}
		Graphics::API::PixelShader RenderSystem::GetPixelShader()
		{
			return this->PShader;
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
		}
		void RenderSystem::BakePixelShader()
		{
			std::vector<std::string> defines;

			if (this->DirLights.size() > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(DirLights.size())); PSDirty = true; }
			if (this->PointLights.size() > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(PointLights.size()));  PSDirty = true; }
			if (this->SpotLights.size() > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(SpotLights.size()));  PSDirty = true; }
			if (Desc.NormalMaps == true) { defines.push_back("NE_USE_NORMAL_MAPS"); }

			if (PSDirty = true)
			{
				Graphics::API::PixelShader::Delete(&PShader);
				Graphics::API::ConstantBuffer::Delete(&NE_Light_CB);

				Graphics::API::PixelShader::Create(
					&PShader,
					Graphics::API::CompileShader(Core::FileSystem::LoadShader(Desc.PShaderPath, defines, std::vector<std::string>(), true),
						Graphics::API::ShaderType::Pixel));

				Calculate_Light_CB_Size();

				Graphics::API::ConstantBuffer::Create(&NE_Light_CB, "NE_Light_CB", this->NE_Light_CB_Size);

				this->PShader.SetConstantBuffer(&this->NE_Light_CB);
				PSDirty = false;
			}
		}
		void RenderSystem::BakeVertexShader()
		{
			if (VSDirty = true)
			{
				Graphics::API::VertexShader::Delete(&VShader);

				if (Desc.VShaderPath == "NE_Default")
				{
					Managers::AutoVertexShaderDesc VertShaderDesc;
					if (Desc.NormalMaps == true) { VertShaderDesc.InTangents = true; }

					VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
				}
				else {
					Graphics::API::VertexShader::Create(
						&VShader,
						Graphics::API::CompileShader(Core::FileSystem::LoadFileToString(Desc.VShaderPath),
							Graphics::API::ShaderType::Vertex));
				}

				if (this->ActiveCamera != nullptr)
					this->VShader.SetConstantBuffer(&this->ActiveCamera->GetCBuffer());
				else
					Log.Warning("[RenderSystem] Baking the renderer without an active camera!\n");

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
			NE_Light_CB.Update(LightsBuffer.data(), NE_Light_CB_Size);
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
			for (size_t i = 0; i< mesh->SubMeshes.size(); i++)
			{
				material->BindTexSet(mesh->SubMeshes.at(i).data.TexSetIndex);
				mesh->SubMeshes.at(i).VBO.Bind();
				mesh->SubMeshes.at(i).IBO.Bind();
				Graphics::API::Context::DrawIndexed(mesh->SubMeshes.at(i).IndicesCount);
			}
		}
		void RenderSystem::RenderToPostProcessingRT()
		{
			PostProcessRT.Bind();
			Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		}
		void RenderSystem::RenderPostProcessingContents()
		{
			PostProcessRT.Bind_Default();
			Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
			PostProcess_VShader.Bind();
			PostProcess_PShader.Bind();
			ScreenSampler.PSBind(0);
			PostProcessTexture.PSBind(0);
			InstantRender(&PostProcessScreenQuad, nullptr);
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