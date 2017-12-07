#include <Renderer\Renderer.h>
#include <Core\FileSystem.h>
#include <Core\Context.h>
#include <Core\Engine.h>
#include <Components\Skybox.h>
#include <Components\Light.h>

namespace NuclearEngine {
	
	namespace Core {

		Renderer::Renderer(const Renderer3D_Desc& desc)
		{
			m_desc = desc;
			bakedbefore = false;
		}

		void Renderer::AddLight(Components::Light * light)
		{
			Lights.push_back(light);
			flag = Renderer3DStatusFlag::RequireBaking;
		}
		
		API::Shader * Renderer::GetShader()
		{
			return Renderer_Shader;
		}

		void Renderer::SetLightGlobalAmbient(API::Color globalambient)
		{
			m_globalambient = Math::Vector4(globalambient.r, globalambient.g, globalambient.b, globalambient.a);
		}

		void Renderer::SetCamera(Components::GenericCamera * cam)
		{
			if (cam == nullptr)
			{
				Log->Warning("[Renderer] Setting Camera to a null pointer!\n");
			}

			m_cam = cam;
		}

		void Renderer::Render()
		{
			if (flag == Renderer3DStatusFlag::RequireBaking)
			{
				this->Bake();
			}

			std::vector<Math::Vector4> UBOBuffer;

			UBOBuffer.push_back(Math::Vector4(m_cam->GetPosition(),1.0f));
			UBOBuffer.push_back(m_globalambient);
		
			for (uint i = 0; i < Lights.size(); i++)
			{
				UBOBuffer.push_back(Lights.at(i)->data.Position);
				UBOBuffer.push_back(Lights.at(i)->data.Direction);
				UBOBuffer.push_back(Lights.at(i)->data.Attenuation_Intensity);
				UBOBuffer.push_back(Lights.at(i)->data.InnerCutOf_OuterCutoff);
				UBOBuffer.push_back(Lights.at(i)->data.Color);
			}
			
			NE_LightUBO->Update(UBOBuffer.data(), LightUBOSize);

		}
		void Renderer::Bake()
		{
			if (bakedbefore)
			{
				if (flag != Renderer3DStatusFlag::RequireBaking)
				{
					Log->Warning("[Renderer] Baking the renderer while no changes is recorded!\n");
				}
				delete Renderer_Shader;
				delete NE_LightUBO;
				LightUBOSize = 0;
			}

			if (m_cam == nullptr)
			{
				Log->Warning("[Renderer] Baking Renderer While Camera is Nullptr!\n");
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

		void Renderer::Bake_Forward_Phong()
		{

			std::vector<std::string> defines;
			if (Lights.size() > 0)
			{
				defines.push_back("NR_POINT_LIGHTS " + std::to_string(Lights.size()));
			}
		
			Renderer_Shader = new API::Shader("NE_Phong_Light", 
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.vs.hlsl").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
				&API::CompileShader(Core::FileSystem::LoadShaderWithDefines("Assets/NuclearEngine/Shaders/LightSystem/HLSL/PhongLight.ps.hlsl", defines).c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));
			

			Bake_Uniform_Buffers();
		}

		void Renderer::Bake_Uniform_Buffers()
		{
			unsigned int NECslot = Renderer_Shader->GetUniformBufferSlot(m_cam->GetCBuffer(), ShaderType::Vertex);
			Renderer_Shader->SetUniformBuffer(m_cam->GetCBuffer(), NECslot, ShaderType::Vertex);
			LightUBOSize = sizeof(Math::Vector4);

			if (Lights.size() > 0)
			{
				LightUBOSize = LightUBOSize + (Lights.size() * sizeof(Components::Internal::Shader_Light_Struct));
			}
			
			NE_LightUBO = new API::UniformBuffer("NE_LightUBO", LightUBOSize);
			unsigned int NELslot = Renderer_Shader->GetUniformBufferSlot(NE_LightUBO, ShaderType::Pixel);
			Renderer_Shader->SetUniformBuffer(NE_LightUBO, NELslot, ShaderType::Pixel);
		}

	}
}