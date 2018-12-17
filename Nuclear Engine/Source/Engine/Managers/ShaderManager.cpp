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

			result.mShader = Graphics::Context::GetRenderer()->CreateShader(CreateShaderDescFromBlob(Graphics::ShaderCompiler::CompileShader(
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoVertexShader.hlsl", defines, std::vector<std::string>(), true),
				LLGL::ShaderType::Vertex), LLGL::ShaderType::Vertex));

			return result;
		}
		Graphics::Shader ShaderManager::CreateAutoPixelShader(const AutoPixelShaderDesc & desc)
		{
			Graphics::Shader result;
			std::vector<std::string> defines;

			if (desc.OutputTexture)
				defines.push_back("NE_OUTPUT_TEXTURE");
			
			result.mShader = Graphics::Context::GetRenderer()->CreateShader(CreateShaderDescFromBlob(Graphics::ShaderCompiler::CompileShader(
				Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/ShaderManager/AutoPixelShader.hlsl", defines, std::vector<std::string>(), true),
				LLGL::ShaderType::Fragment), LLGL::ShaderType::Fragment));

			return result;
		}
		LLGL::ShaderDescriptor ShaderManager::CreateShaderDescFromBlob(const Graphics::BinaryShaderBlob & blob, const LLGL::ShaderType& type)
		{
			LLGL::ShaderDescriptor sdesc;
			sdesc.type = type;

			const auto& languages = Graphics::Context::GetRenderer()->GetRenderingCaps().shadingLanguages;

			if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::GLSL) != languages.end())
			{
				sdesc.source = blob.SourceCode.c_str();
				sdesc.sourceSize = blob.SourceCode.size();
				sdesc.sourceType = LLGL::ShaderSourceType::CodeString;
			}
			/*else if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::SPIRV) != languages.end())
			{
			}*/
			else if (std::find(languages.begin(), languages.end(), LLGL::ShadingLanguage::HLSL) != languages.end())
			{
				sdesc.source = static_cast<const char*>(blob.DXBC_SourceCode.Buffer);
				sdesc.sourceSize = blob.DXBC_SourceCode.Size;
				sdesc.sourceType = LLGL::ShaderSourceType::BinaryBuffer;
			}
			return sdesc;
		}
	}
}