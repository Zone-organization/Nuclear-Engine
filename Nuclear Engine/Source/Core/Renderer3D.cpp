#include <Core\Renderer3D.h>
#include <Core\FileSystem.h>
#include <Core\Context.h>
#include <Core\Engine.h>
#include <Components\Skybox.h>
#include <Components\Light.h>

namespace NuclearEngine {
	
	namespace Core {

		Renderer3D::Renderer3D(const Renderer3D_Desc& desc)
		{
			m_desc = desc;
			bakedbefore = false;
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
			return Renderer_Shader;
		}

		void Renderer3D::SetCamera(Components::GenericCamera * cam)
		{
			if (cam == nullptr)
			{
				Log->Warning("[Renderer3D] Setting Camera to a null pointer!\n");
			}

			m_cam = cam;
		}

		void Renderer3D::Render()
		{
			if (flag == Renderer3DStatusFlag::RequireBaking)
			{
				this->Bake();
			}

			std::vector<Math::Vector4> UBOBuffer;

			UBOBuffer.push_back(Math::Vector4(m_cam->GetPosition(), 32.0f));

			for (uint i = 0; i < dirLights.size(); i++)
			{
				UBOBuffer.push_back(dirLights.at(i)->data.m_dir);
				UBOBuffer.push_back(dirLights.at(i)->data.m_color);
			}
			
			for (uint i = 0; i < pointLights.size(); i++)
			{
				UBOBuffer.push_back(pointLights.at(i)->data.m_pos);
				UBOBuffer.push_back(pointLights.at(i)->data.m_intensity_att);
				UBOBuffer.push_back(pointLights.at(i)->data.m_color);
			} 
			
			for (uint i = 0; i < spotLights.size(); i++)
			{
				UBOBuffer.push_back(spotLights.at(i)->data.m_pos);
				UBOBuffer.push_back(spotLights.at(i)->data.m_dir);
				UBOBuffer.push_back(spotLights.at(i)->data.m_intensity_att);
				UBOBuffer.push_back(spotLights.at(i)->data.m_cutoff_outercutoff);
				UBOBuffer.push_back(spotLights.at(i)->data.m_color);
			}
			
			NE_LightUBO->Update(UBOBuffer.data(), LightUBOSize);

		}
		void Renderer3D::Bake()
		{
			if (bakedbefore)
			{
				if (flag != Renderer3DStatusFlag::RequireBaking)
				{
					Log->Warning("[Renderer3D] Baking the renderer while no changes is recorded!\n");
				}
				delete Renderer_Shader;
				delete NE_LightUBO;
				LightUBOSize = 0;
			}

			if (m_cam == nullptr)
			{
				Log->Warning("[Renderer3D] Baking Renderer While Camera is Nullptr!\n");
			}

			if (m_desc.lightmodel == LightShading::PhongShading)
			{			
				if (m_desc.tech == RenderingTechnique::Forward)
				{
					Bake_Forward_Phong();
				}
			}

			flag = Renderer3DStatusFlag::Ready;

			bakedbefore = true;
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

							
			Renderer_Shader = new API::Shader("NE_Phong_Light", 
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.vs.hlsl").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
				&API::CompileShader(Core::FileSystem::LoadShaderWithDefines("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.ps.hlsl", defines).c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));
			

			Bake_Uniform_Buffers();
		}

		void Renderer3D::Bake_Uniform_Buffers()
		{
			unsigned int NECslot = Renderer_Shader->GetUniformBufferSlot(m_cam->GetCBuffer(), ShaderType::Vertex);
			Renderer_Shader->SetUniformBuffer(m_cam->GetCBuffer(), NECslot, ShaderType::Vertex);
			LightUBOSize = sizeof(Math::Vector4);

			if (dirLights.size() > 0)
			{
				LightUBOSize = LightUBOSize + (dirLights.size() * sizeof(Components::Internal::_DirLight));
			}
			if (pointLights.size() > 0)
			{
				LightUBOSize = LightUBOSize + (pointLights.size() * sizeof(Components::Internal::_PointLight));
			}
			if (spotLights.size() > 0)
			{
				LightUBOSize = LightUBOSize + (spotLights.size() * sizeof(Components::Internal::_SpotLight));
			}

			NE_LightUBO = new API::UniformBuffer("NE_LightUBO", LightUBOSize);
			unsigned int NELslot = Renderer_Shader->GetUniformBufferSlot(NE_LightUBO, ShaderType::Pixel);
			Renderer_Shader->SetUniformBuffer(NE_LightUBO, NELslot, ShaderType::Pixel);
		}

	}
}