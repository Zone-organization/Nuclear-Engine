#pragma once
#include <Base\NE_Common.h>
#include <vector>
#include <Engine/Graphics/ShaderTypes.h>
#include <LLGL/ShaderFlags.h>

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

			NEAPI BinaryShaderBlob CompileShader(const ShaderLoadDesc& desc, const LLGL::ShaderType& type);

			NEAPI bool CompileShader(BinaryShaderBlob* result, std::string SourceCode, const LLGL::ShaderType& type);

			NEAPI BinaryShaderBlob CompileShader(std::string SourceCode, const LLGL::ShaderType& type);
		}
	}
}