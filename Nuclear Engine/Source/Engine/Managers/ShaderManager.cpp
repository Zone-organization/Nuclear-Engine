#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine/Graphics/ShaderCompiler.h>
#include <Engine\Graphics\Context.h>

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
		Graphics::Shader ShaderManager::CreateAutoVertexShader(const AutoVertexShaderDesc & desc)
		{
			Graphics::Shader result;

			result.mVSFormat.AppendAttribute({ "Position", LLGL::Format::RGBA32Float });
			
			std::vector<std::string> defines;

			if (desc.InTexCoords)
			{
				defines.push_back("NE_USE_UV");
				result.mVSFormat.AppendAttribute({ "TexCoord", LLGL::Format::RG32Float });
			}
			if (desc.InNormals)
			{
				defines.push_back("NE_USE_NORMALS");
				result.mVSFormat.AppendAttribute({ "Normals", LLGL::Format::RGB32Float });
			}
			if (desc.InTangents)
			{
				defines.push_back("NE_USE_TANGENTS");
				result.mVSFormat.AppendAttribute({ "Tangents", LLGL::Format::RGB32Float });
			}
			if (desc.Use_Camera)
			{
				defines.push_back("NE_USE_DEF_CAMERA");
			}
			if (desc.OutFragPos)
			{
				defines.push_back("NE_OUT_FRAG_POS");
			}

			Graphics::ShaderCompiler::CompileAndCreateShader(&result,
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true),
				LLGL::ShaderType::Vertex);

			return result;
		}
		Graphics::Shader ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			Graphics::Shader result;
			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");
			

			Graphics::ShaderCompiler::CompileAndCreateShader(&result,
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
				LLGL::ShaderType::Fragment);
			return result;
		}
	}
}