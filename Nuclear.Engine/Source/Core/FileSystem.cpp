#include "Core\FileSystem.h"
#include <Core\Logger.h>
#include <fstream>

namespace Nuclear
{
	namespace Core
	{
			std::string FileSystem::LoadFileToString(const Path& Filepath)
			{
				std::ifstream file(Filepath.GetRealPath(), std::ios::in);
				std::string data = "", line = "";

				if (file.is_open())
				{
					while (std::getline(file, line))
						data += line + "\n";
				}
				else
				{
					NUCLEAR_ERROR("[FileSystem] Couldn't Read File: '{0}'", Filepath.GetRealPath());
					return std::string("");
				}
				return data;
			}

			std::string FileSystem::LoadShader(const Path& Filepath)
			{
				return LoadFileToString(Filepath);
			}

			std::string FileSystem::LoadShader(const Path& Filepath, std::set<std::string> defines, std::set<std::string> includes, bool reverseorder)
			{
				std::ifstream file(Filepath.GetRealPath(), std::ios::in);
				std::string data = "", line = "";

				if (file.is_open())
				{
					while (std::getline(file, line))
					{
						data += line + "\n";
					}
				}
				else
				{
					NUCLEAR_ERROR("[FileSystem] Couldn't Read Shader File: '{0}'", Filepath.GetRealPath());
					return std::string();
				}
				if (reverseorder == false)
				{
					if (defines.size() > 0)
					{
						std::vector<std::string> MergedCode;

						std::string firstLine = data.substr(0, data.find("\n"));
						//If GLSL
						if (firstLine.find("#version") != std::string::npos)
						{
							data = data.substr(data.find("\n") + 1, data.length() - 1);
							MergedCode.push_back(firstLine + "\n");
						}
						for (auto& i : defines)
						{
							std::string define = "#define " + i + "\n";
							MergedCode.push_back(define);
						}

						MergedCode.push_back(data);
						std::string str;
						for (unsigned int i = 0; i < MergedCode.size(); ++i)
							str = str + MergedCode[i].c_str();

						data = str;
					}
					if (includes.size() > 0)
					{
						std::vector<std::string> MergedCode;

						std::string firstLine = data.substr(0, data.find("\n"));
						//If GLSL
						if (firstLine.find("#version") != std::string::npos)
						{
							data = data.substr(data.find("\n") + 1, data.length() - 1);
							MergedCode.push_back(firstLine + "\n");
						}
						for (auto& i : includes)
						{
							std::string define = "#include \"" + i + "\"\n";
							MergedCode.push_back(define);
						}

						MergedCode.push_back(data);
						std::string str;
						for (unsigned int i = 0; i < MergedCode.size(); ++i)
							str = str + MergedCode[i].c_str();

						data = str;
					}
				}
				else {
					if (includes.size() > 0)
					{
						std::vector<std::string> MergedCode;

						std::string firstLine = data.substr(0, data.find("\n"));
						//If GLSL
						if (firstLine.find("#version") != std::string::npos)
						{
							data = data.substr(data.find("\n") + 1, data.length() - 1);
							MergedCode.push_back(firstLine + "\n");
						}
						for (auto& i : includes)
						{
							std::string define = "#include \"" + i + "\"\n";
							MergedCode.push_back(define);
						}

						MergedCode.push_back(data);
						std::string str;
						for (unsigned int i = 0; i < MergedCode.size(); ++i)
							str = str + MergedCode[i].c_str();

						data = str;
					}
					if (defines.size() > 0)
					{
						std::vector<std::string> MergedCode;

						std::string firstLine = data.substr(0, data.find("\n"));
						//If GLSL
						if (firstLine.find("#version") != std::string::npos)
						{
							data = data.substr(data.find("\n") + 1, data.length() - 1);
							MergedCode.push_back(firstLine + "\n");
						}
						for (auto& i : defines)
						{
							std::string define = "#define " + i + "\n";
							MergedCode.push_back(define);
						}

						MergedCode.push_back(data);
						std::string str;
						for (unsigned int i = 0; i < MergedCode.size(); ++i)
							str = str + MergedCode[i].c_str();

						data = str;
					}
				}
				return data;
			}		
	}
}