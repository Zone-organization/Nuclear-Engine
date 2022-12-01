#pragma once
#include <Core\Path.h>
#include <set>

namespace Nuclear
{
	namespace Core
	{
		class FileSystem
		{
		public:
			static std::string LoadFileToString(const Path& Filepath);

			static std::string LoadShader(const Path& Filepath);

			//static std::string LoadScript(const Path& Filepath, std::vector<std::string> defines, std::vector<std::string> includes = std::vector<std::string>(), bool reverseorder = false);
			static std::string LoadShader(const Path& Filepath, std::set<std::string> defines, std::set<std::string> includes = std::set<std::string>(), bool reverseorder = false);

		};
	}
}