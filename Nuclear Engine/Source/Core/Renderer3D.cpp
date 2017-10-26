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
			dirLights.reserve(1);
			pointLights.reserve(1);
			spotLights.reserve(1);

			m_desc = desc;
			this->SetCamera(cam);
			initialized = false;
			this->Bake();
			initialized = true;
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

			m_lightubo->Update(&Math::Vector4(m_cam->GetPosition(), 32.0f), 0, sizeof(Math::Vector4));

			uint point = 0;
			for (uint i = 0; i < dirLights.size(); i++)
			{
				m_lightubo->Update(&dirLights.at(i)->data,
					sizeof(Math::Vector4) + sizeof(Components::Internal::_DirLight) * point,
					sizeof(Components::Internal::_DirLight));

				point++;
			}
			
			point = 0;
			for (uint i = 0; i < pointLights.size(); i++)
			{
				m_lightubo->Update(&pointLights.at(i)->data,
					(sizeof(Math::Vector4) + dirLights.size() * sizeof(Components::Internal::_DirLight)) + sizeof(Components::Internal::_PointLight) * point,
					sizeof(Components::Internal::_PointLight));

				point++;
			} 
			
			point = 0;
			for (uint i = 0; i < spotLights.size(); i++)
			{
				m_lightubo->Update(&spotLights.at(i)->data,
					(sizeof(Math::Vector4) + dirLights.size() * sizeof(Components::Internal::_DirLight) + pointLights.size() * sizeof(Components::Internal::_PointLight)) + +sizeof(Components::Internal::_SpotLight) * point,
					sizeof(Components::Internal::_SpotLight));
			}

		}
		void Renderer3D::Bake()
		{
			lightubosize = sizeof(Math::Vector4) + dirLights.size() * sizeof(Components::Internal::_DirLight) + pointLights.size() * sizeof(Components::Internal::_PointLight) + spotLights.size() * sizeof(Components::Internal::_SpotLight);

			if (initialized)
			{
				if (flag != Renderer3DStatusFlag::RequireBaking)
				{
					Log->Warning("[Renderer3D] Baking the renderer while no changes is recorded!\n");
				}
				delete m_shader;
				delete m_lightubo;
			}
			if (m_desc.lightmodel == LightShading::PhongShading)
			{			
				std::vector<std::string> defines;
				defines.push_back("NR_POINT_LIGHTS " + std::to_string(pointLights.capacity()));
				defines.push_back("NR_SPOT_LIGHTS " + std::to_string(spotLights.capacity()));
				defines.push_back("NR_DIR_LIGHTS " + std::to_string(dirLights.capacity()));
				
				if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					m_shader = new API::Shader("NE_Phong_Light",
						Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/LightSystem/GLSL/PhongLight.vs").c_str(),
						Core::FileSystem::LoadShaderWithDefines("Assets/NuclearEngine/Shaders/LightSystem/GLSL/PhongLight.fs", defines).c_str(),
						nullptr,
						ShaderType::GLSL);
				}
				else if (Core::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					m_shader = new API::Shader("NE_Phong_Light",
						Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.vs").c_str(),
						Core::FileSystem::LoadShaderWithDefines("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.ps", defines).c_str(),
						nullptr,
						ShaderType::HLSL);
				}			
				
				m_shader->SetUniformBuffer(m_cam->GetCBuffer());

				m_lightubo = new API::UniformBuffer("NE_LightUBO", lightubosize);
				m_shader->SetUniformBuffer(m_lightubo);
			}

			flag = Renderer3DStatusFlag::Ready;
			return;
		}
	}
}