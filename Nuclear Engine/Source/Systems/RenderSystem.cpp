#include <Systems\RenderSystem.h>
#include <Core\FileSystem.h>
#include <Graphics/API/ConstantBuffer.h>
#include <Graphics/API/ShaderCompiler.h>
#include <Graphics\API\Context.h>
#include <Transform\Transform.h>
#include <Managers\ShaderManager.h>
#include <Assets\Material.h>

namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(const RenderSystemDesc & desc)
		{
			Desc = desc;
			ActiveCamera = &Components::CameraComponent();
			this->status = RenderSystemStatus::RequireBaking;
		}
		RenderSystem::~RenderSystem()
		{
			ActiveCamera = nullptr;
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
				&Graphics::API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/PostProcessing.ps.hlsl", defines, std::vector<std::string>(), true),
					Graphics::API::ShaderType::Pixel));


			Graphics::API::SamplerDesc Samplerdesc;
			Samplerdesc.Filter = Graphics::API::TextureFilter::Point2D;
			Graphics::API::Sampler::Create(&ScreenSampler, Samplerdesc);
		}
		void RenderSystem::SetCamera(Components::CameraComponent * camera)
		{
			this->ActiveCamera = camera;
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
					&Graphics::API::CompileShader(Core::FileSystem::LoadShader(Desc.PShaderPath, defines, std::vector<std::string>(), true),
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
						&Graphics::API::CompileShader(Core::FileSystem::LoadFileToString(Desc.VShaderPath),
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

		void RenderSystem::InstantRender(Components::MeshComponent * object)
		{
			if (object->mMesh)
			{
				for (size_t i = 0; i < object->mMesh->SubMeshes.size(); i++)
				{
					InstantRender(&object->mMesh->SubMeshes.at(i));
				}
			}
		}
		void RenderSystem::InstantRender(Assets::Mesh * object)
		{
			for (size_t i = 0; i< object->SubMeshes.size(); i++)
			{
				InstantRender(&object->SubMeshes.at(i));
			}
		}
		void RenderSystem::InstantRender(Assets::Mesh::SubMesh * mesh)
		{
			//Lil and UGLY AND SLOW hack to ensure only one rendering texture is bound
			//TODO: Support Multi-Texture Models
			bool diffusebound = false;
			bool specularbound = false;
			bool normalbound = false;

			for (unsigned int i = 0; i < mesh->data.textures.size(); i++)
			{

				if (mesh->data.textures[i].type == Assets::MeshTextureType::Diffuse)
				{
					if (diffusebound != true)
					{
						mesh->data.textures[i].Texture.PSBind(0);
						diffusebound = true;
					}
				}
				else if (mesh->data.textures[i].type == Assets::MeshTextureType::Specular)
				{
					if (specularbound != true)
					{
						mesh->data.textures[i].Texture.PSBind(1);
						specularbound = true;
					}
				}
				else if (mesh->data.textures[i].type == Assets::MeshTextureType::Normal)
				{
					if (normalbound != true)
					{
						mesh->data.textures[i].Texture.PSBind(2);
						normalbound = true;
					}
				}
			}
			mesh->VBO.Bind();
			mesh->IBO.Bind();
			Graphics::API::Context::DrawIndexed(mesh->IndicesCount);
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
			InstantRender(&PostProcessScreenQuad);
		}
		void RenderSystem::Update(ECS::EntityManager & es, ECS::EventManager & events, ECS::TimeDelta dt)
		{
			ECS::ComponentHandle<Components::MeshComponent> ModelObject;
			for (ECS::Entity entity : es.entities_with_components(ModelObject))
			{
				entity.GetTransform()->Update();
				ActiveCamera->SetModelMatrix(entity.GetTransform()->GetTransformMatrix());
				if (ModelObject.Get()->mMaterial)
				{
					if (ModelObject.Get()->mMaterial->mVShader.isValid)
						ModelObject.Get()->mMaterial->mVShader.Bind();
					//else

					if (ModelObject.Get()->mMaterial->mPShader.isValid)
						ModelObject.Get()->mMaterial->mPShader.Bind();
					//else
				}
				InstantRender(ModelObject.Get());
			}
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