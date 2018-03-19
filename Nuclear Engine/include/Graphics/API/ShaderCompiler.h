#pragma once
#include <NE_Common.h>
#include <vector>
#include <Graphics/API/Shader_Types.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct ShaderLoadDesc
			{
				ShaderLoadDesc(std::string aPath)
				{
					Path = aPath;
				}
				std::string Path;
				std::vector<std::string> Includes;
				std::vector<std::string> Defines;
				bool ReverseIncludeOrder = false;
			};

			NEAPI BinaryShaderBlob CompileShader(const ShaderLoadDesc& desc, const Graphics::API::ShaderType& type);

			NEAPI bool CompileShader(BinaryShaderBlob* result, std::string SourceCode, const Graphics::API::ShaderType& type);

			NEAPI BinaryShaderBlob CompileShader(std::string SourceCode, const Graphics::API::ShaderType& type);
		}
	}
}