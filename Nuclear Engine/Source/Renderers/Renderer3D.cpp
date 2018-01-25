#include <Renderers\Renderer3D.h>
#include <Core\FileSystem.h>
#include <Shading\Techniques\NoLight.h>
#include <API\ConstantBuffer.h>
#include <API\ShaderCompiler.h>

namespace NuclearEngine {
	namespace Renderers {

		Renderer3D::Renderer3D()
		{
			this->Light_Rendering_Tech = nullptr;
		}
		Renderer3D::~Renderer3D()
		{
		}
	
		void Renderer3D::SetCamera(Components::GenericCamera * camera)
		{
			this->Camera = camera;
		}

		void Renderer3D::SetTechnique(Shading::Technique * Tech)
		{
			switch (Tech->GetType())
			{
			case Shading::Technique_Type::LightShading:
				this->Light_Rendering_Tech = Tech;
			}

			this->flag = Renderer3DStatusFlag::RequireBaking;
		}
		API::Shader Renderer3D::GetShader()
		{
			return this->Shader;
		}
		void Renderer3D::AddLight(Components::DirectionalLight * light)
		{
			DirLights.push_back(light);
		}
		void Renderer3D::AddLight(Components::PointLight * light)
		{
			PointLights.push_back(light);
		}
		void Renderer3D::AddLight(Components::SpotLight * light)
		{
			SpotLights.push_back(light);
		}

		void Renderer3D::Bake()
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

			if (this->DirLights.size() > 0)	{	defines.push_back("NE_DIR_LIGHTS_NUM " + std::to_string(DirLights.size()));	}
			if (this->PointLights.size() > 0) { defines.push_back("NE_POINT_LIGHTS_NUM " + std::to_string(PointLights.size())); }
			if (this->SpotLights.size() > 0) { defines.push_back("NE_SPOT_LIGHTS_NUM " + std::to_string(SpotLights.size())); }

			std::string SHIT = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl", defines, includes);
			API::ShaderDesc desc;
			desc.Name = "Renderer3D";
			API::CompileShader(&desc.VertexShaderCode, Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.vs.hlsl").c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
			API::CompileShader(&desc.PixelShaderCode, Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl",defines, includes).c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

			API::Shader::Create(&Shader, &desc);

			Calculate_Light_CB_Size();

			API::ConstantBuffer::Create(&NE_Light_CB, "NE_Light_CB", this->NE_Light_CB_Size);

			if (this->Camera != nullptr)
			{
				this->Shader.SetConstantBuffer(&this->Camera->GetCBuffer(), API::ShaderType::Vertex);
			}
			else 
			{
				Log->Warning("[Renderer3D] Baking the renderer without an active camera!\n");
			}
			//this->Shader.SetConstantBuffer(&this->NE_Light_CB,API::ShaderType::Pixel);
		}

		void Renderer3D::Render_Light()
		{
			
			
				
			//this->NE_LightUBO.Update( this->LightUBOSize);
		}
		void Renderer3D::Calculate_Light_CB_Size()
		{
			NE_Light_CB_Size = sizeof(Math::Vector4);
			if (this->DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
			}
			if (this->PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
			}
			if (this->SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (this->SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
			}
		}
	}
}