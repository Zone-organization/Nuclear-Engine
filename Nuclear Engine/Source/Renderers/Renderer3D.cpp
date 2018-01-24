#include <Renderers\Renderer3D.h>
#include <Core\FileSystem.h>
#include <Shading\Techniques\NoLight.h>
#include <API\ConstantBuffer.h>
#include <API\ShaderCompiler.h>

namespace NuclearEngine {
	namespace Renderers {

		Renderer3D::Renderer3D(Components::GenericCamera * camera)
		{
			this->Camera = camera;
			this->Light_Rendering_Tech = nullptr;
		}
		Renderer3D::~Renderer3D()
		{
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
		void Renderer3D::AddLight(Components::Light* light)
		{
			this->Lights.push_back(light);
		}
		void Renderer3D::Bake()
		{
			std::vector<std::string> includes, defines;
			includes.push_back("Assets/NuclearEngine/Shaders/Renderer/Renderer3D_Textures.hlsl");

			if (this->Light_Rendering_Tech != nullptr)
			{
				defines.push_back("NE_LIGHT_SHADING_TECH\n");
				includes.push_back(this->Light_Rendering_Tech->GetShaderPath());
			}

			if (this->Lights.size() > 0)
			{
				defines.push_back("NE_LIGHTS_NUM " + std::to_string(Lights.size()));
			}
			API::ShaderDesc desc;
			desc.Name = "Renderer3D";
			API::CompileShader(&desc.VertexShaderCode, Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.vs.hlsl").c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
			API::CompileShader(&desc.PixelShaderCode, Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl").c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

			API::Shader::Create(&Shader, &desc);
			
			
			if (this->Lights.size() > 0)
			{
				this->LightUBOSize = sizeof(Math::Vector4) + (this->Lights.size() * sizeof(Components::Internal::Shader_Light_Struct));
			}

			API::ConstantBuffer::Create(&NE_LightUBO,"NE_Light_CB", this->LightUBOSize);

			this->Shader.SetConstantBuffer(&this->Camera->GetCBuffer(),API::ShaderType::Vertex);
			this->Shader.SetConstantBuffer(&this->NE_LightUBO,API::ShaderType::Pixel);
		}

		void Renderer3D::Render_Light()
		{
			std::vector<Math::Vector4> UBOBuffer;
			
			UBOBuffer.push_back(Math::Vector4(this->Camera->GetPosition(), 32.0f));
					

			for (uint i = 0; i < this->Lights.size(); i++)
			{
				UBOBuffer.push_back(this->Lights.at(i)->GetInternalData().Position);
				UBOBuffer.push_back(this->Lights.at(i)->GetInternalData().Direction);
				UBOBuffer.push_back(this->Lights.at(i)->GetInternalData().Intensity_Attenuation);
				UBOBuffer.push_back(this->Lights.at(i)->GetInternalData().InnerCutOf_OuterCutoff);
				UBOBuffer.push_back(this->Lights.at(i)->GetInternalData().Color);
			}
				
			this->NE_LightUBO.Update(UBOBuffer.data(), this->LightUBOSize);
		}
	}
}