#pragma once
#include <Base\NE_PrivateCommon.h>
#include <Core\Path.h>
#include <vector>

namespace NuclearEngine
{
	namespace Core
	{
	
			class NEAPI FileSystem
			{
			public:
				static std::string LoadFileToString(const Path& Filepath);
				static std::string LoadShader(const Path& Filepath, std::vector<std::string> defines, std::vector<std::string> includes = std::vector<std::string>() , bool reverseorder = false);

			};

	}
}