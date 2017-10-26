#pragma once
#include "NE_Common.h"
#include <vector>
namespace NuclearEngine
{
	namespace Core
	{
			struct NEAPI FileSystem
			{
				static std::string LoadFileToString(std::string Filepath);

				static std::string LoadGLSLShaderWithDefines(std::string Filepath, std::vector<std::string> defines);

				static char* LoadBinaryFile(std::string Filepath, size_t* size);
			};

	}
}