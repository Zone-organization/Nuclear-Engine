#include <Renderers\Renderer3D.h>
#include <Core\FileSystem.h>
#include <Shading\Techniques\NoLight.h>

namespace NuclearEngine {
	namespace Renderers {
		namespace Members
		{
			API::Shader *Shader;
			Shading::Technique* Light_Rendering_Tech;
			Components::GenericCamera* Camera;
			bool Init = false;
			bool Reload_Notif = false;
		}

		bool Renderer3D::Initialize(Components::GenericCamera* camera)
		{
			Members::Camera = camera;
			Members::Light_Rendering_Tech = nullptr;
			Members::Init = true;
			return Members::Init;
		}
		void Renderer3D::SetTechnique(Shading::Technique * Tech)
		{
			switch (Tech->GetType())
			{
			case Shading::Technique_Type::LightShading:
				Members::Light_Rendering_Tech = Tech;
			}

			Members::Reload_Notif = true;
		}
		API::Shader * Renderer3D::GetShader()
		{
			return Members::Shader;
		}
		void Renderer3D::Reload()
		{
			std::vector<std::string> includes, defines;
			includes.push_back("Assets/NuclearEngine/Shaders/Renderer/Renderer3D_Textures.hlsl");

			if (Members::Light_Rendering_Tech != nullptr)
			{
				defines.push_back("NE_LIGHT_SHADING_TECH\n");
				includes.push_back(Members::Light_Rendering_Tech->GetShaderPath());
			}

			Members::Shader = new API::Shader("Renderer3D_Shader",
				&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.vs.hlsl").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
				&API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl", defines, includes).c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));
			
			std::cout << Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Renderer/Renderer3D.ps.hlsl", defines, includes);

			Members::Shader->SetUniformBuffer(Members::Camera->GetCBuffer(), Members::Shader->GetUniformBufferSlot(Members::Camera->GetCBuffer(),ShaderType::Vertex), ShaderType::Vertex);

		}
		void Renderer3D::Render_Light()
		{
		}
	}
}