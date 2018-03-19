#include <Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Graphics/API/ShaderCompiler.h>
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
		Graphics::API::VertexShader ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc)
		{
			Graphics::API::VertexShader result;
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

			Graphics::API::VertexShader::Create(
				&result,
				&Graphics::API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true),
					Graphics::API::ShaderType::Vertex));

			return result;
		}
		Graphics::API::PixelShader ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			Graphics::API::PixelShader result;
			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");

			Graphics::API::PixelShader::Create(
				&result,
				&Graphics::API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
					Graphics::API::ShaderType::Pixel));
			return result;
		}
	}
}