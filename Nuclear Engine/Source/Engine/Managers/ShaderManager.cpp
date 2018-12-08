#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>
#include <Engine/Graphics/ShaderCompiler.h>

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


			auto shadersource = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true);
			
			LLGL::ShaderDescriptor sdesc;
			sdesc.source = shadersource.c_str();
			sdesc.sourceSize = shadersource.size();
			sdesc.sourceType = LLGL::ShaderSourceType::CodeString;
			sdesc.type = LLGL::ShaderType::Vertex;
			result.mShader = Graphics::Context::GetRenderer()->CreateShader(sdesc);

			return result;
		}
		Graphics::Shader ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			Graphics::Shader result;
			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");
			
			auto shadersource = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true);

			LLGL::ShaderDescriptor sdesc;
			sdesc.source = shadersource.c_str();
			sdesc.sourceSize = shadersource.size();
			sdesc.sourceType = LLGL::ShaderSourceType::CodeString;
			sdesc.type = LLGL::ShaderType::Fragment;
			result.mShader = Graphics::Context::GetRenderer()->CreateShader(sdesc);

			return result;
		}
	}
}