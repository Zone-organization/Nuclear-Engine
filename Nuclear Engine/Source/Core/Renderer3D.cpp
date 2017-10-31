#include <Core\Renderer3D.h>
#include <Core\FileSystem.h>
#include <Core\Context.h>
#include <Core\Engine.h>
#include <Components\Skybox.h>
#include <Components\Light.h>

namespace NuclearEngine {
	
	namespace Core {

		Renderer3D::Renderer3D(const Renderer3D_Desc& desc, Components::GenericCamera *cam)
		{
			m_desc = desc;
			this->SetCamera(cam);
			/*initialized = false;
			this->Bake();
			initialized = true;*/
		}


		void Renderer3D::AddLight(Components::DirectionalLight * light)
		{
			dirLights.push_back(light);
			flag = Renderer3DStatusFlag::RequireBaking;
		}
		void Renderer3D::AddLight(Components::PointLight * light)
		{
			pointLights.push_back(light);
			flag = Renderer3DStatusFlag::RequireBaking;
		}
		void Renderer3D::AddLight(Components::SpotLight * light)
		{
			spotLights.push_back(light);
			flag = Renderer3DStatusFlag::RequireBaking;
		}

		API::Shader * Renderer3D::GetShader()
		{
			return m_shader;
		}

		void Renderer3D::SetCamera(Components::GenericCamera * cam)
		{
			m_cam = cam;
		}

		void Renderer3D::Render()
		{
			if (flag == Renderer3DStatusFlag::RequireBaking)
			{
				this->Bake();
			}

			GlobalUBO->Update(&Math::Vector4(m_cam->GetPosition(), 32.0f), sizeof(Math::Vector4));

			for (uint i = 0; i < dirLights.size(); i++)
			{
				DirLightUBO->Update(&dirLights.at(i)->data, sizeof(Components::Internal::_DirLight));
			}
			
			for (uint i = 0; i < pointLights.size(); i++)
			{
				PointLightUBO->Update(&pointLights.at(i)->data, sizeof(Components::Internal::_PointLight));
			} 
			
			for (uint i = 0; i < spotLights.size(); i++)
			{
				SpotLightUBO->Update(&spotLights.at(i)->data, sizeof(Components::Internal::_SpotLight));
			}

		}
		void Renderer3D::Bake()
		{
			/*if (initialized)
			{
				if (flag != Renderer3DStatusFlag::RequireBaking)
				{
					Log->Warning("[Renderer3D] Baking the renderer while no changes is recorded!\n");
				}
				delete m_shader;
				delete GlobalUBO;
				delete DirLightUBO;			
				delete PointLightUBO;
				delete SpotLightUBO;
			}*/
			if (m_desc.lightmodel == LightShading::PhongShading)
			{			
				if (m_desc.tech == RenderingTechnique::Forward)
				{
					Bake_Forward_Phong();
				}
			}

			flag = Renderer3DStatusFlag::Ready;
			return;
		}

		void Renderer3D::Bake_Forward_Phong()
		{

			std::vector<std::string> defines;
			if (pointLights.size() > 0)
			{
				defines.push_back("NR_POINT_LIGHTS " + std::to_string(pointLights.size()));
			}
			if (spotLights.size() > 0)
			{
				defines.push_back("NR_SPOT_LIGHTS " + std::to_string(spotLights.size()));
			}
			if (dirLights.size() > 0)
			{
				defines.push_back("NR_DIR_LIGHTS " + std::to_string(dirLights.size()));
			}

			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				m_shader = new API::Shader("NE_Phong_Light",
					Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/LightSystem/GLSL/PhongLight.vs").c_str(),
					Core::FileSystem::LoadShaderWithDefines("Assets/NuclearEngine/Shaders/LightSystem/GLSL/PhongLight.fs", defines).c_str(),
					nullptr,
					ShaderLanguage::GLSL);
			}
			else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
			{
				m_shader = new API::Shader("NE_Phong_Light",
					Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.vs").c_str(),
					Core::FileSystem::LoadShaderWithDefines("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.ps", defines).c_str(),
					nullptr,
					ShaderLanguage::HLSL);
			}

			Bake_Uniform_Buffers();
		}

		void Renderer3D::Bake_Uniform_Buffers()
		{

			m_shader->SetUniformBuffer(m_cam->GetCBuffer(), 0, ShaderType::Vertex);

			/*
			Pixel Shader Slots:
			0:Global
			1:DirLightUBO
			2:PointLightUBO
			3:SpotLightUBO
			*/

			GlobalUBO = new API::UniformBuffer("NE_GlobalUBO", sizeof(Math::Vector4));
			m_shader->SetUniformBuffer(DirLightUBO, 0, ShaderType::Pixel);

			if (dirLights.size() > 0)
			{
				DirLightUBO = new API::UniformBuffer("NE_DirLightUBO", dirLights.size() * sizeof(Components::Internal::_DirLight));
				m_shader->SetUniformBuffer(DirLightUBO, 1, ShaderType::Pixel);
			}
			if (pointLights.size() > 0)
			{
				PointLightUBO = new API::UniformBuffer("NE_PointLightUBO", pointLights.size() * sizeof(Components::Internal::_PointLight));
				m_shader->SetUniformBuffer(PointLightUBO, 2, ShaderType::Pixel);
			}
			if (spotLights.size() > 0)
			{
				SpotLightUBO = new API::UniformBuffer("NE_SpotLightUBO", spotLights.size() * sizeof(Components::Internal::_SpotLight));
				m_shader->SetUniformBuffer(SpotLightUBO, 3, ShaderType::Pixel);
			}
		}

	}
}