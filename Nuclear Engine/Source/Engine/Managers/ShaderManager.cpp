#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine/Graphics/API/ShaderCompiler.h>
namespace NuclearEngine
{
	namespace Managers
	{
		ShaderManager::ShaderManager()
		{
		}
		ShaderManager::~ShaderManager()
		{
		}
		LLGL::Shader* ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc)
		{
			LLGL::Shader* result;
			std::vector<std::string> defines;

			if (desc.InTexCoords)
				defines.push_back("NE_USE_UV");
			if (desc.InNormals)
				defines.push_back("NE_USE_NORMALS");
			if (desc.InTangents)
				defines.push_back("NE_USE_TANGENTS");
			if (desc.Use_Camera)
				defines.push_back("NE_USE_DEF_CAMERA");
			if (desc.OutFragPos)
				defines.push_back("NE_OUT_FRAG_POS");

			LLGL::Shader*::Create(
				&result,
				LLGL::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true),
					LLGL::ShaderType::Vertex));

			return result;
		}
		LLGL::Shader* ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			LLGL::Shader* result;
			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");

			LLGL::Shader*::Create(
				&result,
				LLGL::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
					LLGL::ShaderType::Pixel));
			return result;
		}
	}
}