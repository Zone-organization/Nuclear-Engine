#pragma once
#include <Base\NE_Common.h>
#include <vector>
namespace NuclearEngine
{
	namespace Core
	{
	
			class NEAPI FileSystem
			{
			public:
				static std::string LoadFileToString(std::string Filepath);

				static std::string LoadShader(std::string Filepath, std::vector<std::string> defines, std::vector<std::string> includes, bool reverseorder =false);
			};

	}
}