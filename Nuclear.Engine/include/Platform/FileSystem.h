#pragma once
#include <Core\Path.h>
#include <set>

namespace Nuclear
{
	namespace Platform
	{
		class NEAPI FileSystem
		{
		public:
			static FileSystem& GetInstance();

			FileSystem(const FileSystem&) = delete;
			FileSystem& operator= (const FileSystem) = delete;

			std::string LoadFileToString(const Core::Path& Filepath);

			std::string LoadShader(const Core::Path& Filepath);

			//std::string LoadScript(const Path& Filepath, std::vector<std::string> defines, std::vector<std::string> includes = std::vector<std::string>(), bool reverseorder = false);
			std::string LoadShader(const Core::Path& Filepath, std::set<std::string> defines, std::set<std::string> includes = std::set<std::string>(), bool reverseorder = false);
	
			bool SaveBinaryBuffer(const std::vector<Uint8>& buffer, const Core::Path& Filepath);

			bool LoadBinaryBuffer(std::vector<Uint8>& buffer, const Core::Path& Filepath);
		private:
			FileSystem();
		};
	}
}