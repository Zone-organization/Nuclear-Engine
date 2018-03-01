#ifndef RenderSystemImpl
#include <Systems\RenderSystem.h>
#else
#include <Core\FileSystem.h>
#include <API\ConstantBuffer.h>
#include <API\ShaderCompiler.h>
#include <Core\Context.h>
#include <Components\Transform.h>
#include <Components\Skybox.h>
#include <Components\ModelRenderDesc.h>
#include <Managers\ShaderManager.h>


namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(const RenderSystemDesc & desc)
		{
			Desc = desc;
			ActiveCamera = &Components::GenericCamera();
			this->status = RenderSystemStatus::RequireBaking;
		}
		RenderSystem::~RenderSystem()
		{
			ActiveCamera = nullptr;
		}
		void RenderSystem::InitializePostProcessing(unsigned int WindowWidth, unsigned int WindowHeight)
		{
			API::Texture_Desc ScreenTexDesc;
			ScreenTexDesc.Format = API::Format::R16G16B16A16_FLOAT;
			ScreenTexDesc.Type = API::TextureType::Texture2D;
			ScreenTexDesc.GenerateMipMaps = false;

			API::Texture_Data Data;
			Data.Img_Data_Buf = NULL;
			Data.Width = WindowWidth;
			Data.Height = WindowHeight;
			API::Texture::Create(&PostProcessTexture, Data, ScreenTexDesc);

			API::RenderTarget::Create(&PostProcessRT);
			PostProcessRT.AttachTexture(&PostProcessTexture);
			PostProcessRT.AttachDepthStencilBuffer(Math::Vector2ui(WindowWidth, WindowHeight));

			Managers::AutoVertexShaderDesc VertShaderDesc;
			VertShaderDesc.Use_Camera = false;
			VertShaderDesc.InNormals = false;
			VertShaderDesc.OutFragPos = false;

			PostPrcoess_VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
			PostProcessScreenQuad.Initialize(&PostPrcoess_VShader);
			std::vector<std::string> defines;
			if (Desc.GammaCorrection == true) { defines.push_back("NE_GAMMA_CORRECTION"); }
			if (Desc.HDR == true) { defines.push_back("NE_HDR_ENABLED"); }

			API::PixelShader::Create(
				&PostPrcoess_PShader,
				&API::CompileShader(Core::FileSystem::LoadShader(Desc.PShaderPath, defines, std::vector<std::string>(), true),
					API::ShaderType::Pixel,
					API::ShaderLanguage::HLSL));
		}
		void RenderSystem::SetCamera(Components::GenericCamera * camera)
		{
			this->ActiveCamera = camera;
		}
		API::VertexShader RenderSystem::GetVertexShader()
		{
			return this->VShader;
		}
		API::PixelShader RenderSystem::GetPixelShader()
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

			if (this->DirLights.size() > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(DirLights.size())); PSDirty = true;	}
			if (this->PointLights.size() > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(PointLights.size()));  PSDirty = true; }
			if (this->SpotLights.size() > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(SpotLights.size()));  PSDirty = true; }
			if (Desc.NormalMaps == true) { defines.push_back("NE_USE_NORMAL_MAPS"); }

			if (PSDirty = true)
			{
				API::PixelShader::Delete(&PShader);
				API::ConstantBuffer::Delete(&NE_Light_CB);

				API::PixelShader::Create(
					&PShader,
					&API::CompileShader(Core::FileSystem::LoadShader(Desc.PShaderPath, defines, std::vector<std::string>(), true),
						API::ShaderType::Pixel,
						API::ShaderLanguage::HLSL));

				Calculate_Light_CB_Size();

				API::ConstantBuffer::Create(&NE_Light_CB, "NE_Light_CB", this->NE_Light_CB_Size);

				this->PShader.SetConstantBuffer(&this->NE_Light_CB);
				PSDirty = false;
			}
		}
		void RenderSystem::BakeVertexShader()
		{
			if (VSDirty = true)
			{
				API::VertexShader::Delete(&VShader);

				if (Desc.VShaderPath == "NE_Default")
				{
					Managers::AutoVertexShaderDesc VertShaderDesc;
					if (Desc.NormalMaps == true) { VertShaderDesc.InTangents = true; }

					VShader = Managers::ShaderManager::CreateAutoVertexShader(VertShaderDesc);
				}
				else {
					API::VertexShader::Create(
						&VShader,
						&API::CompileShader(Core::FileSystem::LoadFileToString(Desc.VShaderPath),
							API::ShaderType::Vertex,
							API::ShaderLanguage::HLSL));
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

		void RenderSystem::InstantRender(Components::Model * object)
		{
			
			for (size_t i = 0; i< object->GetAsset()->Meshes.size(); i++)
			{	
				InstantRender(&object->GetAsset()->Meshes.at(i));
			}
		}
		 void RenderSystem::InstantRender(XAsset::Mesh * mesh)
		{
			//Lil hack to ensure only one rendering texture is bound
			//TODO: Support Multi-Texture Models
			bool diffusebound = false;
			bool specularbound = false;
			bool normalbound = false;

			for (unsigned int i = 0; i < mesh->data.textures.size(); i++)
			{

				if (mesh->data.textures[i].type == XAsset::MeshTextureType::Diffuse)
				{
					if (diffusebound != true)
					{
						mesh->data.textures[i].Texture.PSBind(0);
						diffusebound = true;
					}
				}
				else if (mesh->data.textures[i].type == XAsset::MeshTextureType::Specular)
				{
					if (specularbound != true)
					{
						mesh->data.textures[i].Texture.PSBind(1);
						specularbound = true;
					}
				}
				else if (mesh->data.textures[i].type == XAsset::MeshTextureType::Normal)
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
			Core::Context::DrawIndexed(mesh->IndicesCount);
		}
		void RenderSystem::Update(Core::EntityManager & es, Core::EventManager & events, Core::TimeDelta dt)
		{
			Core::ComponentHandle<Components::Model> ModelObject;
			for (Core::Entity entity : es.entities_with_components(ModelObject))
			{
				auto transform = entity.GetComponent<Components::Transform>();
					
				if (transform)
				{
					transform.Get()->Update();
					ActiveCamera->SetModelMatrix(transform.Get()->GetTransform());
				}

				InstantRender(ModelObject.Get());
			}

			Core::ComponentHandle<Components::Skybox> skybox;
			for (Core::Entity entity : es.entities_with_components(skybox))
			{
				auto renderdesc = entity.GetComponent<Components::ModelRenderDesc>();

				if (renderdesc.Get()->Render == true)
				{
					skybox.Get()->m_vb.Bind();
					skybox.Get()->m_ds_state.Bind();
					skybox.Get()->v_shader.Bind();
					skybox.Get()->p_shader.Bind();
					skybox.Get()->m_texcube.PSBind(0);
					skybox.Get()->m_sampler.PSBind(0);
					Core::Context::Draw(36);
				}
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
#endif