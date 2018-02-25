#ifndef RenderSystemImpl
#include <Systems\RenderSystem.h>
#else
#include <Core\FileSystem.h>
#include <Shading\Techniques\NoLight.h>
#include <API\ConstantBuffer.h>
#include <API\ShaderCompiler.h>
#include <Core\Context.h>
#include <Components\Transform.h>
#include <Components\Skybox.h>

namespace NuclearEngine
{
	namespace Systems
	{
		RenderSystem::RenderSystem(const RenderSystemDesc & desc)
		{
			Light_Rendering_Tech = nullptr;
			ActiveCamera = &Components::GenericCamera();
			this->Light_Rendering_Tech = desc.Light_Rendering_Tech;

			this->status = RenderSystemStatus::RequireBaking;
		}
		RenderSystem::~RenderSystem()
		{
			Light_Rendering_Tech = nullptr;
			ActiveCamera = nullptr;
		}
		void RenderSystem::SetCamera(Components::GenericCamera * camera)
		{
			this->ActiveCamera = camera;
		}
		API::Shader RenderSystem::GetShader()
		{
			return this->Shader;
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
			std::vector<std::string> includes, defines;
			includes.push_back("Assets/NuclearEngine/Shaders/Renderer/Renderer3D_Header.hlsl");

			if (this->Light_Rendering_Tech != nullptr)
			{
				defines.push_back("NE_LIGHT_SHADING_TECH\n");
				includes.push_back(this->Light_Rendering_Tech->GetShaderPath());

				for (size_t i = 0; i < this->Light_Rendering_Tech->GetDefines().size(); i++)
				{
					defines.push_back(Light_Rendering_Tech->GetDefines().at(i));
				}

			}
			int LightingEnabled = 0;

			if (this->DirLights.size() > 0) { defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(DirLights.size())); LightingEnabled = 1; }
			if (this->PointLights.size() > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(PointLights.size()));  LightingEnabled = 1; }
			if (this->SpotLights.size() > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(SpotLights.size()));  LightingEnabled = 1; }

			std::string SHIT = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl", defines, includes, true);
			API::ShaderDesc desc;
			desc.Name = "RenderSystem";
			API::CompileShader(&desc.VertexShaderCode, Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.vs.hlsl").c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
			API::CompileShader(&desc.PixelShaderCode, Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl", defines, includes, true).c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

			API::Shader::Create(&Shader, &desc);

			Calculate_Light_CB_Size();

			API::ConstantBuffer::Create(&NE_Light_CB, "NE_Light_CB", this->NE_Light_CB_Size);

			if (this->ActiveCamera != nullptr)
			{
				this->Shader.SetConstantBuffer(&this->ActiveCamera->GetCBuffer(), API::ShaderType::Vertex);
			}
			else
			{
				Log.Warning("[RenderSystem] Baking the renderer without an active camera!\n");
			}
			if (LightingEnabled == 1)
			{
				this->Shader.SetConstantBuffer(&this->NE_Light_CB, API::ShaderType::Pixel);
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
		 void RenderSystem::InstantRender(Assets::Mesh * mesh)
		{
			//Lil hack to ensure only one rendering texture is bound
			//TODO: Support Multi-Texture Models
			bool diffusebound = false;
			bool specularbound = false;
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
				skybox.Get()->m_vb.Bind();
				skybox.Get()->m_ds_state.Bind();
				skybox.Get()->m_shader.Bind();
				skybox.Get()->m_texcube.PSBind(0);
				skybox.Get()->m_sampler.PSBind(0);
				Core::Context::Draw(36);
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