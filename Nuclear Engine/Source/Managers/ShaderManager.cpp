#include <Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <API\ShaderCompiler.h>
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
		API::VertexShader ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc)
		{
			API::VertexShader result;
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

			API::VertexShader::Create(
				&result,
				&API::CompileShader(Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true).c_str(),
					API::ShaderType::Vertex,
					API::ShaderLanguage::HLSL));

			return result;
		}
	}
}